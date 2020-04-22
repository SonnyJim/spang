#include "spang.h"
#include <math.h>
int size;
int xpos = 0;
/*
void draw_test (void)
{
    float x, y;

    size = screen_height / 2;

    for (x = 0; x <= screen_width; x += screen_width / (2 * M_PI) )
    {
        y = sin (x);
        //fprintf (stdout, "%.2f, %.2f\n", x*10, y*10);
        SDL_SetRenderDrawColor (renderer, 0,255,0,0);
        SDL_RenderDrawPoint(renderer, x, (y * size)+ (screen_height /2));
    }
    //gamestate = GAME_AMODE;
}
*/
/*
 *Necrocomment: Ha ha ha
void throw_life_away (void)
{
    gamestate = GAME_DEMO;
    Mix_PlayMusic (music[MUSIC_WEDDING], -1);
}
*/
void draw_test (void)
{
    if (xpos < screen_height / 2)
    {
        xpos ++;
        render_string_centre ("POO PLOP WILLY BUM", xpos, red, font4);

    }
    else
        render_string_centre ("ASH IS KING", xpos, green, font5);
}
