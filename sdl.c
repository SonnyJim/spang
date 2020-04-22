#include "spang.h"
int fire, left, right, up, down, smartbomb, pause = 0;
extern int running;

const int JOYSTICK_DEAD_ZONE = 8000;

static void input_read_pause (void)
{
    if (input_keyrepeat())
        return;
    if (gamestate == GAME_RUNNING)
    {
        if (paused)
            game_unpause ();
        else
            game_pause ();
    }
}

static void input_read_up (void)
{
    switch (gamestate)
    {
        case GAME_CONFIG:
            config_up ();
            break;
        default:
            break;
    }
}

static void input_read_down (void)
{
    switch (gamestate)
    {
        case GAME_CONFIG:
            config_down ();
            break;
        default:
            break;
    }
}

static void input_read_left (void)
{
    switch (gamestate)
    {
        case GAME_RUNNING:
        case GAME_ENDLEVEL:
            if (!paused)
                player_move_left ();
            break;
        case GAME_HSENTRY:
            hsentry_left ();
            break;
        case GAME_CONFIG:
            config_left ();
            break;
        case GAME_SELECT_RECORD:
            playback_input_left ();
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
        case GAME_ENDLEVEL:
            if (!paused)
                player_move_right ();
            break;
        case GAME_HSENTRY:
            hsentry_right ();
            break;
        case GAME_CONFIG:
            config_right ();
            break;
        case GAME_SELECT_RECORD:
            playback_input_right ();
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
        case GAME_CONFIG:
            config_fire ();
            break;
        case GAME_SELECT_RECORD:
            playback_input_fire ();
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
        case GAME_AMODE:
            gamestate = GAME_CONFIG;
            break;
    }
}

