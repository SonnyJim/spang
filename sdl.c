#include "spang.h"
int fire, left, right = 0;
extern int running;

Mix_Chunk *laser1 = NULL;
Mix_Chunk *explosion = NULL;
Mix_Chunk *playerhit = NULL;
Mix_Chunk *death = NULL;
Mix_Chunk *comboup = NULL;
Mix_Chunk *combodown = NULL;
Mix_Music *music = NULL;
Mix_Chunk *test = NULL;

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
                    case SDLK_m:
                        if ( Mix_PausedMusic())
                            Mix_ResumeMusic();
                        else
                            Mix_PauseMusic();
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
                    case SDLK_SPACE:
                        if (player.smartbomb)
                            powerup_smartbomb ();
                        break;
                    case SDLK_p:
                        if (paused)
                            paused = 0;
                        else
                            paused = 1;
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
    if (paused)
        return;
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

    int flags = MIX_INIT_OGG|MIX_INIT_MOD;
    int initted = Mix_Init (flags);
    if ((initted&flags) != flags)
    {
        fprintf (stderr, "Mix_Init: Failed to init required ogg and mod support!\n");
        fprintf (stderr, "Mix_Init: %s\n", Mix_GetError());
    }
    music = Mix_LoadMUS( "data/sfx/cyberrid.mod" );
    if (music == NULL)
        fprintf (stderr, "Error loading music\n");

    if( Mix_OpenAudio( 44100, MIX_DEFAULT_FORMAT, 2, 1024 ) == -1 )
        return 1;
    Mix_AllocateChannels (32);
    Mix_PlayMusic (music, -1);
    if (TTF_Init () == -1)
    {
        fprintf (stderr, "Error setting up font system\n");
        return 1;
    }
    return fonts_load ();
}
