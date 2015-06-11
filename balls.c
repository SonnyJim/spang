#include "spang.h"
#include "balls.h"
SDL_Texture *ball_tex = NULL;
Mix_Chunk *levelup = NULL;

void balls_speed_up (void)
{
    int i;
    for (i = 0; i < NUM_BALLS; i++)
    {
        if (balls[i].size != 0)
        {
            if (balls[i].xvel > 0)
                balls[i].xvel = player.speed;
            else
                balls[i].xvel = -player.speed;

            if (balls[i].yvel > 0)
                balls[i].yvel = player.speed;
            else
                balls[i].yvel = -player.speed;
        }
    }
}

void balls_check (void)
{
    int found = 0;
    int i;
    for (i = 0; i < NUM_BALLS; i++)
    {
        if (balls[i].size != 0)
        {
            found = 1;
            break;
        }
    }
    if (!found)
    {
        player_level_up ();
    }
}
void ball_add (int size, int xpos, int ypos, int speed, int direction)
{
    int i;
    for (i = 0; i < NUM_BALLS; i++)
    {
        if (balls[i].size == 0)
        {
            balls[i].size = size;
            balls[i].xpos = xpos;
            balls[i].ypos = ypos;
            if (direction)
                balls[i].xvel = speed;
            else
                balls[i].xvel = -speed;

            balls[i].yvel = -speed;

            ball_rects[i].w = balls[i].size * 20;
            ball_rects[i].h = ball_rects[i].w;
            return;
        }
    }
    return;
}

void ball_split (int ball_num)
{
    int size;
    Mix_PlayChannel( SND_EXPLOSION, explosion, 0 );
    combo_increment ();
    //If the ball is only size 1, then just destroy it
    if ( balls[ball_num].size == 1)
    {
        balls[ball_num].size = 0;
        return;
    }

    if (player.hits % 50 == 0)
        powerup_add (POWERUP_HEALTH, balls[ball_num].xpos, balls[ball_num].ypos);
    size = balls[ball_num].size - 1;
    balls [ball_num].size = 0;
    ball_add (size, balls[ball_num].xpos - (ball_rects[ball_num].w / 2), balls[ball_num].ypos, player.speed, 0);
    ball_add (size, balls[ball_num].xpos + (ball_rects[ball_num].w / 2), balls[ball_num].ypos, player.speed, 1);
}


void ball_init (int num)
{
    balls[num].xpos = rand () % screen_width;
    balls[num].ypos = rand () % screen_height;
    if (rand() % 2)
        balls[num].xvel = 5;
    else
        balls[num].xvel = -5;

    if (rand() % 2)
        balls[num].yvel = 5;
    else
        balls[num].yvel = -5;

    balls[num].angle = 0;
    balls[num].angle_vel = (rand () % 6) - 10;
    balls[num].size = 0;

}


void balls_init_all (void)
{
    int i;
    for (i = 0; i < NUM_BALLS; i++)
    {
        ball_init(i);
    }
}

void ball_update (int num)
{
    balls[num].xpos += balls[num].xvel;
    balls[num].ypos += balls[num].yvel;

    balls[num].angle += balls[num].angle_vel;
    if (balls[num].angle > 360)
        balls[num].angle = 0;
    if (balls[num].angle <= 0)
        balls[num].angle = 360;


    ball_rects[num].x = balls[num].xpos;
    ball_rects[num].y = balls[num].ypos;

    if (balls[num].xpos <= 0)
        balls[num].xvel = player.speed;
    if (balls[num].xpos + ball_rects[num].w >= screen_width)
        balls[num].xvel = -player.speed;

    if (balls[num].ypos + ball_rects[num].h >= screen_height)
        balls[num].yvel = -player.speed;
    if (balls[num].ypos <= 0)
        balls[num].yvel = player.speed;
}

void balls_draw (void)
{
    int i;
    for (i = 0; i < NUM_BALLS; i++)
    {
        if (balls[i].size > 0)
        {
            ball_update (i);
            if (draw_hitbox)
            {
                SDL_SetRenderDrawColor( renderer, 0, 255, 0, 0 );
                SDL_RenderDrawRect( renderer, &ball_rects[i]);
            }
            SDL_RenderCopyEx (renderer, ball_tex, NULL, &ball_rects[i], balls[i].angle, NULL, 0);
        }
    }
}
