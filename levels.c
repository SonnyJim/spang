#include "spang.h"
#define NUM_LEVELS 10
int current_texture = 0;
void level_one (void)
{
    ball_add (5, screen_width / 2, screen_height / 2, player.speed, 0, 1);
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
    textures_load(current_texture);
    ball_add (6, screen_width / 2, screen_height / 2, player.speed, 0, 5);
}
void level_ten (void)
{
    ball_add (3, (screen_width /2) - 100, screen_height / 2, player.speed, 0, 1);
    ball_add (3, (screen_width /2) + 100, screen_height / 2, player.speed, 0, 1);
    ball_add (3, (screen_width /2) - 100, (screen_height / 2) - 100, player.speed, 0, 1);
    ball_add (3, (screen_width /2) + 100, (screen_height / 2) - 100, player.speed, 0, 1);
}
void (*level_p[NUM_LEVELS]) () =
{
    level_one, level_two, level_three, level_four, level_five, level_five, level_five, level_eight,
    level_nine, level_ten
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
        msg_show ("50000", player.xpos + (player_rect.w / 2), player.ypos - 20, 3, font3, ALIGN_TCENTRE, white);

        player.score += 50000;
    }
    else if (hitratio >= 0.95)
    {
        msg_show ("AMAZING", screen_width /2, screen_height - 200, 3, font2, ALIGN_CENTRE, green);
        msg_show (buffer, screen_width /2, screen_height - 150, 3, font3, ALIGN_CENTRE, green);
        msg_show ("20000", player.xpos + (player_rect.w / 2), player.ypos - 20, 3, font3, ALIGN_TCENTRE, white);

        player.score += 20000;
    }
    else if (hitratio >= 0.90)
    {
        msg_show ("GOOD", screen_width /2, screen_height - 200, 3, font2, ALIGN_CENTRE, green);
        msg_show (buffer, screen_width /2, screen_height - 150, 3, font3, ALIGN_CENTRE, green);
        msg_show ("8000", player.xpos + (player_rect.w / 2), player.ypos - 20, 3, font3, ALIGN_TCENTRE, white);

        player.score += 8000;
    }
}

void level_up (void)
{
    Mix_PlayChannel (SND_MUSIC, levelup, 0);
    level_hit_ratio ();


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
