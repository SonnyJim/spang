/* TODO:
High scores
Bonus wave
*/

#include "spang.h"
int paused;

int screen_height, screen_width;
SDL_Renderer *renderer;

int running;
int draw_hitbox = 0;


int main (int argc, char *argv[])
{
    if (sdl_init () != 0)
        return 1;

    if (textures_init ())
        return 1;
    if (audio_init ())
    {
        fprintf (stderr, "Error loading SFX: %s\n", SDL_GetError());
        return 1;
    }

    hiscore_init ();
    balls_init_all ();
    bullets_init ();
    player_init ();
    powerups_init ();
    explosions_init ();
    msg_init ();
    stars_init ();

    running = 1;
    while (running)
    {
        sdl_read_input ();
        if (!paused)
        {

        balls_check ();
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
        SDL_RenderClear (renderer);
        //SDL_RenderCopy (renderer, bg_tex, NULL, NULL);
        stars_draw ();
        player_draw ();
        balls_draw ();
        enemy_draw ();
        bullets_draw ();

        powerups_draw ();
        explosions_draw ();

        //detect_playerhit();
        render_score ();
        msg_draw ();
        level_change_pause ();
        }
        SDL_RenderPresent (renderer);
    }
    Mix_HaltMusic ();
    fprintf (stdout, "Score: %ld\n", player.score);
    fprintf (stdout, "Shots fired: %ld\n", player.shots_fired);
    fprintf (stdout, "Hits: %ld\n", player.hits);
    float hitratio = (float) player.shots_fired / (float) player.hits;
    fprintf (stdout, "Ratio: %.3f\n", hitratio);
    fprintf (stdout, "Balls destroyed: %ld\n", player.destroyed_balls);
    Mix_PlayChannel( -1, death, 0 );
    running = 1;
    hiscore_save ();
    while (running)
    {
        sdl_read_input();
        SDL_RenderPresent (renderer);
    }

    Mix_FreeChunk (laser1);
    Mix_FreeChunk (explosion);
    Mix_CloseAudio();
    SDL_Quit ();
    return 0;
}
