#include "spang.h"

#define GRAVITY 0.5
#define NUM_PARTICLES 12
#define NUM_EXPLOSIONS 12
#define EXPLOSION_SIZE 5

static SDL_Color colors[256];
int explosion_type = 0;
int trippy_mode;

struct particle_t
{
    SDL_Rect particle_rect;
    float xvel;
    float yvel;
    int active;
    SDL_Color colour;
    int colourindex;
};

struct particle_t explosions[NUM_EXPLOSIONS][NUM_PARTICLES];

static void explosions_init_palette (void)
{
    int i;

    for (i = 0; i < 32; ++i)
    {
      /* black to blue, 32 values*/
      colors[i].b = i << 1;

      /* blue to red, 32 values*/
      colors[i + 32].r = i << 3;
      colors[i + 32].b =  64 - (i << 1);

      /*red to yellow, 32 values*/
      colors[i + 64].r = 255;
      colors[i + 64].g = i << 3;

      /* yellow to white, 162 */
      colors[i + 96].r = 255;
      colors[i + 96].g = 255;
      colors[i + 96].b = i << 2;
      colors[i + 128].r = 255;
      colors[i + 128].g = 255;
      colors[i + 128].b = 64 + (i << 2);
      colors[i + 160].r = 255;
      colors[i + 160].g = 255;
      colors[i + 160].b = 128 + (i << 2);
      colors[i + 192].r = 255;
      colors[i + 192].g = 255;
      colors[i + 192].b = 192 + i;
      colors[i + 224].r = 255;
      colors[i + 224].g = 255;
      colors[i + 224].b = 224 + i;
    }
}

void explosions_init (void)
{
    int i, j;
    trippy_mode = 0;
    explosions_init_palette ();
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
    for (i = 0; i < NUM_EXPLOSIONS; i++)
    {
        for (j = 0; j < 4; j++)
        {
            if (explosions[i][j].active == 0)
            {
                explosions[i][j].particle_rect.x = xpos; //- 30 + (rand () % 60);
                explosions[i][j].particle_rect.y = ypos;

                explosions[i][j].xvel = (rand() %40) - 20;
                explosions[i][j].yvel = (rand() % 10) - 20;

                if (!explosion_type)
                {

                    explosions[i][j].colour.r = colors[explosions[i][j].colourindex].r;
                    explosions[i][j].colour.g = colors[explosions[i][j].colourindex].g;
                    explosions[i][j].colour.b = colors[explosions[i][j].colourindex].b;
                }
                else
                {
                    explosions[i][j].colour.r = (rand () % 255);
                    explosions[i][j].colour.g = (rand () % 255);
                    explosions[i][j].colour.b = (rand () % 255);
                }
                explosions[i][j].active = 1;

                explosions[i][j].particle_rect.w = (rand () % (40)) + 30;

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
                    if (explosion_type)
                    {
                        explosions[i][j].colour.r = 128 * trippy_mode;
                        explosions[i][j].colour.g = 128 * trippy_mode;
                        explosions[i][j].colour.b = 128 * trippy_mode;
                    }
                }
                else
                {
                    if (explosion_type)
                    {
                        explosions[i][j].colour.r = 255;
                        explosions[i][j].colour.g = 10;
                        explosions[i][j].colour.b = 10;
                    }
                    explosions[i][j].xvel = rand() % 20 - 10;
                    explosions[i][j].yvel = rand() % 20 - 10;
                }
                explosions[i][j].colourindex = (rand() %20) + 90;
                if (!explosion_type)
                {

                    explosions[i][j].colour.r = colors[explosions[i][j].colourindex].r;
                    explosions[i][j].colour.g = colors[explosions[i][j].colourindex].g;
                    explosions[i][j].colour.b = colors[explosions[i][j].colourindex].b;
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
    if (explosion_type)
    {
    explosions[i][j].colour.r += rand () % 10 * trippy_mode;
    explosions[i][j].colour.g -= rand () % 10 * trippy_mode;
    explosions[i][j].colour.b -= rand () % 10 * trippy_mode;
    }
    else
    {
    if (explosions[i][j].colourindex > 10)
        explosions[i][j].colourindex--;
    explosions[i][j].colour.r = colors[explosions[i][j].colourindex].r;
    explosions[i][j].colour.g = colors[explosions[i][j].colourindex].g;
    explosions[i][j].colour.b = colors[explosions[i][j].colourindex].b;
    }
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
        || explosions[i][j].particle_rect.y >= screen_height
        || explosions[i][j].colourindex <= 10)
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
