#include "spang.h"
#define MAX_POWERUPS 5
#define POWERUP_DISPLAY_TIME 3000

SDL_Texture *health_tex;
Mix_Chunk *health;

struct powerup_t
{
    int type;
    Uint32 time;
    SDL_Rect rect;
};

struct powerup_t powerups[MAX_POWERUPS];

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
}

void powerup_add (int type, int xpos, int ypos)
{
    fprintf (stdout, "Adding power up at %i hits\n", player.hits);
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

void powerup_collect (int num)
{
    if (powerups[num].type == POWERUP_HEALTH)
    {
        Mix_PlayChannel (SND_MUSIC, health, 0);
        if (player.health > 100)
            player.score += 2000;
        player.health += 20;
        if (player.health > 100)
            player.health = 100;
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
                SDL_RenderCopy (renderer, health_tex, NULL, &powerups[i].rect);
                powerups_check_collision(i);
            }
        }
    }
}

