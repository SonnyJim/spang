#include "spang.h"
#define COMBO_MAX 50
Mix_Chunk *speedup = NULL;

void combo_set_level (int level)
{
    if (player.combo_level > level && player.combo_time + 500 > SDL_GetTicks ())
    {
        return;
    }
    player.combo_time = SDL_GetTicks ();
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
            player.bullet_max = 2;
            player.bullet_size = 10;
            player.bullet_delay = 250;
            break;
        case 1:
            trippy_mode = 0;
            player.bullet_max = 4;
            player.bullet_delay = 200;
            player.bullet_size = 15;
            break;
        case 2:
            trippy_mode = 1;
            player.bullet_max = 5;
            player.bullet_delay = 150;
            player.bullet_size = 20;
            break;
        case 3:
            trippy_mode = 2;
            player.bullet_max = 10;
            player.bullet_delay = 100;
            player.bullet_size = 30;
            break;
        case 4:
            trippy_mode = 3;
            player.bullet_max = 20;
            player.bullet_delay = 50;
            player.bullet_size = 40;
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

    if (player.combo < COMBO_MAX)
        player.combo++;

    if (player.combo % 10 == 0)
        player_trippy_level_inc ();
    combo_check_level ();
}

void combo_decrement (void)
{
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
