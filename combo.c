#include "spang.h"
#define COMBO_MAX 50
Mix_Chunk *speedup = NULL;

static void combo_set_level (int level)
{
    if (level > player.combo_level)
        Mix_PlayChannel (SND_COMBO, comboup, 0);
    else if (level != player.combo_level)
        Mix_PlayChannel (SND_COMBO, combodown, 0);
    player.combo_level = level;
    switch (level)
    {
        case 0:
        default:
            player.bullet_max = 2;
            player.bullet_size = 10;
            player.bullet_delay = 250;
            break;
        case 1:
            player.bullet_max = 4;
            player.bullet_delay = 200;
            player.bullet_size = 15;
            break;
        case 2:
            player.bullet_max = 5;
            player.bullet_delay = 150;
            player.bullet_size = 20;
            break;
        case 3:
            player.bullet_max = 10;
            player.bullet_delay = 100;
            player.bullet_size = 30;
            break;
        case 4:
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
    if (player.hits % 31 == 0)
    {
        Mix_PlayChannel (SND_MUSIC, speedup, 0);
        player.speed += 0.5;
        balls_speed_up ();

    }
    if (player.combo < COMBO_MAX)
        player.combo++;
    combo_check_level ();
}

void combo_decrement (void)
{
    player.combo -= 2;
    if (player.combo < 0)
        player.combo = 1;
    combo_check_level ();
}
