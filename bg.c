#include "spang.h"
#include <math.h>

#define NUMBER_OF_STARS 1020
Uint16 centerx;
Uint16 centery;
int init = 0;
int rotate = 0;
float theta;

/*star struct*/
typedef struct
{
    SDL_Rect rect;
    float xpos, ypos;
    float zpos, speed;
    Uint8 color;
    Uint32 time;
} star_s;

static star_s stars[NUMBER_OF_STARS];

void init_star(star_s* star, int i)
{
    /* randomly init stars, generate them around the center of the screen */
    star->rect.w = (player.trippy_level) + 1;
    star->rect.h = star->rect.w;

    if (megashot_active)
    {
        star->xpos =  -40.0 + (80.0 * (rand()/(RAND_MAX+1.0)));
        star->ypos =  -40.0 + (80.0 * (rand()/(RAND_MAX+1.0)));

    }
    else
    {
        star->xpos =  -20.0 + (40.0 * (rand()/(RAND_MAX+1.0)));
        star->ypos =  -20.0 + (40.0 * (rand()/(RAND_MAX+1.0)));
    }
  star->xpos *= 3072.0; /*change viewpoint */
  star->ypos *= 3072.0;

  star->zpos =  i * ((player.trippy_level /2) + 0.1);
  //star->zpos = i;
  //star->speed =  ((player.trippy_level)) + (int)(2.0 * (rand()/(RAND_MAX+1.0)));
  star->speed = ((player.speed + 1) * (player.trippy_level +1)) / 3;
  star->color = i >> 2; /*the closer to the viewer the brighter*/
  star->time = 0;
  if (megashot_active)
  {
      star->rect.w = 4;
      star->rect.h = 4;
      star->time = 100;
  }
}

static void stars_init (void)
{
    int i;
    centerx = screen_width /2;
    centery = screen_height /2;

    for (i = 0; i < NUMBER_OF_STARS; i++)
    {
        init_star(stars + i, i + 1);
    }
}

void stars_toggle_rotation (void)
{
    if (rotate == 1)
        rotate = 0;
    else
        rotate = 1;
}

void stars_draw (void)
{
    int i, tempx, tempy;
    float rotate_speed, px, py;

    if (!init)
    {
        stars_init ();
        init = 1;
    }

    for (i = 0; i < NUMBER_OF_STARS; i++)
	{
	    stars[i].zpos -= stars[i].speed;

	    if (stars[i].zpos <= 0)
	    {
	        init_star(stars + i, i + 1);
	    }

        rotate_speed = player.speed / 300000;
        if (rotate == 1)
        {
            theta += rotate_speed;
            //if (theta > 360)
            //    theta = 0;
        }
        else if (rotate == 0)
        {
            theta -= rotate_speed;
        }

        if (gamestate == GAME_AMODE || GAME_HSENTRY)
        {
            theta += 0.000003;
        }

        px = (stars[i].xpos / stars[i].zpos) + centerx;
        py = (stars[i].ypos / stars[i].zpos) + centery;
        tempx = cos(theta) * (px-centerx) - sin(theta) * (py-centery) + centerx;
        tempy = sin(theta) * (px-centerx) + cos(theta) * (py-centery) + centery;

        if (stars[i].time < 100)
            stars[i].time++;
        else
            stars[i].speed += 0.1;

	    if (tempx < 0 || tempx > screen_width - 1 || tempy < 0 || tempy > screen_height - 1)
        {
            init_star(stars + i, i + 1);
            continue;
	    }

	    stars[i].rect.x = tempx;
	    stars[i].rect.y = tempy;
        SDL_SetRenderDrawColor( renderer, 0, stars[i].color, 0, 0 );
        SDL_RenderFillRect( renderer, &stars[i].rect);
	}
}
