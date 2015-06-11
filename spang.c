/* TODO:
High scores
Bonus wave
*/

#include "spang.h"

int screen_height, screen_width;
SDL_Renderer *renderer;

SDL_Rect ball_rects[NUM_BALLS];
SDL_Rect bullet_rects[MAX_BULLETS];


SDL_Texture *bullet_tex = NULL;
SDL_Surface *ball_srf = NULL;
SDL_Texture *bg_tex = NULL;

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
    for (i = 0; i < NUM_BALLS; i++)
    {
        if (balls[i].size > 0)
        {
            if (check_axis (ball_rects[i], player_hitrect1) ||
                check_axis (ball_rects[i], player_hitrect2))
            {
                player_hit ();
                if (balls[i].size == 1)
                {
                    ball_split (i);
                }

            }
        }
    }
}

void detect_collision (int num)
{
    int i;
    for (i = 0; i < NUM_BALLS; i++)
    {
        if (balls[i].size > 0)
        {
            if (check_axis (bullet_rects[num], ball_rects[i]))
            {
                player_score (balls[i].size);
                bullet_remove (num);
                ball_split (i);
                return;
            }
        }
    }
}

int init_audio (void)
{
    laser1 = Mix_LoadWAV("data/sfx/laser1.wav");
    if (laser1 == NULL)
        return 1;
    explosion = Mix_LoadWAV ("data/sfx/explosion1.wav");
    if (explosion == NULL)
        return 1;
    playerhit = Mix_LoadWAV ("data/sfx/hit.wav");
    if (playerhit == NULL)
        return 1;
    death = Mix_LoadWAV ("data/sfx/death.wav");
    if (death == NULL)
        return 1;
    comboup = Mix_LoadWAV ("data/sfx/comboup.wav");
    if (comboup == NULL)
        return 1;
    combodown = Mix_LoadWAV ("data/sfx/combodown.wav");
    if (combodown == NULL)
        return 1;
    levelup = Mix_LoadWAV ("data/sfx/levelup.wav");
    if (levelup == NULL)
        return 1;
    speedup = Mix_LoadWAV ("data/sfx/speedup.wav");
    if (speedup == NULL)
        return 1;
    alarm = Mix_LoadWAV ("data/sfx/alarm1.wav");
    if (alarm == NULL)
        return 1;
    health1 = Mix_LoadWAV ("data/sfx/health.wav");
    if (health1 == NULL)
        return 1;
    kaching = Mix_LoadWAV ("data/sfx/kaching.wav");
    if (kaching == NULL)
        return 1;
    slowdown = Mix_LoadWAV ("data/sfx/slowdown.wav");
    if (slowdown == NULL)
        return 1;
    alarmreverse = Mix_LoadWAV ("data/sfx/alarmreverse.wav");
    if (alarmreverse == NULL)
        return 1;
    return 0;
}

void init_tex (void)
{
        ball_srf = IMG_Load ("data/gfx/asteroid1.png");
        if (ball_srf == NULL)
            fprintf (stderr, "Error loading ball_srf: %s\n", SDL_GetError ());

        ball_tex = SDL_CreateTextureFromSurface (renderer, ball_srf);
        if (ball_tex == NULL)
            fprintf (stderr, "Error creating texture\n");

        ball_srf = IMG_Load ("data/gfx/bullet1.png");
        if (ball_srf == NULL)
            fprintf (stderr, "Error loading bullet: %s\n", SDL_GetError ());

        bullet_tex = SDL_CreateTextureFromSurface (renderer, ball_srf);
        if (bullet_tex == NULL)
            fprintf (stderr, "Error creating texture\n");

        ball_srf = IMG_Load ("data/gfx/ship.png");
        if (ball_srf == NULL)
            fprintf (stderr, "Error loading ship: %s\n", SDL_GetError ());
        ship_tex = SDL_CreateTextureFromSurface (renderer, ball_srf);

        ball_srf = IMG_Load ("data/gfx/bg.png");
        if (ball_srf == NULL)
            fprintf (stderr, "Error loading bg: %s\n", SDL_GetError ());
        bg_tex = SDL_CreateTextureFromSurface (renderer, ball_srf);


        ball_srf = IMG_Load ("data/gfx/health.png");
        if (ball_srf == NULL)
            fprintf (stderr, "Error loading health: %s\n", SDL_GetError ());
        health_tex = SDL_CreateTextureFromSurface (renderer, ball_srf);
        if (health_tex == NULL)
            fprintf (stderr, "Error loading health: %s\n", SDL_GetError ());

        ball_srf = IMG_Load ("data/gfx/coin.png");
        if (ball_srf == NULL)
            fprintf (stderr, "Error loading coin: %s\n", SDL_GetError ());
        coin_tex = SDL_CreateTextureFromSurface (renderer, ball_srf);
        if (coin_tex == NULL)
            fprintf (stderr, "Error loading coin: %s\n", SDL_GetError ());

        ball_srf = IMG_Load ("data/gfx/slow.png");
        if (ball_srf == NULL)
            fprintf (stderr, "Error loading slow: %s\n", SDL_GetError ());
        slow_tex = SDL_CreateTextureFromSurface (renderer, ball_srf);
        if (coin_tex == NULL)
            fprintf (stderr, "Error loading slow: %s\n", SDL_GetError ());

        ball_srf = IMG_Load ("data/gfx/megashot.png");
        if (ball_srf == NULL)
            fprintf (stderr, "Error loading megashot: %s\n", SDL_GetError ());
        megashot_tex = SDL_CreateTextureFromSurface (renderer, ball_srf);
        if (coin_tex == NULL)
            fprintf (stderr, "Error loading megashot: %s\n", SDL_GetError ());

        SDL_FreeSurface (ball_srf);
}

int main (int argc, char *argv[])
{
    if (sdl_init () != 0)
        return 1;

    init_tex ();
    if (init_audio ())
    {
        fprintf (stderr, "Error loading SFX: %s\n", SDL_GetError());
    }

    balls_init_all ();
    bullets_init ();
    player_init ();
    powerups_init ();


    running = 1;
    while (running)
    {
        sdl_read_input ();
        balls_check ();
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
        SDL_RenderClear (renderer);
        SDL_RenderCopy (renderer, bg_tex, NULL, NULL);


        balls_draw ();
        bullets_draw ();
        player_draw ();
        powerups_draw ();

        detect_playerhit ();
        render_score ();

        SDL_RenderPresent (renderer);
    }
    Mix_HaltMusic ();
    fprintf (stdout, "Score: %ld\n", player.score);
    fprintf (stdout, "Shots fired: %ld\n", player.shots_fired);
    fprintf (stdout, "Hits: %ld\n", player.hits);
    Mix_PlayChannel( -1, death, 0 );
    SDL_Delay (3000);
    running = 1;
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
