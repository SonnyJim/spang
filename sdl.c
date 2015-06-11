#include "spang.h"
int fire, left, right = 0;
extern int running;

Mix_Chunk *laser1 = NULL;
Mix_Chunk *explosion = NULL;
Mix_Chunk *playerhit = NULL;
Mix_Chunk *death = NULL;
Mix_Chunk *comboup = NULL;
Mix_Chunk *combodown = NULL;

void sdl_read_input (void)
{
    SDL_Event event;
    while (SDL_PollEvent (&event))
    {
        switch (event.type)
        {
            case SDL_QUIT:
                running = 0;
                break;

            case SDL_KEYDOWN:
                switch (event.key.keysym.sym)
                {
                    case SDLK_ESCAPE:
                        running = 0;
                        break;
                    case SDLK_b:
                        ball_add (5, screen_height / 2, screen_width / 2, player.speed, 1);
                        break;
                    case SDLK_h:
                        if (draw_hitbox)
                            draw_hitbox = 0;
                        else
                            draw_hitbox = 1;
                        break;
                    case SDLK_LCTRL:
                        fire = 1;
                        break;
                    case SDLK_LEFT:
                        left = 1;
                        break;
                    case SDLK_RIGHT:
                        right = 1;
                        break;
                }
                break;
            case SDL_KEYUP:
                switch (event.key.keysym.sym)
                {
                    case SDLK_LCTRL:
                        fire = 0;
                        break;
                    case SDLK_LEFT:
                        left = 0;
                        break;
                    case SDLK_RIGHT:
                        right = 0;
                        break;
                }
                break;
        }
    }

    if (fire)
        bullet_add ();
    if (left && !right)
        player_move_left ();
    else if (right && !left)
        player_move_right ();
}

int sdl_init (void)
{
       SDL_DisplayMode videomode;
    SDL_Window *window;

    if (SDL_Init (SDL_INIT_VIDEO |SDL_INIT_AUDIO) != 0)
    {
        fprintf (stderr, "\nError initialising SDL: %s\n", SDL_GetError ());
        return 1;
    }

    if (SDL_GetCurrentDisplayMode (0, &videomode) != 0)
    {
        fprintf (stderr, "Error getting current display mode: %s\n", SDL_GetError ());
        return 1;
    }

  //  screen_height = videomode.h;
  //  screen_width = videomode.w;

    screen_width = 1024;
    screen_height = 600;

    window = SDL_CreateWindow("Spang!", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, screen_width, screen_height, 0);

    if (window == NULL)
    {
        fprintf (stderr, "Error creating window: %s\n", SDL_GetError());
        return 1;
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

    if (renderer == NULL)
    {
        fprintf (stderr, "Error creating software renderer: %s\n", SDL_GetError());
        return 1;
    }

    if( Mix_OpenAudio( 22050, MIX_DEFAULT_FORMAT, 2, 512 ) == -1 )
        return 1;
    Mix_AllocateChannels (32);

    if (TTF_Init () == -1)
    {
        fprintf (stderr, "Error setting up font system\n");
        return 1;
    }
    return fonts_load ();
}
