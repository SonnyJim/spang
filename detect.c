#include "spang.h"

int check_axis_x (SDL_Rect a, SDL_Rect b)
{
    int leftA, leftB;
    int rightA, rightB;

    leftA = a.x;
    rightA = a.x + a.w;

    leftB = b.x;
    rightB = b.x + b.w;
    if ( rightA <= leftB || leftA >= rightB )
        return 0;
    else
        return 1;
}

int check_axis (SDL_Rect a, SDL_Rect b)
{
     int leftA, leftB;
     int rightA, rightB;
     int topA, topB;
     int bottomA, bottomB;

     leftA = a.x;
     rightA = a.x + a.w;
     topA = a.y;
     bottomA = a.y + a.h;

     leftB = b.x;
     rightB = b.x + b.w;
     topB = b.y;
     bottomB = b.y + b.h;

     if( bottomA <= topB || topA >= bottomB || rightA <= leftB ||leftA >= rightB )
        return 0;
     else
        return 1;
}

static void player_was_hit_by_ball (int i)
{
    if (balls[i].size == 1)
        ball_hit (i);
}

/*
static void player_was_hit_by_bullet (int i)
{
    enemy_bullet_remove (i);
}
*/


static void detect_playerhit (void (*function)(int), SDL_Rect rect, int object_num)
{
    if (player.invuln_time)
        return;
    if (check_axis (rect, player_hitrect1) || check_axis (rect, player_hitrect2))
    {
        explosion_add (player.rect.x + (player.rect.w / 2), player.rect.y + (player.rect.h / 2) );
        player_hit ();
        function (object_num);
    }
}

void detect_enemyhit (SDL_Rect rect, int enemy)
{
    detect_playerhit (enemy_hit, rect, enemy);
}

//Check if something hit the player
void detect_ballhit (SDL_Rect rect, int ball)
{
    detect_playerhit (player_was_hit_by_ball, rect, ball);
}

void detect_enemy_bullet (SDL_Rect rect, int bullet)
{
    detect_playerhit (enemy_bullet_remove, rect, bullet);
}

//Detect if a bullet has hit a rock/enemy/barrel
void detect_bullet (int num)
{
    int i;

    if (bonus_level_active)
    {
        for (i = 0; i < BONUS_BARRELS; i++)
        {
            if (barrels[i].active && check_axis (bullets[num].rect, barrels[i].rect))
            {
                bullet_remove (num);
                bonus_barrel_hit (&barrels[i], &bullets[num]);
                break;
            }
        }
        return;
    }

    for (i = 0; i < MAX_BALLS; i++)
    {
        if (balls[i].size > 0)
        {
            if (check_axis (bullets[num].rect, balls[i].rect))
            {
                bullet_remove (num);
                ball_hit (i);
                break;
            }
        }
    }

    for (i = 0; i < MAX_ENEMIES; i++)
    {
        if (enemies[i].type != ENEMY_NONE)
        {
            if (check_axis (bullets[num].rect, enemies[i].rect))
            {
                bullet_remove (num);
                enemy_hit (i);
                break;
            }
        }
    }
}
