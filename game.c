#include "spang.h"
int gameover_timer = 0;
Uint32 playtime_start;

void game_pause (void)
{
    if (gamestate != GAME_RUNNING)
        return;
    Mix_Pause (-1);
    Mix_PauseMusic ();
    paused = 1;
}

void game_unpause (void)
{
    if (gamestate != GAME_RUNNING)
        return;
    Mix_Resume (-1);
    Mix_ResumeMusic ();
    paused = 0;
}

static void game_init (void)
{
    enemy_init ();
    gameover_timer = 0;
    balls_init_all ();
    bullets_init ();
    player_init ();
    powerups_init ();
    msg_init ();
    textures_load (0);
    explosions_init ();
//    stars_init ();
}

void game_start (void)
{
    bonus_level_active = 0;
    game_init ();
    gamestate = GAME_RUNNING;

    old_input_mask = 0;
    input_mask = 0;
    frame_counter = 0;
    if (record_state == REC_REC)
        record_start ();

    playtime_start = SDL_GetTicks ();
    Mix_PlayMusic (music[MUSIC_SPANGONE], -1);

    //Guarenteed to be random, Stern says so!
    random_seed = 69696969;
    level_up();
}

static void gameover_draw (void)
{
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
    SDL_RenderClear (renderer);
    render_score ();
    msg_draw ();
     if (hiscore_check () && !hsentry_playback)
    {
        //msg_show ("New Hiscore", screen_width / 2, screen_height / 2, 5, font2, ALIGN_CENTRE, red);
        render_string_centre ("New Hiscore", (screen_height / 2) - 100, red, font2);
    }
    else
        //msg_show ("Game Over", screen_width / 2, screen_height / 2, 5, font2, ALIGN_CENTRE, red);
        render_string_centre ("Game Over", (screen_height / 2) - 100, red, font2);


}

static void gameover_init (void)
{

    hiscore_position = NUM_HISCORES + 1;
    Mix_HaltMusic ();
    total_playtime += (SDL_GetTicks () - playtime_start) / 1000;

    fprintf (stdout, "Total playtime: %i\n", total_playtime);
    fprintf (stdout, "Score: %ld\n", player.score);
    fprintf (stdout, "Shots fired: %ld\n", player.shots_fired);
    fprintf (stdout, "Hits: %ld\n", player.hits);
    float hitratio = (float) player.hits/ (float) player.shots_fired;
    fprintf (stdout, "Ratio: %.3f\n", hitratio);
    fprintf (stdout, "Balls destroyed: %ld\n", player.destroyed_balls);
    Mix_PlayChannel( -1, death, 0 );
    hsentry_playback = 0;
    gameover_timer = 4 * 60;
}

void gameover_loop (void)
{
    if (record_state == REC_REC)
        record_stop ();
    else if (record_state == REC_PLAYING)
        playback_stop ();

    if (gameover_timer == 0)
        gameover_init ();
    else if (--gameover_timer == 1)
    {
        hiscore_position = hiscore_add ();
        if (hiscore_position != NUM_HISCORES + 1)
            gamestate = GAME_HSENTRY;
        else
            gamestate = GAME_AMODE;
    }
    gameover_draw();
}

void game_loop (void)
{
    balls_check ();
    stars_draw ();
    player_draw ();
    balls_draw ();
    enemy_draw ();
    bullets_draw ();
    powerups_draw ();
    explosions_draw ();
    render_score ();
    msg_draw ();
    level_change_pause ();
}
