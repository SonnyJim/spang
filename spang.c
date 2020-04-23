/* TODO:
Sort out ball gravity based on player speed
Finish centipede enemy
Build more levels
Close/free files/devices on quit
*/

#include "spang.h"
int paused;
int gamestate = GAME_AMODE;

int screen_height, screen_width;
SDL_Renderer *renderer;

int running;
int draw_hitbox = 0;

void draw_test (void);

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

	fprintf (stdout, "Loading config\n");
    if (config_load ())
    {
        fprintf (stderr, "Error loading config\n");
    }

    hiscore_init ();
    running = 1;
    while (running)
    {
        sdl_read_input ();
        if (!paused)
        {
            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
	//if (gamestate != GAME_OVER)
                SDL_RenderClear (renderer);

            switch (gamestate)
            {
                case GAME_DEMO:
                    draw_test ();
                    break;
                case GAME_RUNNING:
                case GAME_ENDLEVEL:
                case GAME_BONUS:
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
                case GAME_CONFIG:
                case GAME_CONFIG_INPUT:
                    config_loop ();
                    break;
                case GAME_SELECT_RECORD:
                    playback_loop ();
                    break;
            }
        }
        else
        {
            render_score ();
        }

        SDL_RenderPresent (renderer);
    }

    hiscore_save ();
    sdl_close ();
    return 0;
}
