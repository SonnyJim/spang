#include "spang.h"
int fire, left, right, smartbomb = 0;
extern int running;

static void input_read_left (void)
{
    switch (gamestate)
    {
        case GAME_RUNNING:
            if (!paused)
                player_move_left ();
            break;
        case GAME_HSENTRY:
            hsentry_left ();
            break;
        default:
            break;
    }
}

static void input_read_right (void)
{
    switch (gamestate)
    {
        case GAME_RUNNING:
            if (!paused)
                player_move_right ();
            break;
        case GAME_HSENTRY:
            hsentry_right ();
            break;
        default:
            break;
    }
}

static void input_read_fire (void)
{
    switch (gamestate)
    {
        case GAME_RUNNING:
            if (!paused)
                bullet_add ();
            break;
        case GAME_AMODE:
            amode_fire ();
            break;
        case GAME_HSENTRY:
            hsentry_fire ();
            break;
        default:
            break;
    }
}

static void input_read_smartbomb (void)
{
   switch (gamestate)
    {
        case GAME_RUNNING:
            if (!paused)
                powerup_smartbomb ();
            break;
    }
}

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
                        if (gamestate == GAME_RUNNING)
                            gamestate = GAME_OVER;
                        else
                            running = 0;
                        break;
                    case SDLK_h:
                        if (draw_hitbox)
                            draw_hitbox = 0;
                        else
                            draw_hitbox = 1;
                        break;
                    case SDLK_m:
                        if ( Mix_VolumeMusic(-1) == 0)
                            Mix_VolumeMusic (128);
                        else
                            Mix_VolumeMusic(0);
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
                        smartbomb = 1;
                        break;
                    case SDLK_p:
                        if (paused)
                            game_unpause ();
                        else
                            game_pause ();
                        break;
                    case SDLK_z:
                        player.level++;
                        break;
                    case SDLK_x:
                        player.level--;
                        break;
                    default:
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
                    case SDLK_SPACE:
                        smartbomb = 0;
                        break;
                    default:
                        break;
                }
                break;
        }
    }
    if (fire)
        input_read_fire ();
    if (smartbomb)
        input_read_smartbomb ();
    if (left && !right)
        input_read_left ();
    else if (right && !left)
        input_read_right ();
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
    SDL_ShowCursor(SDL_DISABLE);

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

    int flags = MIX_INIT_OGG|MIX_INIT_MOD|MIX_INIT_OGG;
    int initted = Mix_Init (flags);
    if ((initted&flags) != flags)
    {
        fprintf (stderr, "Mix_Init: Failed to init required ogg and mod support!\n");
        fprintf (stderr, "Mix_Init: %s\n", Mix_GetError());
    }
    //music = Mix_LoadMUS( "data/sfx/cyberrid.mod" );

    if( Mix_OpenAudio( 44100, MIX_DEFAULT_FORMAT, 2, 2048 ) == -1 )
        return 1;
    Mix_AllocateChannels (16);
    if (TTF_Init () == -1)
    {
        fprintf (stderr, "Error setting up font system\n");
        return 1;
    }
    return fonts_init ();
}
