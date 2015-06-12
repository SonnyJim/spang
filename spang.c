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

int check_axis (SDL_Rect a, SDL_Rect b)
{
     int leftA, leftB;
     int rightA, rightB;
     int topA, topB;
     int bottomA, bottomB;

     leftA = a.x;
     rightA = a.x + a.w;
     topA = a.y;
     bottomA = a.y + a.h;

     leftB = b.x;
     rightB = b.x + b.w;
     topB = b.y;
     bottomB = b.y + b.h;

     if( bottomA <= topB || topA >= bottomB || rightA <= leftB ||leftA >= rightB )
        return 0;
     else
        return 1;
}

void detect_playerhit (void)
{
    int i;
    for (i = 0; i < MAX_BALLS; i++)
    {
        if (balls[i].size > 0)
        {
            if (check_axis (balls[i].rect, player_hitrect1) ||
                check_axis (balls[i].rect, player_hitrect2))
            {
                if (player.invuln_time)
                    return;

                explosion_add (player.xpos + (player_rect.w / 2), player.ypos + (player_rect.h / 2) );
                //paused = 1;
                player_hit ();
                if (balls[i].size == 1)
                {
                    ball_hit (i);
                }

            }
        }
    }
}

void detect_collision (int num)
{
    int i;
    for (i = 0; i < MAX_BALLS; i++)
    {
        if (balls[i].size > 0)
        {
            if (check_axis (bullet_rects[num], balls[i].rect))
            {
                bullet_remove (num);
                ball_hit (i);
                return;
            }
        }
    }
}


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

    running = 1;
    while (running)
    {
        sdl_read_input ();
        if (!paused)
        {

        balls_check ();
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
        SDL_RenderClear (renderer);
        SDL_RenderCopy (renderer, bg_tex, NULL, NULL);

        player_draw ();
        explosions_draw ();
        balls_draw ();
        bullets_draw ();

        powerups_draw ();

        detect_playerhit ();
        render_score ();
        msg_draw ();
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
