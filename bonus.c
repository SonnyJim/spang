#include "spang.h"
SDL_Texture *bonus_tex;

#define BARREL_W 50
#define BARREL_H 50
#define GRAVITY 0.1

#define NUM_TARGETS 4

int bonus_perfect;

//TODO Award bonus pojnts for lower targets, health for upper targets?

struct targets_t
{
    SDL_Rect rect;
    char label[64];
    long    score;
};

struct targets_t bonus_targets[NUM_TARGETS];

int bonus_level_active = 0;
int bonus_level_time;
int old_combo_level;

enum
{
    TOP_LEFT,
    BOTTOM_LEFT,
    TOP_RIGHT,
    BOTTOM_RIGHT
};

/*
void bonus_loop (void)
{
    fprintf (stdout, "Bonus loop\n");
    game_loop();
    bonus_draw ();
}
*/
static void bonus_barrels_init (void)
{
    int i;
    for (i = 0; i < BONUS_BARRELS; i++)
    {
        barrels[i].active = 0;
        barrels[i].angle = 1;
        barrels[i].angle_vel = 0;
        barrels[i].rect.x = 0;
        barrels[i].rect.y = 0;
        barrels[i].rect.w = BARREL_W;
        barrels[i].rect.h = BARREL_H;
        barrels[i].size = 1;
        barrels[i].speed = 1;
        barrels[i].xvel = 1;
        barrels[i].yvel = 1;
    }

    bonus_targets[BOTTOM_LEFT].score = 5000;
    bonus_targets[BOTTOM_RIGHT].score = 5000;
    bonus_targets[TOP_LEFT].score = 20000;
    bonus_targets[TOP_RIGHT].score = 20000;

    strcpy (bonus_targets[BOTTOM_LEFT].label, "5000");
    strcpy (bonus_targets[BOTTOM_RIGHT].label, "5000");
    strcpy (bonus_targets[TOP_LEFT].label, "20000");
    strcpy (bonus_targets[TOP_RIGHT].label, "20000");

    TTF_SizeText (font3, bonus_targets[BOTTOM_LEFT].label, &bonus_targets[BOTTOM_LEFT].rect.w, &bonus_targets[BOTTOM_LEFT].rect.h);
    TTF_SizeText (font3, bonus_targets[TOP_LEFT].label, &bonus_targets[TOP_LEFT].rect.w, &bonus_targets[TOP_LEFT].rect.h);

    bonus_targets[BOTTOM_RIGHT].rect.w = bonus_targets[BOTTOM_LEFT].rect.w;
    bonus_targets[BOTTOM_RIGHT].rect.h = bonus_targets[BOTTOM_LEFT].rect.h;
    bonus_targets[TOP_RIGHT].rect.w = bonus_targets[TOP_LEFT].rect.w;
    bonus_targets[TOP_RIGHT].rect.h = bonus_targets[TOP_LEFT].rect.h;

    bonus_targets[TOP_LEFT].rect.x = 0;
    bonus_targets[TOP_LEFT].rect.y = (screen_height / 2) - 200;
    bonus_targets[BOTTOM_LEFT].rect.x = 0;
    bonus_targets[BOTTOM_LEFT].rect.y = (screen_height / 2) + 200;

    bonus_targets[TOP_RIGHT].rect.x = screen_width - bonus_targets[TOP_RIGHT].rect.w;
    bonus_targets[TOP_RIGHT].rect.y = (screen_height / 2) - 200;
    bonus_targets[BOTTOM_RIGHT].rect.x = screen_width - bonus_targets[BOTTOM_RIGHT].rect.w;
    bonus_targets[BOTTOM_RIGHT].rect.y = (screen_height / 2) + 200;

}

static void bonus_barrel_remove (int num)
{
    barrels[num].active = 0;
}

