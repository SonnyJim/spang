#include "spang.h"

int current_texture = 0;

void level_one (void)
{
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
    for (i = 0; i < player.level; i ++)
        ball_add (5, screen_width / 2 + (rand () % 200) - 100, 200 + (rand() % 50), player.speed, rand () % 1, 1);
}

void level_eight (void)
{
    player.speed = 0.2;
    ball_add (8, screen_width / 2, screen_height / 2, player.speed, 0, 10);
}

void level_nine (void)
{
    if (current_texture < 2)
        current_texture++;
    else
        current_texture = 0;
    textures_load (current_texture);
    if (current_texture == 1)
        Mix_PlayMusic (music[MUSIC_BUBBLEBOBBLE], -1);
    else if (current_texture == 2)
        Mix_PlayMusic (music[MUSIC_PACMAN], -1);
    ball_add (7, screen_width / 2, screen_height / 2, player.speed, 0, 6);
}

void level_ten (void)
{
    ball_add (3, (screen_width /2) - 100, (screen_height / 2) + 100, player.speed, 0, 1);
    ball_add (3, (screen_width /2) + 100, (screen_height / 2) + 100, player.speed, 0, 1);
    ball_add (3, (screen_width /2) - 100, (screen_height / 2) - 100, player.speed, 0, 1);
    ball_add (3, (screen_width /2) + 100, (screen_height / 2) - 100, player.speed, 0, 1);

    ball_add (7, (screen_width /2), (screen_height/2), player.speed, 0, 1);
}

void level_eleven (void)
{
    int i, offset;
    offset = (screen_width /2) - (4 * 40);
    for (i = 1; i < 4; i++)
    {
        enemy_add (ENEMY_CENTIPEDE, BULLET_NORMAL, offset + (i * 40), 200, player.speed, 0, 1);
    }
}

void level_twelve (void)
{
    enemy_add (ENEMY_HOMING, BULLET_NORMAL, screen_width / 2, 0 , 3, 2, 2);
    enemy_add (ENEMY_HOMING, BULLET_HOMING, (screen_width / 2) - 100, 0 , 3, 1, 2);
    enemy_add (ENEMY_HOMING, BULLET_NORMAL, (screen_width / 2) + 100, 0 , 3, 2, 2);
}

