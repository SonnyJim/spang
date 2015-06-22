#include "spang.h"

Mix_Chunk *levelup = NULL;

void balls_speed_change (void)
{
    int i;
    for (i = 0; i < MAX_BALLS; i++)
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

//Check to see how many balls left
void balls_check (void)
{
    int found = 0;
    int i;
    for (i = 0; i < MAX_BALLS; i++)
    {
        if (balls[i].size != 0)
        {
            found = 1;
            break;
        }
    }

    for (i = 0;i < MAX_ENEMIES; i++)
    {
        if (enemies[i].type != ENEMY_NONE)
        {
            found = 1;
            break;
        }
    }

    if (!found)
    {
        if (level_end_timer == 0)
            level_end_timer = 20;
        level_end ();
    }
}

void ball_add (int size, int xpos, int ypos, int speed, int direction, int strength)
{
    int i;
    for (i = 0; i < MAX_BALLS; i++)
    {
        if (balls[i].size == 0)
        {
            balls[i].size = size;
            balls[i].rect.x = xpos - (size * 10);
            balls[i].rect.y = ypos - (size * 10);
            balls[i].strength = strength;
            balls[i].hits = 0;
            if (direction)
                balls[i].xvel = speed;
            else
                balls[i].xvel = -speed;

            balls[i].yvel = -speed;

            balls[i].rect.w = balls[i].size * 20;
            balls[i].rect.h = balls[i].rect.w;
            return;
        }
    }
    return;
}

void ball_hit (int ball_num)
{
    int size;

    combo_increment ();
    //player.combo_level starts at 0
    if (player.combo_level <= 1)
        balls[ball_num].hits++;
    else
        balls[ball_num].hits += player.combo_level + 1;

    combo_hit (balls[ball_num].size);

    if (balls[ball_num].hits < balls[ball_num].strength)
    {
        Mix_PlayChannel (SND_EXPLOSION, tink, 0);
        return;
    }

    player_ball_destroyed ();
    player_score (balls[ball_num].size);
    Mix_PlayChannel( SND_EXPLOSION, explosion, 0 );
    explosion_add (balls[ball_num].rect.x + (balls[ball_num].rect.w / 2),
                               balls[ball_num].rect.y + (balls[ball_num].rect.h /2 ));

    //If the ball is only size 1, then just destroy it
    if ( balls[ball_num].size == 1)
    {
        balls[ball_num].size = 0;
        return;
    }

    player_hit_inc (balls[ball_num].rect.x, balls[ball_num].rect.y);

    size = balls[ball_num].size - 1;

    ball_add (size, balls[ball_num].rect.x + (size /2), balls[ball_num].rect.y + (balls[ball_num].rect.h / 2), player.speed, 0, balls[ball_num].strength - 1);
    ball_add (size, balls[ball_num].rect.x + balls[ball_num].rect.w - (size/2), balls[ball_num].rect.y + (balls[ball_num].rect.h / 2), player.speed, 1, balls[ball_num].strength - 1);
    //Remove the old ball last so the balls before can inherit the properties
    balls [ball_num].size = 0;
}


void ball_init (int num)
{
    /*
    balls[num].rect.x = rand () % screen_width;
    balls[num].rect.y = rand () % screen_height;
    if (rand() % 2)
        balls[num].xvel = 5;
    else
        balls[num].xvel = -5;

    if (rand() % 2)
        balls[num].yvel = 5;
    else
        balls[num].yvel = -5;
*/
    balls[num].xvel = 1;
    balls[num].yvel = 1;
    balls[num].angle = 0;
    balls[num].angle_vel = (rand () % 6) - 10;
    balls[num].size = 0;
    balls[num].strength = 0;
    balls[num].hits = 0;
}


void balls_init_all (void)
{
    int i;
    for (i = 0; i < MAX_BALLS; i++)
    {
        ball_init(i);
    }
}

void ball_update (int num)
{
    balls[num].angle += balls[num].angle_vel;
    if (balls[num].angle > 360)
        balls[num].angle = 0;
    else if (balls[num].angle <= 0)
        balls[num].angle = 360;

    if (level_change_timer)
        return;
    balls[num].rect.x += balls[num].xvel;
    balls[num].rect.y += balls[num].yvel;



    if (balls[num].rect.x <= 0)
        balls[num].xvel = player.speed;
    else if (balls[num].rect.x + balls[num].rect.w >= screen_width)
        balls[num].xvel = -player.speed;

    if (balls[num].rect.y + balls[num].rect.h >= screen_height)
        balls[num].yvel = -player.speed;
    else if (balls[num].rect.y <= 0)
        balls[num].yvel = player.speed;
}

void balls_draw (void)
{
    int i;
    for (i = 0; i < MAX_BALLS; i++)
    {
        if (balls[i].size > 0)
        {
            ball_update (i);
            detect_ballhit (balls[i].rect, i);

            if (draw_hitbox)
            {
                SDL_SetRenderDrawColor( renderer, 0, 255, 0, 0 );
                SDL_RenderDrawRect( renderer, &balls[i].rect);
            }
            SDL_RenderCopyEx (renderer, ball_tex, NULL, &balls[i].rect, balls[i].angle, NULL, 0);
        }
    }
}
