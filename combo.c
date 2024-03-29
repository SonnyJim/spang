#include "spang.h"
#define COMBO_MAX 50
Mix_Chunk *speedup = NULL;

void combo_set_level (int level)
{
    if (!bonus_level_active && player.combo_level > level && player.combo_time + 50 > frame_counter)
    {
        return;
    }
    player.combo_time = frame_counter;
    if (level > player.combo_level)
        Mix_PlayChannel (SND_COMBO, comboup, 0);
    else if (level != player.combo_level)
        Mix_PlayChannel (SND_COMBO, combodown, 0);
    player.combo_level = level;

    switch (level)
    {
        case 0:
        default:
            trippy_mode = 0;
            player.bullet_max = 4;
            player.bullet_size = 20;
            player.bullet_delay = 12;
            break;
        case 1:
            trippy_mode = 0;
            player.bullet_max = 8;
            player.bullet_delay = 10;
            player.bullet_size = 30;
            break;
        case 2:
            trippy_mode = 1;
            player.bullet_max = 16;
            player.bullet_delay = 7;
            player.bullet_size = 40;
            break;
        case 3:
            trippy_mode = 2;
            player.bullet_max = 20;
            player.bullet_delay = 5;
            player.bullet_size = 50;
            break;
        case 4:
            trippy_mode = 3;
            player.bullet_max = 32;
            player.bullet_delay = 3;
            player.bullet_size = 60;
            break;
    }
}

void combo_check_level (void)
{
    if (player.combo >= 50)
        combo_set_level (4);
    else if (player.combo > 20)
        combo_set_level (3);
    else if (player.combo > 10)
        combo_set_level (2);
    else if (player.combo > 3)
        combo_set_level (1);
    else
        combo_set_level (0);
}

void combo_increment (void)
{

    player.hits++;
    player.hits_round++;
//    fprintf (stdout, "Hit++ %li\n", player.hits_round);

    if (player.combo < COMBO_MAX)
        player.combo++;

    if (player.combo % 10 == 0)
        player_trippy_level_inc ();
    combo_check_level ();
}

void combo_decrement (void)
{
    if (bonus_level_active)
        return;
    score_bonus = 10;
    player.combo -= 2;
    if (player.combo < 0)
        player.combo = 1;
    combo_check_level ();
}

void combo_hit (int size)
{
    if (player.last_size == size)
    {
        score_bonus += score_bonus;
        if (score_bonus > 800)
            score_bonus = 800;
    }
    else
        score_bonus = 10;
    player.last_size = size;
}
