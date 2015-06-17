#include "spang.h"
#define CONFIG_FILE "spang.cfg"
#define NUM_OPTIONS 4
#define NUM_INPUT_OPTS 7

SDL_Color color;
SDL_Color dark_green = {0, 140, 0, 0};

int config_index = 0;
int config_input_index = 0;
Uint32 total_playtime = 0;

const char* config_input_text[] = {"Left", "Right", "Up", "Down", "Fire", "Smartbomb", "Pause"};

enum
{
    CFG_EXPLOSION,
    CFG_ROTATE,
    CFG_JOYSTICK,
    CFG_EXIT
};

struct config_opt
{
    char *text;
    int *ip; //Pointer to int
    char *option1;
    char *option2;
};

struct config_opt config_options[NUM_OPTIONS];

void config_init (void)
{
    config_options[0].text = "Explosion type";
    config_options[0].option1 = "Cool";
    config_options[0].option2 = "Red";
    config_options[0].ip = &explosion_type;

    config_options[1].text = "Background";
    config_options[1].option1 = "No Rotate";
    config_options[1].option2 = "Rotate";
    config_options[1].ip = &bg_rotate;

    config_options[2].text = "Config Joystick";
    config_options[2].option1 = "Not Implemented";
    config_options[2].option2 = "";
    config_options[2].ip = NULL;

    config_options[3].text = "Exit";
    config_options[3].option1 = "";
    config_options[3].option2 = "";
    config_options[3].ip = NULL;

    //Init explosions so we can test them
    explosions_init ();

}

static int config_save (void)
{
    FILE *fp;
    fp = fopen (CONFIG_FILE, "w");
    if (fp == NULL)
    {
        fprintf (stderr, "Error writing config file\n");
        return 1;
    }
    fwrite (&explosion_type, sizeof(int), 1, fp);
    fwrite (&bg_rotate, sizeof(int), 1, fp);
    fwrite (&joy_cfg, sizeof(joy_cfg), 1, fp);
    fwrite (&total_playtime, sizeof(total_playtime), 1, fp);
    fclose (fp);
    return 0;
}

int config_load (void)
{
    FILE *fp;

    config_init ();

    fp = fopen (CONFIG_FILE, "r");
    if (fp == NULL)
    {
        fprintf (stderr, "Error reading config file\n");
        config_save ();
        return 1;
    }
    fread (&explosion_type, sizeof(int), 1, fp);
    fread (&bg_rotate, sizeof(int), 1, fp);
    fread (&joy_cfg, sizeof(joy_cfg), 1, fp);
    fread (&total_playtime, sizeof(total_playtime), 1, fp);
    fclose (fp);
    return 0;
}

static void config_input_start (void)
{
    config_input_index = 0;
    gamestate = GAME_CONFIG_INPUT;
    joy_cfg_new.axis1 = 128;
    joy_cfg_new.axis2 = 128;
    joy_cfg_new.fire = 128;
    joy_cfg_new.bomb = 128;
    joy_cfg_new.pause = 128;
}

static void config_input_stop (void)
{
    gamestate = GAME_CONFIG;
    joy_cfg.axis1 = joy_cfg_new.axis1;
    joy_cfg.axis2 = joy_cfg_new.axis2;
    joy_cfg.fire = joy_cfg_new.fire;
    joy_cfg.bomb = joy_cfg_new.bomb;
    joy_cfg.pause = joy_cfg_new.pause;
}

void config_input_read (int mode, int value)
{
    if (input_keyrepeat())
        return;
    switch (config_input_index)
    {
        case 0:
        case 1:
            if (mode == SDL_JOYAXISMOTION)
            {
                joy_cfg_new.axis1 = value;
                config_input_index++;
                Mix_PlayChannel (-1, comboup, 0);

            }
            break;
        case 2:
        case 3:
            if (mode == SDL_JOYAXISMOTION && value != joy_cfg_new.axis1)
            {
                joy_cfg_new.axis2 = value;
                config_input_index++;
                Mix_PlayChannel (-1, comboup, 0);
            }
            break;
        case 4:
            if (mode == SDL_JOYBUTTONDOWN)
            {
                joy_cfg_new.fire = value;
                config_input_index++;
                Mix_PlayChannel (-1, comboup, 0);
            }
            break;
        case 5:
            if (mode == SDL_JOYBUTTONDOWN && value != joy_cfg_new.fire)
            {
                joy_cfg_new.bomb = value;
                config_input_index++;
                Mix_PlayChannel (-1, comboup, 0);
            }
            break;
        case 6:
            if (mode == SDL_JOYBUTTONDOWN && value != joy_cfg_new.fire && value != joy_cfg_new.bomb)
            {
                joy_cfg_new.pause = value;
                config_input_stop ();
                Mix_PlayChannel (-1, comboup, 0);
            }
            break;
    }
}

void config_up (void)
{
    if (input_keyrepeat())
        return;
    if (config_index > 0)
    {
        config_index--;
        Mix_PlayChannel (-1, comboup, 0);
    }
    else
        Mix_PlayChannel (-1, tink, 0);
}

void config_down (void)
{
    if (input_keyrepeat())
        return;
    if (config_index < NUM_OPTIONS - 1)
    {
        config_index++;
        Mix_PlayChannel (-1, comboup, 0);
    }
    else
        Mix_PlayChannel (-1, tink, 0);
}

void config_fire (void)
{
    if (input_keyrepeat())
        return;

    Mix_PlayChannel (SND_LASER, speedup, 0);
    switch (config_index)
    {
        case CFG_EXIT:
            config_save ();
            gamestate = GAME_AMODE;
            break;
        case CFG_ROTATE:
            if (bg_rotate)
                bg_rotate = 0;
            else
                bg_rotate = 1;
            break;
        case CFG_EXPLOSION:
            trippy_mode = 3;
            if (explosion_type)
                explosion_type = 0;
            else
                explosion_type = 1;
            explosion_add (screen_width /2, screen_height /2);
            break;
        case CFG_JOYSTICK:
            config_input_start ();
            break;
        default:
            break;
    }
}

void config_left (void)
{
    if (input_keyrepeat())
        return;
}

void config_right (void)
{
    if (input_keyrepeat())
        return;
}

static void config_input_draw (void)
{
    char buffer[64];

    if (config_input_index >= NUM_INPUT_OPTS)
        gamestate = GAME_CONFIG;

    sprintf (buffer, "Press joystick button for %s", config_input_text[config_input_index]);
    render_string_centre (buffer, screen_height - 100, green, font3);
}

void config_loop (void)
{
    int i;
    char buffer[64];

    render_string_centre ("OPTIONS", 0, green, font2);

    for (i = 0; i < NUM_OPTIONS; i++)
    {
        if (i == config_index)
            color = green;
        else
            color = dark_green;

        render_string (config_options[i].text, (screen_width / 2) - 200, 100 + (i * 30), color, font3);

        if (config_options[i].ip != NULL)
        {
            if (*config_options[i].ip != 0)
                sprintf (buffer, "%s", config_options[i].option1);
            else
                sprintf (buffer, "%s", config_options[i].option2);
            render_string (buffer, (screen_width / 2) + 10, 100 + (i * 30), color, font3);
        }
    }
    if (gamestate == GAME_CONFIG_INPUT)
    {
        config_input_draw ();
    }
    stars_draw ();
    explosions_draw ();
}
