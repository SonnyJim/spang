#include "spang.h"
#define MAX_POWERUPS 32
#define POWERUP_DISPLAY_TIME 3000
#define MEGASHOT_TIME 6000

SDL_Texture *health_tex = NULL;
SDL_Texture *coin_tex = NULL;
SDL_Texture *slow_tex = NULL;
SDL_Texture *megashot_tex = NULL;

Mix_Chunk *health1;
Mix_Chunk *kaching;
Mix_Chunk *slowdown;
Mix_Chunk *alarmreverse;

Uint32 megashot_timer = 0;
int megashot_active = 0;

struct powerup_t
{
    int type;
    Uint32 time;
    SDL_Rect rect;
};

struct powerup_t powerups[MAX_POWERUPS];
SDL_Texture *powerups_tex[3];

void powerups_init (void)
{
    int i;
    for (i = 0; i < MAX_POWERUPS; i++)
    {
        powerups[i].type = POWERUP_NONE;
        powerups[i].time = 0;
        powerups[i].rect.w = 48;
        powerups[i].rect.h = 48;
        powerups[i].rect.x = 0;
        powerups[i].rect.y = 0;
    }
    powerups_tex[POWERUP_HEALTH] = health_tex;
    powerups_tex[POWERUP_COIN] = coin_tex;
    powerups_tex[POWERUP_SLOW] = slow_tex;
    powerups_tex[POWERUP_MEGASHOT] = megashot_tex;
}

void powerup_add (int type, int xpos, int ypos)
{
    fprintf (stdout, "Adding power up at %li hits\n", player.hits);
    int i;
    for (i = 0; i < MAX_POWERUPS; i++)
    {
        if (powerups[i].type == POWERUP_NONE)
        {
            powerups[i].type = type;
            powerups[i].time = SDL_GetTicks ();
            powerups[i].rect.x = xpos;
            powerups[i].rect.y = ypos;
            return;
        }
    }
    fprintf (stderr, "Error:  Couldn't add powerup, no free spaces?\n");
}

static void powerup_health (void)
{
    //fprintf (stdout, "HEALTHZ\n");
    Mix_PlayChannel (-1, health1, 0);
    if (player.health >= 100)
        player.score += 2000;
    player.health += 20;
    if (player.health > 100)
        player.health = 100;

}

static void powerup_coin (void)
{
    Mix_PlayChannel (-1, kaching, 0);
    player.score += 1000;
}

static void powerup_slow (void)
{
    Mix_PlayChannel (SND_MUSIC, slowdown, 0);
    if (player.speed > 1)
        player.speed--;
    balls_speed_change ();
}

static void powerup_megashot (void)
{
    megashot_timer = SDL_GetTicks ();
    megashot_active = 1 ;
}

void powerup_collect (int num)
{
    switch (powerups[num].type)
    {
        case POWERUP_HEALTH:
            powerup_health ();
            break;
        case POWERUP_COIN:
            powerup_coin ();
            break;
        case POWERUP_SLOW:
            powerup_slow ();
            break;
        case POWERUP_MEGASHOT:
            powerup_megashot ();
            break;
    }
    powerups[num].type = POWERUP_NONE;
}

void powerups_check_collision (int num)
{
    if (check_axis (powerups[num].rect, player_hitrect2))
    {
        powerup_collect (num);
    }
}

void powerups_draw (void)
{
    int i;
    for (i = 0; i < MAX_POWERUPS; i++)
    {
        if (powerups[i].type != POWERUP_NONE)
        {

            if (powerups[i].time + POWERUP_DISPLAY_TIME < SDL_GetTicks ())
            {
                powerups[i].type = POWERUP_NONE;
                break;
            }
            else
            {

                if (powerups[i].rect.y < (screen_height - powerups[i].rect.h))
                    powerups[i].rect.y += 5;
                SDL_RenderCopy (renderer, powerups_tex[powerups[i].type], NULL, &powerups[i].rect);
                powerups_check_collision(i);
            }
        }
    }
    if (megashot_active && megashot_timer + MEGASHOT_TIME > SDL_GetTicks ())
    {
        Mix_HaltMusic ();
        combo_set_level (4);
    }
    else if (megashot_active && megashot_timer + MEGASHOT_TIME < SDL_GetTicks ())
    {
        Mix_ResumeMusic ();
        Mix_PlayChannel(-1, alarmreverse, 0);
        megashot_active = 0;
    }
}

void powerup_smartbomb (void)
{
    int i;
    player.smartbomb = 0;
    for (i = 0; i < NUM_BALLS;i++)
    {
        if (balls[i].size > 0)
            ball_split(i);
    }
}
