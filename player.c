#include "spang.h"
SDL_Rect player_rect;
SDL_Rect player_hitrect1;
SDL_Rect player_hitrect2;



Mix_Chunk *alarm;

int score_bonus = 10;

int invuln_delay = 2 * 60;

void player_init (void)
{
    player_rect.w = 50;
    player_rect.h = 50;
    player.xpos = screen_width / 2 - (player_rect.w / 2);
    player.ypos = screen_height - player_rect.h;
    player.bullet_delay = 200;
    player.bullet_max = 5;
    player.bullet_size = 10;
    player.health = 100;
    player.speed = 1;
    player.shots_fired = 0;
    player.shots_fired_round = 0;
    player.hits = 0;
    player.hits_round = 0;
    player.combo = 1;
    player.combo_level = 0;
    player.level = 0;
    player.stage_time = 0;
    player.last_size = 0;

    player_hitrect1.w = 14;
    player_hitrect1.h = 25;
    player_hitrect2.w = 50;
    player_hitrect2.h = 25;
    player.smartbomb = 1;

    player.destroyed_balls = 0;
    player.invuln_time = 0;
}

void player_update_hitrect (void)
{
        player_hitrect1.x = player_rect.x + 18;
        player_hitrect1.y = player_rect.y;
        player_hitrect2.x = player_rect.x;
        player_hitrect2.y = player_rect.y + 25;
}

void player_score (int size)
{
    switch (size)
    {
        case 5:
            player.score += 1000;
            break;
        case 4:
            player.score += 500;
            break;
        case 3:
            player.score += 200;
            break;
        case 2:
            player.score += 100;
            break;
        case 1:
            player.score += 50;
            break;
    }

    player.score += score_bonus;
    if (score_bonus == 800)
        powerup_add (POWERUP_COIN, rand () % screen_width, 0);
    hiscore_check ();
}

void player_hit (void)
{
    Mix_PlayChannel( SND_MUSIC, playerhit, 0 );
    player.invuln_time = invuln_delay;
    player.health -= 20;
    player.combo /= 2;
    if (player.health <= 0)
        running = 0;
    else if (player.health <= 20)
    {
        //Mix_PauseMusic();
        Mix_PlayChannel (SND_MUSIC, alarm, 0);
    }
    /*
    else if (player.health > 20 && !megashot_active)
        Mix_ResumeMusic();
        */
}

void player_move_left (void)
{
    if (player.xpos > 0)
        player.xpos -= 10;
}

void player_move_right (void)
{
    if (player.xpos + player_rect.w < screen_width)
        player.xpos += 10;
}
void player_ball_destroyed (void)
{
    player.destroyed_balls++;
    if (player.destroyed_balls % 31 == 0)
    {
        Mix_PlayChannel (SND_MUSIC, speedup, 0);
        player.speed += 0.5;
        balls_speed_change ();

    }
}



void player_draw (void)
{
    player_rect.x = player.xpos;
    player_rect.y = player.ypos;
    player_update_hitrect ();
    if (draw_hitbox)
    {
        SDL_SetRenderDrawColor( renderer, 0, 255, 0, 0 );
        SDL_RenderDrawRect( renderer, &player_hitrect1);
        SDL_RenderDrawRect (renderer, &player_hitrect2);
    }
    if (player.invuln_time)
    {
        player.invuln_time--;
        if (player.invuln_time % 5 == 0)
            return;

    }
    SDL_RenderCopy (renderer, ship_tex, NULL, &player_rect);
}