static void bonus_barrel_add (int xpos, int ypos, int size, int xvel, int strength)
{
    int i;
    if (bonus_level_time == 0)
        return;

    for (i = 0; i < BONUS_BARRELS; i++)
    {
        if (barrels[i].active == 0)
        {
           barrels[i].active = 1;
           barrels[i].rect.w = BARREL_W * size;
           barrels[i].rect.h = BARREL_H * size;
           barrels[i].rect.x = xpos - (barrels[i].rect.w / 2);
           barrels[i].rect.y = ypos - (barrels[i].rect.h / 2);
           barrels[i].angle = 1;
           barrels[i].angle_vel = 0;
           barrels[i].yvel = -3;
           barrels[i].xvel = xvel;
           barrels[i].hits = 0;
           barrels[i].strength = strength;
           return;
        }
    }
}

static void bonus_barrel_score (int num, int target)
{
    Mix_PlayChannel (SND_EXPLOSION, explosion, 0);
    explosion_add (barrels[num].rect.x, barrels[num].rect.y);
    bonus_barrel_remove (num);
    bonus_barrel_add (screen_width / 2, 100, 3, 0, 5);
    player.score += bonus_targets[target].score;
}

static void bonus_barrel_update (int num)
{
    barrels[num].angle += barrels[num].xvel;
    if (barrels[num].angle > 360)
        barrels[num].angle = 0;
    else if (barrels[num].angle <= 0)
        barrels[num].angle = 360;

    barrels[num].yvel += GRAVITY;
    barrels[num].rect.x += barrels[num].xvel;
    barrels[num].rect.y += barrels[num].yvel;

    if (check_axis (barrels[num].rect, bonus_targets[TOP_LEFT].rect))
    {
        bonus_barrel_score (num, TOP_LEFT);
        return;
    }
    else if (check_axis (barrels[num].rect, bonus_targets[BOTTOM_LEFT].rect))
    {
            bonus_barrel_score (num, BOTTOM_LEFT);
            return;
    }
    else if (check_axis (barrels[num].rect, bonus_targets[TOP_RIGHT].rect))
    {
        bonus_barrel_score (num, TOP_RIGHT);
        return;
    }
    else if (check_axis (barrels[num].rect, bonus_targets[BOTTOM_RIGHT].rect))
    {
        bonus_barrel_score (num, BOTTOM_RIGHT);
        return;
    }

    if (barrels[num].rect.x <= 0)
    {
        if (barrels[num].xvel == 0)
            barrels[num].xvel = 1;
        barrels[num].xvel = -barrels[num].xvel;

    }
    else if (barrels[num].rect.x + barrels[num].rect.w >= screen_width)
    {
        if (barrels[num].xvel == 0)
            barrels[num].xvel = 1;
        barrels[num].xvel = -barrels[num].xvel;

    }

    if (barrels[num].rect.y <= 0)
        barrels[num].yvel = barrels[num].speed;
    else if (barrels[num].rect.y >= screen_height) //The barrel fell off the screen
    {
        //TODO Do we really want to reduce the bonus level time?
        /*
        if (bonus_level_time > 60 * 5)
            bonus_level_time -= 60 * 5;
        */
        bonus_perfect = 0;
        bonus_barrel_remove (num);
        bonus_barrel_add (screen_width / 2, 100, 3, 0, 5);
        Mix_PlayChannel (SND_DEATH, slowdown, 0);

    }
}

void bonus_barrel_hit (struct barrel_t *barrel, struct bullet_t *bullet)
{
    if (level_change_timer)
        return;
    if (++barrel->hits > barrel->strength)
    {
        Mix_PlayChannel (SND_EXPLOSION, explosion, 0 );
        explosion_add (barrel->rect.x, barrel->rect.y);
        barrel->active = 0;
        player.score += 500;
        msg_show ("500", barrel->rect.x, barrel->rect.y, 2, font3, ALIGN_TCENTRE, red);
        bonus_perfect = 0;
        bonus_barrel_add (screen_width / 2, 100, 3, 0, barrel->strength + 1);
        return;
    }
    Mix_PlayChannel (SND_EXPLOSION, snd_barrel, 0);
    //Spin barrel based on bullet hit position
    barrel->xvel = ((barrel->rect.x + (barrel->rect.w / 2))
                         - (bullet->rect.x + (bullet->rect.w / 2))) / 7;
    barrel->yvel += -7;
}

