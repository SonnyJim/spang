#include "spang.h"
#define ENDLEVEL_TIMER 2*60

int level_end_timer;
int level_change_timer;
int level_change_paused = 0;

void (*level_p[NUM_LEVELS]) () =
{
    level_one, level_two, level_three, level_four, level_five, level_five, level_five, level_eight,
    level_nine, level_ten, level_eleven, level_twelve, level_thirteen, level_fourteen, level_fiveteen, level_sixteen,
    level_seventeen, level_eighteen, level_nineteen, level_twenty, level_twentyone
};

static void level_hit_ratio (void)
{

    char buffer[64];
    float hitratio =  (float) player.hits_round / (float) player.shots_fired_round;
    int percent = hitratio * 100;

    if (player.hits_round == 0 || player.shots_fired_round == 0)
    {
        fprintf (stdout, "hits: %li, shots: %li\n", player.hits_round, player.shots_fired_round);
        fprintf (stderr, "No shots or hits??????\n");
        return;
    }
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
    if (bonus_level_active)
        return;
    level_end_timer = 20;
    stars_toggle_rotation ();
    Mix_PlayChannel (SND_MUSIC, levelup, 0);


    //Set the pause time before a level starts
    level_change_timer = 2 * 60;

    //Run the function to setup the level
    (*level_p[player.level]) ();

    if (player.level < NUM_LEVELS - 1)
        player.level++;
    else
        player.level = 0;


    if (player.level > 1)
    {
        fprintf (stdout, "Level %i: Stage time: %i\n", player.level, (frame_counter - player.stage_time) / 100);
    }
    if (player.level % 3 == 0)
        powerup_add (POWERUP_SLOW, randy (0, screen_width), 0);
    if (player.level % 5 == 0)
        powerup_add (POWERUP_MEGASHOT, randy (0, screen_width), 0);

    msg_show ("Supertapper recharge", 0, 130, 3, font1, ALIGN_CENTRE, green);
    player.smartbomb = 1;

    msg_level_up ();
    player.stage_time = frame_counter;
}

void level_end (void)
{
    if (gamestate != GAME_ENDLEVEL)
    {
        level_end_timer = ENDLEVEL_TIMER;
        gamestate = GAME_ENDLEVEL;
        if (player.level > 0)
            level_hit_ratio ();
        return;
    }

    level_endlevel_loop ();
}

void level_change_pause (void)
{
    if (bonus_level_active)
        bonus_draw ();
    if (level_change_timer)
        level_change_timer--;

}

static void level_endlevel_draw (void)
{
    int r,g,b;
    SDL_Rect box;

    SDL_SetRenderDrawColor (renderer, 255, 255, 255,0);
                //SDL_SetRenderDrawColor(renderer, rand() % 255, rand() % 255, rand () % 255, 0);
                //SDL_RenderDrawPoint (renderer, explosions[i][j].particle_rect.x, explosions[i][j].particle_rect.y);
    SDL_RenderFillRect (renderer, &box);
}
void level_endlevel_loop (void)
{
    if (level_end_timer-- != 0)
    {
        explosions_draw ();
        render_score ();
        msg_draw ();
    }
    else
    {
        fprintf (stdout, "Start next level\n");
        gamestate = GAME_RUNNING;

        if (player.level != 0 && player.level % 5 == 0 && !bonus_level_active)
        {
            bonus_level_start ();

        }
        else
            level_up ();
    }
}
