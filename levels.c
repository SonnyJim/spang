#include "spang.h"

int current_texture = 0;

void level_one (void)
{
    player.laps++;
    ball_add (5, screen_width / 2, 200, player.speed, 1, 1);
}

void level_two (void)
{
    ball_add (5, screen_width / 2 - 100, screen_height / 2, player.speed, 1, 1);
    ball_add (5, screen_width / 2 + 100, screen_height / 2, player.speed, 0, 1);
}

void level_three (void)
{
    ball_add (6, screen_width / 2, screen_height / 2, player.speed, 0, 5);
}

/*
extern void throw_life_away (void);
void level_four (void)
{
    throw_life_away ();
}
*/
void level_four (void)
{
    int i;
    for (i = 0; i < 16; i++)
    {
        ball_add (1, (screen_width / 2) - ( 10 * 16) + (i * 16), screen_height /2, player.speed, 0, 1);
        ball_add (1, (screen_width / 2) + ( 10 * 16) - (i * 16), screen_height /2, player.speed, 1, 1);
    }

}
void level_five (void)
{
    int i;
    for (i = 0; i < player.level - 1; i ++)
    {
        ball_add (4 + player.laps, ((screen_width / 2) + (i * 15)) - (i * 30), screen_height / 2, player.speed, 0, 1);
    }
}

void level_eight (void)
{
    player.speed = 0.2;
    ball_add (8, screen_width / 2, screen_height / 2, player.speed, 0, 10);
}

void level_nine (void)
{
    ball_add (7, screen_width / 2, screen_height / 2, player.speed, 0, 6 + player.laps);
}

void level_ten (void)
{
    ball_add (3, (screen_width /2) - 100, (screen_height / 2) + 100, player.speed, 0, 1);
    ball_add (3, (screen_width /2) + 100, (screen_height / 2) + 100, player.speed, 0, 1);
    ball_add (3, (screen_width /2) - 100, (screen_height / 2) - 100, player.speed, 0, 1);
    ball_add (3, (screen_width /2) + 100, (screen_height / 2) - 100, player.speed, 0, 1);

    ball_add (7, (screen_width /2), (screen_height/2), player.speed, 0, 1);
    Mix_PlayMusic (music[MUSIC_SPANGTWO], -1);
}

void level_eleven (void)
{
    enemy_add (ENEMY_BALL, BULLET_NONE, (screen_width / 2) - ((BALL_WIDTH * 4)/2), 140, 3, 0, 1, 4 + player.laps);
}

void level_twelve (void)
{
    enemy_add (ENEMY_BALL, BULLET_NONE, 20, 100, 3, 0, 1, 4 + player.laps);
    enemy_add (ENEMY_BALL, BULLET_NONE, screen_width, 100, -3, 0, 1, 4 + player.laps);
}

void level_thirteen (void)
{
    enemy_add (ENEMY_BALL, BULLET_NONE, (screen_width / 2) - ((BALL_WIDTH * 7 / 2)), 150, 3, 0, 1, 7);
}

void level_fourteen (void)
{
    ball_add (5, screen_width / 2, 200, player.speed, 1, 1);
    enemy_add (ENEMY_BALL, BULLET_NONE, 20, 100, 3, 0, 1, 4);
    enemy_add (ENEMY_BALL, BULLET_NONE, screen_width, 100, 3, 0, 1, 4);
}

void level_fiveteen (void)
{
    int i;
    for (i = 0; i < 16; i++)
    {
        enemy_add (ENEMY_BALL, BULLET_NONE, (screen_width / 2) - ( 10 * 16) + (i * 16), 120, 3, 0, 1, 1);
    }
}

void level_sixteen (void)
{
    int i, offset;
    offset = (screen_width /2) - (10 * 20);
    for (i = 1; i < 10; i++)
    {
        enemy_add (ENEMY_CENTIPEDE, BULLET_NORMAL, offset + (i * 40), 200, 1, 0, 1,1);
    }
}

void level_seventeen (void)
{
    int i, offset;
    offset = (screen_width /2) - (10 * 20);
    for (i = 1; i < 10; i++)
    {
        enemy_add (ENEMY_CENTIPEDE, BULLET_NORMAL, offset + (i * 40), 110, player.speed, 0, 1,1);
        enemy_add (ENEMY_CENTIPEDE, BULLET_NORMAL, offset + (i * 40), 150, player.speed, 0, 1,1);
        enemy_add (ENEMY_CENTIPEDE, BULLET_NORMAL, offset + (i * 40), 190, player.speed, 0, 1,1);
        enemy_add (ENEMY_CENTIPEDE, BULLET_NORMAL, offset + (i * 40), 230, player.speed, 0, 1,1);
    }
}

void level_eighteen (void)
{
    int i, offset;
    offset = (screen_width /2) - (10 * 20);
    for (i = 1; i < 10; i++)
    {
        enemy_add (ENEMY_CENTIPEDE, BULLET_NORMAL, offset + (i * 40), 200, 1, 0, 1,1);
    }

    enemy_add (ENEMY_HOMING, BULLET_HOMING, screen_width / 2, 70 , 3, 2, 1, 1);
    enemy_add (ENEMY_HOMING, BULLET_NORMAL, (screen_width / 2) - 400, 70 , 3, 1, 1, 1);
    enemy_add (ENEMY_HOMING, BULLET_NORMAL, (screen_width / 2) + 400, 70, 3, 2, 1, 1);
}

void level_nineteen (void)
{
    enemy_add (ENEMY_HOMING, BULLET_NONE, 20, 70, 3, 1, 5, 3);
    enemy_add (ENEMY_HOMING, BULLET_NONE, screen_width - 20 - ((CENTIPEDE_WIDTH) / 2 * 3), 70, 3, 1, 5, 3);
    enemy_add (ENEMY_HOMING, BULLET_NONE, (screen_width / 2) - ((CENTIPEDE_WIDTH / 2) * 6), 70 , 3, 1, 20, 6);
}

void level_twenty (void)
{
    int i, offset;
    offset = (screen_width /2) - (20 * 20);
    for (i = 1; i < 20; i++)
    {
        enemy_add (ENEMY_CENTIPEDE, BULLET_NORMAL, offset + (i * 40), 110, player.speed, 0, 1,1);
        enemy_add (ENEMY_CENTIPEDE, BULLET_NORMAL, offset + (i * 40), 150, player.speed, 0, 1,1);
        enemy_add (ENEMY_CENTIPEDE, BULLET_NORMAL, offset + (i * 40), 190, player.speed, 0, 1,1);
        enemy_add (ENEMY_CENTIPEDE, BULLET_NORMAL, offset + (i * 40), 230, player.speed, 0, 1,1);
        enemy_add (ENEMY_CENTIPEDE, BULLET_NORMAL, offset + (i * 40), 270, player.speed, 0, 1,1);
        enemy_add (ENEMY_CENTIPEDE, BULLET_NORMAL, offset + (i * 40), 310, player.speed, 0, 1,1);
    }
}

void level_twentyone (void)
{
    if (player.laps <= 2)
        textures_load (player.laps);
    if (player.laps == 1)
        Mix_PlayMusic (music[MUSIC_BUBBLEBOBBLE], -1);
    else
        Mix_PlayMusic (music[MUSIC_T2K], -1);

    enemy_add (ENEMY_BALL, BULLET_NONE, (screen_width / 2) - ((BALL_WIDTH * 10 / 2)), 150, 3, 0, 1, 8);
}
