#include "spang.h"
#define ENEMY_BULLET_TIME 100

void enemy_bullet_add (int xpos, int ypos, bullet_t type)
{
    fprintf (stdout, "adding bullet\n");
    int i;
    for (i = 0; i < MAX_ENEMIES;i++)
    {
        if (enemy_bullets[i].type == BULLET_NONE)
        {
            enemy_bullets[i].type = type;
            enemy_bullets[i].rect.x = xpos;
            enemy_bullets[i].rect.y = ypos;
            enemy_bullets[i].rect.w = 5;
            enemy_bullets[i].rect.h = 20;
            enemy_bullets[i].yvel = 10;
            return;
        }
    }
    fprintf (stderr, "Error: Ran out of space for enemy bullets\n");
}

void enemy_bullet_remove (int bullet)
{
    enemy_bullets[bullet].type = BULLET_NONE;
}

void enemy_init (void)
{
    int i;

    for (i = 0; i < MAX_ENEMIES; i++)
    {
        enemies[i].type = ENEMY_NONE;
        enemies[i].time = 0;
        enemy_bullets[i].type = ENEMY_NONE;
    }
}

int enemy_add (enemy_t type, int xpos, int ypos, float xvel, float yvel, int strength)
{
    int i;
    for (i = 0; i < MAX_ENEMIES;i++)
    {
        if (enemies[i].type == ENEMY_NONE)
        {
            enemies[i].type = type;
            enemies[i].rect.x = xpos - (20 /2);
            enemies[i].rect.y = ypos - (20 /2);
            enemies[i].rect.w = 40;
            enemies[i].rect.h = 40;
            enemies[i].xvel = xvel;
            enemies[i].yvel = yvel;
            enemies[i].time = SDL_GetTicks ();
            enemies[i].hits = 0;
            enemies[i].strength = strength;
            return i;
        }
    }
    return MAX_ENEMIES;
}

static void enemy_remove (int num)
{
    enemies[num].type = ENEMY_NONE;
}

static void enemy_score (void)
{
    player.score += 1000;
}

void enemy_hit (int num)
{
    combo_increment ();
    enemies[num].hits++;
    if (enemies[num].hits < enemies[num].strength)
    {
        Mix_PlayChannel (SND_EXPLOSION, tink, 0);
        return;
    }
    player_ball_destroyed ();
    Mix_PlayChannel( SND_EXPLOSION, explosion, 0 );
    explosion_add (enemies[num].rect.x + (enemies[num].rect.w / 2),
                               enemies[num].rect.y + (enemies[num].rect.h /2 ));
    enemy_remove (num);
    enemy_score ();
}

static void enemy_update_centipede (int i)
{

    enemies[i].rect.x += enemies[i].xvel;
    enemies[i].rect.y += enemies[i].yvel;

    if (enemies[i].rect.x <= 0)
    {
        enemies[i].xvel = -enemies[i].xvel;
        enemies[i].rect.y += enemies[i].rect.h;
    }
    else if (enemies[i].rect.x + enemies[i].rect.w >= screen_width)
    {
        enemies[i].xvel = -enemies[i].xvel;
        enemies[i].rect.y += enemies[i].rect.h;
    }

    if (enemies[i].rect.y >= screen_height)
        enemy_remove (i);

    if (check_axis_x (enemies[i].rect, player.rect) && enemies[i].time + ENEMY_BULLET_TIME < SDL_GetTicks ())
    {
        enemies[i].time = SDL_GetTicks ();
        enemy_bullet_add (enemies[i].rect.x + (enemies[i].rect.w /2), enemies[i].rect.y, BULLET_NORMAL);
    }
}

void enemy_bullet_update (void)
{
    int i;
    for (i = 0; i < MAX_ENEMIES; i++)
    {
        if (enemy_bullets[i].type != BULLET_NONE)
        {
            enemy_bullets[i].rect.y += enemy_bullets[i].yvel;
            if (enemy_bullets[i].rect.y >= screen_height)
                enemy_bullets[i].type = BULLET_NONE;
            else
                detect_enemy_bullet (enemy_bullets[i].rect, i);
        }
    }
}

void enemy_update (void)
{
    int i;
    for (i = 0; i < MAX_ENEMIES; i++)
    {
        if (enemies[i].type != ENEMY_NONE)
        {
            switch (enemies[i].type)
            {
                case ENEMY_CENTIPEDE:
                    enemy_update_centipede (i);
                    break;
                case ENEMY_NONE:
                default:
                    break;
            }
        }
    }
}

static void enemy_draw_centipede (int num)
{
    //SDL_RenderDrawRect( renderer, &enemies[num].rect);
    SDL_RenderCopy (renderer, enemy_tex, NULL, &enemies[num].rect);
}

static void enemy_draw_bullets (void)
{
    int i;
    for (i = 0; i < MAX_ENEMIES; i++)
    {
        if (enemy_bullets[i].type != BULLET_NONE)
        {
            SDL_RenderDrawRect (renderer, &enemy_bullets[i].rect);

        }
    }
}

void enemy_draw (void)
{
    int i;
    SDL_SetRenderDrawColor( renderer, 0, 255, 0, 0 );
    enemy_update ();
    enemy_bullet_update ();

    for (i = 0; i < MAX_ENEMIES; i++)
    {
        if (enemies[i].type != ENEMY_NONE)
        {
            enemy_draw_centipede (i);
            //SDL_RenderDrawRect( renderer, &enemies[i].rect);
        }
    }
    enemy_draw_bullets ();
}
