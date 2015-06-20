#include "spang.h"
//Delay between firing normal bullets
#define ENEMY_BULLET_NORMAL_DELAY 60
#define ENEMY_BULLET_HOMING_DELAY 120
#define GRAVITY_BALL 2

void enemy_bullet_add (int xpos, int ypos, bullet_t type)
{
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
            if (type == BULLET_HOMING)
            {
                enemy_bullets[i].yvel = 2;
                enemy_bullets[i].xvel = 2;
            }
            else
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

int enemy_add (enemy_t type, bullet_t bullet, int xpos, int ypos, float xvel, float yvel,
               int strength, int size)
{
    int i, w, h;
    if (size <= 0)
        size = 1;


    switch (type)
    {
        case ENEMY_CENTIPEDE:
        case ENEMY_HOMING:
            w = CENTIPEDE_WIDTH;
            h = w;
            break;
        case ENEMY_BALL:
            w = BALL_WIDTH;
            h = w;
            break;
        default:
            w = 20;
            h = 20;
            break;
    }

    if (xpos < 0)
        xpos = 0;
    else if (xpos >= screen_width)
        xpos = screen_width - (w * size);

    if (ypos > screen_height)
        ypos = screen_height - (h * size);

    for (i = 0; i < MAX_ENEMIES;i++)
    {
        if (enemies[i].type == ENEMY_NONE)
        {
            enemies[i].type = type;
            enemies[i].bullet = bullet;
            enemies[i].rect.x = xpos - (20 /2);
            enemies[i].rect.y = ypos - (20 /2);
            enemies[i].rect.w = w * size;
            enemies[i].rect.h = h * size;
            enemies[i].size = size;
            enemies[i].xvel = xvel;
            enemies[i].yvel = yvel;
            enemies[i].time = 0;
            enemies[i].hits = 0;
            enemies[i].strength = strength;
            if (type == ENEMY_CENTIPEDE)
                enemies[i].yvel = 0;
            return i;
        }
    }
    return MAX_ENEMIES;
}

void enemy_remove (int num)
{
    enemies[num].type = ENEMY_NONE;
}

static void enemy_ball_hit (int num)
{
    if (enemies[num].size == 1)
    {
        enemies[num].type = ENEMY_NONE;
        return;
    }

    if (enemies[num].xvel > 0)
    {
        enemy_add (ENEMY_BALL, BULLET_NONE, enemies[num].rect.x + (enemies[num].rect.w / 2), enemies[num].rect.y,
                enemies[num].xvel, -2, enemies[num].strength - 1, enemies[num].size -1);
        enemy_add (ENEMY_BALL, BULLET_NONE, enemies[num].rect.x - (enemies[num].rect.w / 2), enemies[num].rect.y,
               -enemies[num].xvel, -2, enemies[num].strength - 1, enemies[num].size -1);
    }
    else
    {
        enemy_add (ENEMY_BALL, BULLET_NONE, enemies[num].rect.x + (enemies[num].rect.w / 2), enemies[num].rect.y,
                -enemies[num].xvel, -2, enemies[num].strength - 1, enemies[num].size -1);
        enemy_add (ENEMY_BALL, BULLET_NONE, enemies[num].rect.x - (enemies[num].rect.w / 2), enemies[num].rect.y,
               enemies[num].xvel, -2, enemies[num].strength - 1, enemies[num].size -1);
    }
    enemies[num].type = ENEMY_NONE;
}

void enemy_hit (int num)
{
    combo_increment ();
    enemies[num].hits++;
    player_hit_inc (enemies[num].rect.x, enemies[num].rect.y);
    if (enemies[num].hits < enemies[num].strength)
    {
        Mix_PlayChannel (SND_EXPLOSION, tink, 0);
        return;
    }
    player_ball_destroyed ();
    Mix_PlayChannel( SND_EXPLOSION, explosion, 0 );
    explosion_add (enemies[num].rect.x + (enemies[num].rect.w / 2),
                               enemies[num].rect.y + (enemies[num].rect.h /2 ));
    switch (enemies[num].type)
    {
        case ENEMY_CENTIPEDE:
        case ENEMY_HOMING:
            combo_hit (enemies[num].size);
            player_score (enemies[num].size);
            enemy_remove (num);
            break;
        case ENEMY_BALL:
            combo_hit (enemies[num].size);
            player_score (enemies[num].size);
            enemy_ball_hit (num);
            break;
        default:
            break;
    }
    //enemy_score ();
}

static void enemy_bullet_check_to_fire (int i)
{
    //Check to see if the enemy can fire a bullet yet
    switch (enemies[i].bullet)
    {
        case BULLET_NORMAL:
            if (enemies[i].time + ENEMY_BULLET_NORMAL_DELAY < frame_counter
                && check_axis_x (enemies[i].rect, player.rect))
            {
                enemies[i].time = frame_counter;
                enemy_bullet_add (enemies[i].rect.x + (enemies[i].rect.w /2), enemies[i].rect.y, BULLET_NORMAL);
            }
            break;
        case BULLET_HOMING:
            if (enemies[i].time + ENEMY_BULLET_HOMING_DELAY < frame_counter )
            {
                enemies[i].time = frame_counter;
                enemy_bullet_add (enemies[i].rect.x + (enemies[i].rect.w /2), enemies[i].rect.y, BULLET_HOMING);
            }
            break;
        case BULLET_NONE:
        default:
            break;
    }
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
}

static void enemy_bresenhem_homing (SDL_Rect *start, int xvel, int yvel, bullet_t type)
{
    int dx = abs(player.rect.x - start->x), sx = start->x < player.rect.x ? 1 : -1;
    int dy = abs(player.rect.y - start->y), sy = start->y < player.rect.y ? 1 : -1;
    int err = (dx>dy ? dx : -dy)/2;
    int e2;

    if (xvel == 0)
        xvel = 1;
    if (yvel == 0)
        yvel = 1;

    xvel *= player.speed;
    yvel *= player.speed;

    e2 = err;
    if (e2 > -dx)
    {
        err -= dy;
        start->x += sx * xvel;
    }

    if (type == BULLET_HOMING)
    {
        start->y += yvel;
    }
    else if (e2 < dy)
    {
        err += dx;
        start->y += sy * yvel;
    }
}

void enemy_bullet_update (void)
{
    int i;
    for (i = 0; i < MAX_ENEMIES; i++)
    {
        if (enemy_bullets[i].type != BULLET_NONE)
        {
            switch (enemy_bullets[i].type)
            {
                case BULLET_NORMAL:
                    enemy_bullets[i].rect.y += enemy_bullets[i].yvel;
                    break;
                case BULLET_HOMING:
                    enemy_bresenhem_homing (&enemy_bullets[i].rect, enemy_bullets[i].xvel, enemy_bullets[i].yvel, BULLET_HOMING);
                    break;
                case BULLET_NONE:
                default:
                    break;
            }
            if (enemy_bullets[i].rect.y >= screen_height)
                enemy_bullets[i].type = BULLET_NONE;
            else
                detect_enemy_bullet (enemy_bullets[i].rect, i);
        }
    }
}

static void enemy_update_ball (struct enemy *enemy)
{
    float gravity = 0.1;
    enemy->yvel += gravity;

    enemy->rect.x += enemy->xvel;
    enemy->rect.y += enemy->yvel;

    if (enemy->rect.x <= 0)
    {
        enemy->xvel = -enemy->xvel;
    }
    else if (enemy->rect.x + enemy->rect.w >= screen_width)
    {
        enemy->xvel = -enemy->xvel;
    }
    else if (enemy->rect.y + enemy->rect.h >= screen_height)
    {
        switch (enemy->size)
        {
            default:
            case 4:
                enemy->yvel = -10;
                break;
            case 3:
                enemy->yvel = -8;
                break;
            case 2:
                enemy->yvel = -6;
                break;
            case 1:
                enemy->yvel = -4;
                break;
        }
    }
    if (enemy->rect.y > screen_height)
        enemy->type = ENEMY_NONE;
    if (enemy->rect.x <= 0 && enemy->xvel < 0)
        enemy->xvel = -enemy->xvel;
}

void enemy_update (void)
{
    int i;
    if (level_change_timer)
        return;
    for (i = 0; i < MAX_ENEMIES; i++)
    {
        if (enemies[i].type != ENEMY_NONE)
        {
            detect_enemyhit (enemies[i].rect, i);
            enemy_bullet_check_to_fire (i);

            switch (enemies[i].type)
            {
                case ENEMY_CENTIPEDE:
                    enemy_update_centipede (i);
                    break;
                case ENEMY_HOMING:
                    enemy_bresenhem_homing (&enemies[i].rect, enemies[i].xvel, enemies[i].yvel, BULLET_NONE);
                    break;
                case ENEMY_BALL:
                    enemy_update_ball (&enemies[i]);
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

static void enemy_draw_ball (int num)
{
    SDL_RenderCopy (renderer, balloon_tex, NULL, &enemies[num].rect);
}

static void enemy_draw_bullets (void)
{
    int i;
    for (i = 0; i < MAX_ENEMIES; i++)
    {
        if (enemy_bullets[i].type != BULLET_NONE)
        {
            SDL_SetRenderDrawColor (renderer, 255,255,255,0);
            SDL_RenderFillRect (renderer, &enemy_bullets[i].rect);
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
            if (enemies[i].type == ENEMY_CENTIPEDE)
                enemy_draw_centipede (i);
            if (enemies[i].type == ENEMY_HOMING)
                enemy_draw_centipede (i);
            else if (enemies[i].type == ENEMY_BALL)
                enemy_draw_ball (i);
            //SDL_RenderDrawRect( renderer, &enemies[i].rect);
        }
    }
    enemy_draw_bullets ();
}
