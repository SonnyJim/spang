#include "spang.h"
//SDL_Rect player_rect;
SDL_Rect player_hitrect1;
SDL_Rect player_hitrect2;

Mix_Chunk *alarm;

int score_bonus = 10;

int invuln_delay = 2 * 60;

void player_hit_inc (int xpos, int ypos)
{
    if (player.hits % (100 * player.laps) == 0)
        powerup_add (POWERUP_HEALTH, xpos, ypos);
}

void player_init (void)
{
    player.rect.w = 50;
    player.rect.h = 50;
    player.rect.x = screen_width / 2 - (player.rect.w / 2);
    player.rect.y = screen_height - player.rect.h;
    player.bullet_delay = 10;
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
    player.score = 0;
    score_bonus = 10;
    player.combo_time = 0;

    player_hitrect1.w = 14;
    player_hitrect1.h = 25;
    player_hitrect2.w = 50;
    player_hitrect2.h = 25;
    player.smartbomb = 1;

    player.destroyed_balls = 0;
    player.invuln_time = 0;

    player.bonus_level = 0;

    player.laps = 0;
    combo_set_level (player.combo_level);
}

void player_update_hitrect (void)
{
        player_hitrect1.x = player.rect.x + 18;
        player_hitrect1.y = player.rect.y;
        player_hitrect2.x = player.rect.x;
        player_hitrect2.y = player.rect.y + 25;
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
        powerup_add (POWERUP_COIN, player.rect.x, 0);

}

void player_hit (void)
{
    Mix_PlayChannel( SND_EXPLOSION, playerhit, 0 );
    player.invuln_time = invuln_delay;
    player.health -= 20;
    player.combo /= 2;
    if (player.health <= 0)
    {
        player.health = 0;
        gamestate = GAME_OVER;
    }
    else if (player.health <= 20)
    {
        Mix_PlayChannel (SND_MUSIC, alarm, 0);
    }
}

void player_move_left (void)
{
    if (player.rect.x > 0)
        player.rect.x -= 10;
}

void player_move_right (void)
{
    if (player.rect.x + player.rect.w < screen_width)
        player.rect.x += 10;
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

void player_trippy_hit (void)
{
    player.trippy_time = SDL_GetTicks();
}
void player_trippy_level_inc (void)
{
    if (player.trippy_level < 5)
        player.trippy_level++;
}

void player_trippy_calc (void)
{
    if (SDL_GetTicks () > player.trippy_time + (2 * 60) && player.trippy_level > 0)
    {
        player.trippy_level--;
    }
}

void player_draw (void)
{
    player_trippy_calc ();
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
    SDL_RenderCopy (renderer, ship_tex, NULL, &player.rect);
}