//Copy the player shooting values so we can reset them after the bonus level is finished
static void bonus_store_player (void)
{
	//TODO This looks like it's the source of the 'laser level mysteriously jumps back up to full belt' after a bonus round
    /*
    bonus_player.bullet_delay = player.bullet_delay;
    bonus_player.bullet_max = player.bullet_max;
    bonus_player.bullet_size = player.bullet_size;
    bonus_player.combo_level = player.combo_level;
    */
    player.bullet_delay = 200;
    player.bullet_max = 5;
    player.bullet_size = 10;
    player.combo_level = 0;

    old_combo_level = player.combo_level;
    combo_set_level (player.combo_level);

}

static void bonus_restore_player (void)
{
    /*
    player.bullet_delay = bonus_player.bullet_delay;
    player.bullet_max = bonus_player.bullet_max;
    player.bullet_size = bonus_player.bullet_size;
    */
    player.combo_level = old_combo_level;
    combo_set_level (player.combo_level);
}

void bonus_level_start (void)
{
    fprintf (stdout, "Bonus level start\n");
    gamestate = GAME_BONUS;
    powerup_megashot_disable ();
    bonus_barrels_init ();
    bonus_store_player ();
    bonus_level_active = 1;
    bonus_level_time = 30 * 60;
    player.bonus_level++;
    level_change_timer = 3 * 60;
    bonus_barrel_add (screen_width / 2, 100, 3, 0, 7);
    bonus_perfect = 1;
    //Find megashot powerup and remove it
    Mix_PlayMusic (music[MUSIC_BONUS], -1);

}

void bonus_level_stop (void)
{
    if (bonus_perfect)
    {
        msg_show ("PERFECT", screen_width /2, screen_height - 200, 3, font2, ALIGN_CENTRE, red);
        msg_show ("50000", player.rect.x + (player.rect.w / 2), player.rect.y - 20, 3, font3, ALIGN_TCENTRE, white);

        player.score += 50000;
    }
    bonus_restore_player ();
    bonus_level_active = 0;
    //TODO Put in a wait loop so the next level doesn't start so quickly
    level_up ();
}
static void bonus_draw_message (void)
{
    char buffer[1024] = "";
    sprintf (buffer, "Bonus level %i", player.bonus_level);
    render_string_centre (buffer, (screen_height / 2) - 200 ,green, font2);
    if (player.bonus_level == 1)
    {
        sprintf (buffer, "Shoot barrel toward ledges for reward");
        render_string_centre (buffer, (screen_height / 2) - 100, green, font3);
    }
}

static void bonus_draw_targets (void)
{
    int i;
    for (i = 0; i < NUM_TARGETS; i++)
    {
        SDL_SetRenderDrawColor (renderer, 255, 0 ,0 ,0);
        SDL_RenderDrawRect (renderer, &bonus_targets[i].rect);
        render_string (bonus_targets[i].label, bonus_targets[i].rect.x, bonus_targets[i].rect.y, white, font3);
    }
}

void bonus_draw (void)
{
    int i;
    int found = 0;
    char buffer[1024] = "";
    for (i = 0; i < BONUS_BARRELS; i++)
    {
        if (level_change_timer)
        {
            bonus_draw_message ();
        }
        else if (barrels[i].active)
        {
            found = 1;
            bonus_barrel_update (i);
            bonus_draw_targets ();
            if (draw_hitbox)
                SDL_RenderDrawRect (renderer, &barrels[i].rect);
            SDL_RenderCopyEx (renderer, bonus_tex, NULL, &barrels[i].rect, barrels[i].angle, NULL, 0);

            if (bonus_level_time > 0)
            {
                sprintf (buffer, "TIME %i", bonus_level_time / 60);
                if (bonus_level_time > 5 * 60)
                    render_string_centre (buffer, 80, green, font2);
                else
                    render_string_centre (buffer, 80, red, font2);
            }
        }
    }

    if (found && bonus_level_time > 0)
        bonus_level_time--;
    else if (bonus_level_time <= 0 && !found)
        bonus_level_stop ();
}
