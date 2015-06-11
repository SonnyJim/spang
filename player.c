#include "spang.h"
SDL_Rect player_rect;
SDL_Rect player_hitrect1;
SDL_Rect player_hitrect2;

SDL_Texture *ship_tex;

Mix_Chunk *alarm;

int score_bonus = 10;

int invuln_delay = 1500;

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
    player.invulnerable = 0;
    player.speed = 1;
    player.shots_fired = 0;
    player.hits = 0;
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

    if (player.last_size == size)
    {
        score_bonus += score_bonus;
        if (score_bonus > 800)
            score_bonus = 800;
        //fprintf (stdout, "last %i size %i adding %i\n", player.last_size, size, score_bonus);
        player.score += score_bonus;
    }
    else
        score_bonus = 10;
    player.last_size = size;
    if (score_bonus == 800)
        powerup_add (POWERUP_COIN, rand () % screen_width, 0);
}
void player_hit (void)
{
    if (player.invulnerable + invuln_delay > SDL_GetTicks ())
        return;
    Mix_PlayChannel( SND_MUSIC, playerhit, 0 );
    player.invulnerable = SDL_GetTicks ();
    player.health -= 20;
    player.combo /= 2;
    if (player.health <= 0)
        running = 0;
    else if (player.health <= 20)
        Mix_PlayChannel (SND_MUSIC, alarm, 0);
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

void player_level_up (void)
{
    int i;
    Mix_PlayChannel (SND_MUSIC, levelup, 0);

    player.level++;
    for (i = 0; i < player.level; i ++)
        ball_add (5, screen_width / 2 + (rand () % 200) - 100, 200 + (rand() % 50), player.speed, rand () % 1);
    if (player.level > 1)
    {
        fprintf (stdout, "Level %i: Stage time: %i\n", player.level, (SDL_GetTicks() - player.stage_time) / 100);
    }
    if (player.level % 3 == 0)
        powerup_add (POWERUP_SLOW, rand () % screen_width, 0);
    if (player.level % 5 == 0)
        powerup_add (POWERUP_MEGASHOT, rand () % screen_width, 0);
    show_level_up ();
    player.stage_time = SDL_GetTicks ();
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
    SDL_RenderCopy (renderer, ship_tex, NULL, &player_rect);
}
