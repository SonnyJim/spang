#include "spang.h"
#define MAX_POWERUPS 128
#define POWERUP_DISPLAY_TIME 200
#define MEGASHOT_TIME 400
#define NUM_POWERUPS 5

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
SDL_Texture *powerups_tex[NUM_POWERUPS];
const char *powerups_msg[NUM_POWERUPS + 1] = { "NONE", "Shield Up", "1000", "Slowdown", "MEGASHOT"};

void powerups_textures_init (void)
{
    powerups_tex[POWERUP_HEALTH] = health_tex;
    powerups_tex[POWERUP_COIN] = coin_tex;
    powerups_tex[POWERUP_SLOW] = slow_tex;
    powerups_tex[POWERUP_MEGASHOT] = megashot_tex;
}

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
    powerups_textures_init ();
}

void powerup_add (powerup_t type, int xpos, int ypos)
{
    //fprintf (stdout, "Adding power up at %li hits\n", player.hits);
    int i;
    for (i = 0; i < MAX_POWERUPS; i++)
    {
        if (powerups[i].type == POWERUP_NONE)
        {
            if (xpos + 48 > (screen_width - 48))
                xpos -= 48;
            powerups[i].type = type;
            powerups[i].time = 0;
            powerups[i].rect.x = xpos;
            powerups[i].rect.y = ypos;
            powerups[i].rect.w = 48;
            powerups[i].rect.h = 48;
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
    Mix_PlayChannel (SND_SIREN, siren, 0);
    megashot_timer = frame_counter;
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
        int type = powerups[num].type;
        msg_show ((char *) powerups_msg[type], powerups[num].rect.x + (powerups[num].rect.w / 2),
                    player.rect.y - powerups[num].rect.w, 2, font3, ALIGN_TCENTRE, white);
        powerup_collect (num);
    }
}
void powerups_update (int i)
{
    //Move powerup down the screen and start the timer when they hit the bottom
    if (powerups[i].rect.y + 5 < (screen_height - powerups[i].rect.h))
        powerups[i].rect.y += 5;
    else if (powerups[i].time == 0)
        powerups[i].time = frame_counter;
    else if (powerups[i].time + POWERUP_DISPLAY_TIME - 100 < frame_counter)
        powerups[i].rect.h -= 1;
}

void powerup_megashot_disable (void)
{
    if (!megashot_active)
        return;
    Mix_ResumeMusic ();
    if (!bonus_level_active)
        Mix_PlayChannel(-1, alarmreverse, 0);
    megashot_active = 0;
}

void powerups_draw (void)
{
    int i;
    for (i = 0; i < MAX_POWERUPS; i++)
    {
        if (powerups[i].type != POWERUP_NONE)
        {
            if (powerups[i].type == POWERUP_MEGASHOT && bonus_level_active)
                powerups[i].type = POWERUP_NONE;
            if (powerups[i].time != 0 &&
                powerups[i].time + POWERUP_DISPLAY_TIME < frame_counter)
            {
                powerups[i].type = POWERUP_NONE;
                break;
            }
            else
            {
                powerups_update (i);

                SDL_RenderCopy (renderer, powerups_tex[powerups[i].type], NULL, &powerups[i].rect);
                powerups_check_collision(i);
            }
        }
    }
    if (megashot_active && megashot_timer + MEGASHOT_TIME > frame_counter)
    {
        Mix_PauseMusic ();
        combo_set_level (4);
    }
    else if (megashot_active && megashot_timer + MEGASHOT_TIME < frame_counter)
    {
        powerup_megashot_disable ();
    }
}

void powerup_smartbomb (void)
{
    int i;
    if (player.smartbomb == 0 || bonus_level_active || gamestate != GAME_RUNNING)
        return;
    msg_show ("KABLAMO", 0, (screen_height / 2) - 100, 1, font2, ALIGN_CENTRE, red);
    player.smartbomb = 0;
    explosion_superbomb (screen_width /2, screen_height / 2);
    for (i = 0; i < MAX_BALLS;i++)
    {
        if (balls[i].size > 0)
        {
            player.shots_fired_round++;

            balls[i].hits = balls[i].strength;
            ball_hit(i);
        }
    }

    for (i = 0; i < MAX_ENEMIES; i++)
    {
        if (enemies[i].type != ENEMY_NONE)
        {
            player.shots_fired_round++;
            enemies[i].hits = enemies[i].strength;
            enemy_hit (i);
        }
    }
    if (player.speed > 1)
        player.speed = 1;
}
