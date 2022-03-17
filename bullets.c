#include "spang.h"

//SDL_Rect bullet_rects[MAX_BULLETS];
Uint32 bullet_timer = 0;
int bullets_on_screen = 0;
struct bullet_t bullets[MAX_BULLETS];

void bullets_init (void)
{
    int i;
    for (i = 0; i < MAX_BULLETS; i++)
    {
        bullets[i].rect.x = 0;
        bullets[i].rect.y = 0;
        bullets[i].xvel = 0;
        bullets[i].yvel = 0;
        bullets[i].size = 0;
        bullets[i].rect.w = 10;
        bullets[i].rect.h = 10;
    }
    bullet_timer = 0;
    bullets_on_screen = 0;
}

void bullet_add (void)
{
    int i;
    //Don't allow adding any more bullets
    if (bullet_timer + player.bullet_delay > frame_counter|| bullets_on_screen >= player.bullet_max)
        return;
    player.shots_fired++;
    player.shots_fired_round++;
    bullets_on_screen++;
    Mix_PlayChannel( SND_LASER, laser1, 0 );
    for (i = 0; i < MAX_BULLETS; i++)
    {
        if (bullets[i].size == 0)
        {
            bullets[i].rect.x = player.rect.x + (player.rect.w /2) - (bullets[i].rect.w / 2);
            bullets[i].rect.y = screen_height - player.rect.h - player.bullet_size;
            bullets[i].xvel = 0;
            bullets[i].yvel = -10;
            bullets[i].size = player.bullet_size;
            bullets[i].rect.w = player.bullet_size;
            bullets[i].rect.h = player.bullet_size * 1.7;
            break;
        }
    }
    bullet_timer = frame_counter;
}

void bullet_remove (int num)
{
    bullets_on_screen--;
    bullets[num].size = 0;
}

void bullet_update (int num)
{
    bullets[num].rect.x += bullets[num].xvel;
    bullets[num].rect.y += bullets[num].yvel;

    bullets[num].rect.x = bullets[num].rect.x;
    bullets[num].rect.y = bullets[num].rect.y;

    if (bullets[num].rect.x <= 0 || bullets[num].rect.x + bullets[num].rect.w >= screen_width ||
        bullets[num].rect.y <= 0 || bullets[num].rect.y + bullets[num].rect.h >= screen_height)
    {
        combo_decrement ();
        bullet_remove(num);
    }

    detect_bullet (num);
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
            SDL_RenderDrawRect( renderer, &bullets[i].rect);
            */
            SDL_RenderCopy (renderer, bullet_tex, NULL, &bullets[i].rect);
        }
    }
}


