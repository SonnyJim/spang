#include "spang.h"

SDL_Rect bullet_rects[MAX_BULLETS];
Uint32 bullet_timer = 0;
int bullets_on_screen = 0;

void bullets_init (void)
{
    int i;
    for (i = 0; i < MAX_BULLETS; i++)
    {
        bullets[i].xpos = 0;
        bullets[i].ypos = 0;
        bullets[i].xvel = 0;
        bullets[i].yvel = 0;
        bullets[i].size = 0;
        bullet_rects[i].w = 10;
        bullet_rects[i].h = 10;
    }
}

void bullet_add (void)
{
    int i;

    //Don't allow adding any more bullets
    if (bullet_timer + player.bullet_delay > SDL_GetTicks () || bullets_on_screen >= player.bullet_max)
        return;
    player.shots_fired++;
    player.shots_fired_round++;
    bullets_on_screen++;
    Mix_PlayChannel( SND_LASER, laser1, 0 );
    for (i = 0; i < MAX_BULLETS; i++)
    {
        if (bullets[i].size == 0)
        {
            bullets[i].xpos = player.xpos + (player_rect.w /2) - (bullet_rects[i].w / 2);
            bullets[i].ypos = screen_height - player_rect.h - player.bullet_size;
            bullets[i].xvel = 0;
            bullets[i].yvel = -10;
            bullets[i].size = player.bullet_size;
            bullet_rects[i].w = player.bullet_size;
            bullet_rects[i].h = player.bullet_size * 1.7;
            break;
        }
    }
    bullet_timer = SDL_GetTicks ();
}

void bullet_remove (int num)
{
    bullets_on_screen--;
    bullets[num].size = 0;
}

void bullet_update (int num)
{
    bullets[num].xpos += bullets[num].xvel;
    bullets[num].ypos += bullets[num].yvel;

    bullet_rects[num].x = bullets[num].xpos;
    bullet_rects[num].y = bullets[num].ypos;

    if (bullets[num].xpos <= 0 || bullets[num].xpos + bullet_rects[num].w >= screen_width ||
        bullets[num].ypos <= 0 || bullets[num].ypos + bullet_rects[num].h >= screen_height)
        {
            combo_decrement ();
            bullet_remove(num);
        }

    detect_collision (num);
}

void bullets_draw (void)
{
    int i;
    for (i = 0; i < MAX_BULLETS; i++)
    {
        if (bullets[i].size > 0)
        {
            bullet_update (i);
            /*
            SDL_SetRenderDrawColor( renderer, 255,0, 0, 0 );
            SDL_RenderDrawRect( renderer, &bullet_rects[i]);
            */
            SDL_RenderCopy (renderer, bullet_tex, NULL, &bullet_rects[i]);
        }
    }
}


