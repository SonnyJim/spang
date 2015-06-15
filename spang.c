/* TODO:
High scores
*/

#include "spang.h"
int paused;
int gamestate = GAME_AMODE;

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
    if (music_init ())
    {
        fprintf (stderr, "Error loading SFX: %s\n", SDL_GetError());
        return 1;
    }

    hiscore_init ();
    gamestate = GAME_AMODE;
    running = 1;

    while (running)
    {
        sdl_read_input ();
        if (!paused)
        {
            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
            if (gamestate != GAME_OVER)
                SDL_RenderClear (renderer);
            switch (gamestate)
            {
                case GAME_RUNNING:
                    game_loop ();
                    break;
                case GAME_AMODE:
                default:
                    amode_loop ();
                    break;
                case GAME_OVER:
                    gameover_loop ();
                    break;
                case GAME_HSENTRY:
                    hsentry_loop ();
                    break;
            }
        }

        SDL_RenderPresent (renderer);
    }

    hiscore_save ();
    while (running)
    {
        sdl_read_input();
        msg_draw ();
        SDL_RenderPresent (renderer);
    }

    Mix_FreeChunk (laser1);
    Mix_FreeChunk (explosion);
    Mix_CloseAudio();
    SDL_Quit ();
    return 0;
}