void sdl_read_input (void)
{
    SDL_Event event;
    while (SDL_PollEvent (&event))
    {
        if (gamestate == GAME_CONFIG_INPUT)
        {
            switch (event.type)
            {
                case SDL_KEYDOWN:
                    if (event.key.keysym.sym == SDLK_ESCAPE)
                        gamestate = GAME_CONFIG;
                    break;
                case SDL_JOYAXISMOTION:
                    config_input_read (SDL_JOYAXISMOTION, event.jaxis.axis);
                    break;
                case SDL_JOYBUTTONDOWN:
                    config_input_read (SDL_JOYBUTTONDOWN, event.jbutton.button);
                    break;
            }
        }
        switch (event.type)
        {
            case SDL_QUIT:
                running = 0;
                break;
            case SDL_JOYAXISMOTION:
                if( event.jaxis.axis == joy_cfg.axis1 )
                {
                    if( event.jaxis.value < -JOYSTICK_DEAD_ZONE )
                    {
                        right = 0;
                        left = 1;
                    }
                    else if( event.jaxis.value > JOYSTICK_DEAD_ZONE )
                    {
                        left = 0;
                        right = 1;
                    }
                    else
                    {
                        left = 0;
                        right = 0;
                    }
                }
                else if( event.jaxis.axis == joy_cfg.axis2 )
                {
                    if( event.jaxis.value < -JOYSTICK_DEAD_ZONE )
                    {
                        down = 0;
                        up = 1;
                    }
                    else if( event.jaxis.value > JOYSTICK_DEAD_ZONE )
                    {
                        down = 1;
                        up = 0;
					}
					else
                    {
                        up = 0;
                        down = 0;
                    }
                }
                break;
            case SDL_JOYBUTTONDOWN:
                if (event.jbutton.button == joy_cfg.fire)
                {
                    fire = 1;
                }
                else if (event.jbutton.button == joy_cfg.bomb)
                {
                    smartbomb = 1;
                }
                else if (event.jbutton.button == joy_cfg.pause)
                {
                    pause = 1;
                }
                break;
            case SDL_JOYBUTTONUP:
                if (event.jbutton.button == joy_cfg.fire)
                {
                    fire = 0;
                }
                else if (event.jbutton.button == joy_cfg.bomb)
                {
                    smartbomb = 0;
                }
                else if (event.jbutton.button == joy_cfg.pause)
                {
                    pause = 0;
                }
                break;
            case SDL_KEYDOWN:
                switch (event.key.keysym.sym)
                {
                    case SDLK_ESCAPE:
                        game_unpause ();
                        if (gamestate == GAME_RUNNING || gamestate == GAME_ENDLEVEL)
                            gamestate = GAME_OVER;
                        else if (gamestate == GAME_CONFIG)
                            gamestate = GAME_AMODE;
                        else if (gamestate == GAME_CONFIG_INPUT)
                            gamestate = GAME_CONFIG;
                        else if (gamestate == GAME_AMODE)
                            running = 0;
                        else if (gamestate == GAME_SELECT_RECORD)
                            playback_quit ();
                        break;
                    case SDLK_h:
                        if (draw_hitbox)
                            draw_hitbox = 0;
                        else
                            draw_hitbox = 1;
                        break;
                    case SDLK_m:
                        if ( Mix_VolumeMusic(-1) == 0)
                        {
                            msg_show ("Music on", screen_width / 2, screen_height /2, 2, font1, ALIGN_CENTRE, green);
                            Mix_VolumeMusic (128);
                        }
                        else
                        {
                            msg_show ("Music off", screen_width / 2, screen_height /2, 2, font1, ALIGN_CENTRE, green);
                            Mix_VolumeMusic(0);
                        }
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
                    case SDLK_UP:
                        up = 1;
                        break;
                    case SDLK_DOWN:
                        down = 1;
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
                    case SDLK_UP:
                        up = 0;
                        break;
                    case SDLK_DOWN:
                        down = 0;
                        break;
                    default:
                        break;
                }
                break;
        }
    }

    if (pause)
        input_read_pause ();
    if (paused)
        return;
    if (gamestate == GAME_RUNNING ||gamestate == GAME_ENDLEVEL)
        frame_counter++;
    if (record_state == REC_PLAYING)
    {
        playback_frame ();
        if ((input_mask & REC_FIRE) == REC_FIRE)
            input_read_fire ();
        if ((input_mask & REC_BOMB) == REC_BOMB)
            input_read_smartbomb ();
        if ((input_mask & REC_LEFT) == REC_LEFT)
            input_read_left ();
        else if ((input_mask & REC_RIGHT) == REC_RIGHT)
            input_read_right ();
        return;
    }

    input_mask = 0;
    if (fire)
    {
        input_mask |= REC_FIRE;
        input_read_fire ();
    }
    if (smartbomb)
    {
        input_mask |= REC_BOMB;
        input_read_smartbomb ();
    }

    if (left && !right)
    {
        input_mask |= REC_LEFT;
        input_read_left ();
    }
    else if (right && !left)
    {
        input_mask |= REC_RIGHT;
        input_read_right ();
    }

    if (up && !down)
        input_read_up ();
    else if (down && !up)
        input_read_down ();
    if (record_state == REC_REC && (gamestate == GAME_RUNNING || gamestate == GAME_ENDLEVEL))
        record_frame ();
}

int sdl_init (void)
{
    SDL_DisplayMode videomode;
    SDL_Window *window;

    if (SDL_Init (SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_JOYSTICK| SDL_INIT_EVENTS) != 0)
    {
        fprintf (stderr, "\nError initialising SDL: %s\n", SDL_GetError ());
        return 1;
    }
    input_joystick_init ();

    if (SDL_GetCurrentDisplayMode (0, &videomode) != 0)
    {
        fprintf (stderr, "Error getting current display mode: %s\n", SDL_GetError ());
        return 1;
    }

    //screen_height = videomode.h;
    //screen_width = videomode.w;

    screen_width = 1024;
    screen_height = 600;

    window = SDL_CreateWindow("Spang!", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, screen_width, screen_height, 0);
    //window = SDL_CreateWindow("Spang!", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, screen_width, screen_height, SDL_WINDOW_FULLSCREEN);

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

/*
    int flags = MIX_INIT_OGG|MIX_INIT_MOD|MIX_INIT_OGG;
    int initted = Mix_Init (flags);
    if ((initted&flags) != flags)
    {
        fprintf (stderr, "Mix_Init: Failed to init required ogg and mod support!\n");
        fprintf (stderr, "Mix_Init: %s\n", Mix_GetError());
    }
    //music = Mix_LoadMUS( "data/sfx/cyberrid.mod" );

*/
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

void sdl_close (void)
{
    input_joystick_close ();
    Mix_FreeChunk (laser1);
    Mix_FreeChunk (explosion);
    Mix_CloseAudio();
    SDL_Quit ();

}
