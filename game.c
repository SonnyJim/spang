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
    player_init ();
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
    Mix_PlayMusic (music[MUSIC_CYBERRID], -1);
    playtime_start = SDL_GetTicks ();
}

static void gameover_init (void)
{

    hiscore_position = NUM_HISCORES + 1;
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
    SDL_RenderClear (renderer);
    stars_draw ();
    player_draw ();
    balls_draw ();
    enemy_draw ();
    bullets_draw ();
    powerups_draw ();
    //explosions_draw ();

    render_score ();
    //msg_draw ();
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
    if (hiscore_check ())
    {
        //msg_show ("New Hiscore", screen_width / 2, screen_height / 2, 5, font2, ALIGN_CENTRE, red);
        render_string_centre ("New Hiscore", (screen_height / 2) - 100, red, font2);
        hiscore_position = hiscore_add ();
    }
    else
        render_string_centre ("Game Over", (screen_height / 2) - 100, red, font2);

    gameover_timer = 4 * 60;
    //Call game_loop once to draw everything

}

void gameover_loop (void)
{
    if (gameover_timer == 0)
        gameover_init ();
    else if (--gameover_timer == 1)
    {
        if (hiscore_position != NUM_HISCORES + 1)
            gamestate = GAME_HSENTRY;
        else
            gamestate = GAME_AMODE;
    }
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
