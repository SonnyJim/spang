#include "spang.h"

#define GRAVITY 0.5
#define NUM_PARTICLES 12
#define NUM_EXPLOSIONS 12
#define EXPLOSION_SIZE 5

struct particle_t
{
    SDL_Rect particle_rect;
    float xvel;
    float yvel;
    int active;
    SDL_Color colour;
};
int trippy_mode;

struct particle_t explosions[NUM_EXPLOSIONS][NUM_PARTICLES];

void explosions_init (void)
{
    int i, j;
    trippy_mode = 0;
    for (i = 0; i < NUM_EXPLOSIONS; i++)
    {
        for (j = 0; j < NUM_PARTICLES; j++)
        {
            explosions[i][j].particle_rect.x = 0;
            explosions[i][j].particle_rect.y = 0;
            explosions[i][j].particle_rect.w = EXPLOSION_SIZE;
            explosions[i][j].particle_rect.h = EXPLOSION_SIZE;
            explosions[i][j].xvel = 0;
            explosions[i][j].yvel = 0;
            explosions[i][j].active = 0;
        }
    }
}

void explosion_superbomb (int xpos, int ypos)
{
    int i, j;
    for (i = 0; i < 5; i++)
    {
        for (j = 0; j < 5; j++)
        {
            if (explosions[i][j].active == 0)
            {
                explosions[i][j].particle_rect.x = xpos;
                explosions[i][j].particle_rect.y = ypos;

                explosions[i][j].xvel = (rand() %40) - 20;
                explosions[i][j].yvel = (rand() % 10) - 10;
                explosions[i][j].colour.r = (rand () % 255);
                explosions[i][j].colour.g = (rand () % 255);
                explosions[i][j].colour.b = (rand () % 255);

                explosions[i][j].active = 1;

                explosions[i][j].particle_rect.w = (rand () % (40)) + 20;
                //explosions[i][j].colour = { rand() % 255, rand() % 255, rand() % 255};
                //return;
            }
        }
    }
}

void explosion_add (int xpos, int ypos)
{
    int i, j;
    for (i = 0; i < NUM_EXPLOSIONS; i++)
    {
        for (j = 0; j < NUM_PARTICLES; j++)
        {
            if (explosions[i][j].active == 0)
            {
                explosions[i][j].particle_rect.x = xpos;
                explosions[i][j].particle_rect.y = ypos;
                if (trippy_mode)
                {
                    explosions[i][j].xvel = rand() % (20 * trippy_mode) - (10 * trippy_mode);
                    explosions[i][j].yvel = rand() % (25 * trippy_mode) - (10 * trippy_mode);
                    explosions[i][j].colour.r = 128 * trippy_mode;
                    explosions[i][j].colour.g = 128 * trippy_mode;
                    explosions[i][j].colour.b = 128 * trippy_mode;
                }
                else
                {
                    explosions[i][j].colour.r = 255;
                    explosions[i][j].colour.g = 10;
                    explosions[i][j].colour.b = 10;
                    explosions[i][j].xvel = rand() % 20 - 10;
                    explosions[i][j].yvel = rand() % 20 - 10;
                }
                explosions[i][j].active = 1;

                if (trippy_mode)
                    explosions[i][j].particle_rect.w = rand () % (5 * trippy_mode);
                else
                    explosions[i][j].particle_rect.w = EXPLOSION_SIZE;
                //explosions[i][j].colour = { rand() % 255, rand() % 255, rand() % 255};
                //return;
            }
        }
    }
}

static void explosion_update (int i, int j)
{
    explosions[i][j].yvel += GRAVITY;
    explosions[i][j].particle_rect.x += explosions[i][j].xvel;
    explosions[i][j].particle_rect.y += explosions[i][j].yvel;
    explosions[i][j].colour.r += rand () % 10 * trippy_mode;
    explosions[i][j].colour.g -= rand () % 10 * trippy_mode;
    explosions[i][j].colour.b -= rand () % 10 * trippy_mode;
    if (trippy_mode == 0)
    {
        if (explosions[i][j].particle_rect.w > 1)
            explosions[i][j].particle_rect.w -= 1;
    }
    else
        if (explosions[i][j].particle_rect.w < 3 * trippy_mode)
            explosions[i][j].particle_rect.w += 1;
    explosions[i][j].particle_rect.h = explosions[i][j].particle_rect.w;
    if (explosions[i][j].particle_rect.x <= 0 || explosions[i][j].particle_rect.x >= screen_width
        || explosions[i][j].particle_rect.y >= screen_height)
        explosions[i][j].active = 0;
}

static void explosions_update_all (void)
{
    int i, j;
    for (i = 0; i < NUM_EXPLOSIONS; i++)
    {
        for (j = 0; j < NUM_PARTICLES; j++)
        {
            if (explosions[i][j].active == 1)
            {
                explosion_update (i, j);
            }
        }
    }
}

void explosions_draw (void)
{
    int i, j;
    //SDL_SetRenderDrawColor(renderer, 255, 255, 255, 0);
    explosions_update_all ();
    for (i = 0; i < NUM_EXPLOSIONS; i++)
    {
        for (j = 0; j < NUM_PARTICLES; j++)
        {
            if (explosions[i][j].active == 1)
            {
                SDL_SetRenderDrawColor (renderer, explosions[i][j].colour.r, explosions[i][j].colour.g, explosions[i][j].colour.b, 0);
                //SDL_SetRenderDrawColor(renderer, rand() % 255, rand() % 255, rand () % 255, 0);
                //SDL_RenderDrawPoint (renderer, explosions[i][j].particle_rect.x, explosions[i][j].particle_rect.y);
                SDL_RenderFillRect (renderer, &explosions[i][j].particle_rect);
            }
        }
    }
}
