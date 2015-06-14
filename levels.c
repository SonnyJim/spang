#include "spang.h"
#define NUM_LEVELS 11
int current_texture = 0;
int level_change_timer;
int level_change_paused = 0;

void level_one (void)
{
    int i, offset;
    offset = (screen_width /2) - (10 * 40);
    for (i = 1; i < 10; i++)
    {
        enemy_add (ENEMY_CENTIPEDE, offset + (i * 40), 200, player.speed * 3, 0, 5);
    }
   // enemies[ret].data = 6;
    //enemy_add (ENEMY_CENTIPEDE, screen_width /2, screen_height/2, -player.speed * 2, 0);
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

}

void (*level_p[NUM_LEVELS]) () =
{
    level_one, level_two, level_three, level_four, level_five, level_five, level_five, level_eight,
    level_nine, level_ten, level_eleven
};

static void level_hit_ratio (void)
{
    char buffer[64];
    float hitratio =  (float) player.hits_round / (float) player.shots_fired_round;
    int percent = hitratio * 100;
    player.shots_fired_round = 0;
    player.hits_round = 0;


    fprintf (stdout, "hitratio: %f\n", hitratio);
    sprintf (buffer, "Hit ratio %i", percent);
    if (hitratio > 1)
    {
        fprintf (stderr, "Hit ratio over 1\n");
        return;
    }
    if (hitratio == 1)
    {
        msg_show ("PERFECT", screen_width /2, screen_height - 200, 3, font2, ALIGN_CENTRE, red);
        msg_show (buffer, screen_width /2, screen_height - 150, 3, font3, ALIGN_CENTRE, green);
        msg_show ("50000", player.rect.x + (player.rect.w / 2), player.rect.y - 20, 3, font3, ALIGN_TCENTRE, white);

        player.score += 50000;
    }
    else if (hitratio >= 0.95)
    {
        msg_show ("AMAZING", screen_width /2, screen_height - 200, 3, font2, ALIGN_CENTRE, green);
        msg_show (buffer, screen_width /2, screen_height - 150, 3, font3, ALIGN_CENTRE, green);
        msg_show ("20000", player.rect.x + (player.rect.w / 2), player.rect.y - 20, 3, font3, ALIGN_TCENTRE, white);

        player.score += 20000;
    }
    else if (hitratio >= 0.90)
    {
        msg_show ("GOOD", screen_width /2, screen_height - 200, 3, font2, ALIGN_CENTRE, green);
        msg_show (buffer, screen_width /2, screen_height - 150, 3, font3, ALIGN_CENTRE, green);
        msg_show ("8000", player.rect.x + (player.rect.w / 2), player.rect.y - 20, 3, font3, ALIGN_TCENTRE, white);

        player.score += 8000;
    }
}

void level_up (void)
{
    Mix_PlayChannel (SND_MUSIC, levelup, 0);
    level_hit_ratio ();

    level_change_timer = 2 * 60;
    //player.level = 10;
    (*level_p[player.level]) ();
    if (player.level < NUM_LEVELS - 1)
        player.level++;
    else
        player.level = 0;
    /*
    for (i = 0; i < player.level; i ++)
        ball_add (5, screen_width / 2 + (rand () % 200) - 100, 200 + (rand() % 50), player.speed, rand () % 1, 1);
    */
    if (player.level > 1)
    {
        fprintf (stdout, "Level %i: Stage time: %i\n", player.level, (SDL_GetTicks() - player.stage_time) / 100);
    }
    if (player.level % 3 == 0)
        powerup_add (POWERUP_SLOW, rand () % screen_width, 0);
    if (player.level % 5 == 0)
        powerup_add (POWERUP_MEGASHOT, rand () % screen_width, 0);
    if (player.level % 4 == 0)
    {
        msg_show ("Supertapper recharge", 0, (screen_height / 2) - 100, 3, font1, ALIGN_CENTRE, green);
        player.smartbomb = 1;
    }

    msg_level_up ();
    player.stage_time = SDL_GetTicks ();
}

void level_change_pause (void)
{
    if (level_change_timer)
        level_change_timer--;
}
