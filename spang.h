#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_ttf.h>

#include <stdio.h>
#include <stdlib.h>

#include "balls.h"
#include "enemy.h"
#include "player.h"
#include "bullets.h"
#include "detect.h"

extern int screen_width;
extern int screen_height;
extern int running;
extern int running;
extern int draw_hitbox;
extern int score_bonus;
extern int paused;

extern Mix_Chunk *laser1;
extern Mix_Chunk *explosion;
extern Mix_Chunk *playerhit;
extern Mix_Chunk *death;
extern Mix_Chunk *comboup;
extern Mix_Chunk *combodown;
extern Mix_Chunk *levelup;
extern Mix_Chunk *speedup;
extern Mix_Chunk *alarm;
extern Mix_Chunk *health1;
extern Mix_Chunk *kaching;
extern Mix_Chunk *slowdown;
extern Mix_Chunk *alarmreverse;
extern Mix_Chunk *tink;
extern Mix_Chunk *siren;

enum
{
    SND_NONE,
    SND_LASER,
    SND_EXPLOSION,
    SND_COMBO,
    SND_DEATH,
    SND_MUSIC
};

extern SDL_Texture *ball_tex;
extern SDL_Texture *bullet_tex;
extern SDL_Texture *ship_tex;
extern SDL_Texture *bg_tex;

extern SDL_Texture *health_tex;
extern SDL_Texture *coin_tex;
extern SDL_Texture *slow_tex;
extern SDL_Texture *megashot_tex;

extern SDL_Texture *enemy_tex;

extern SDL_Renderer *renderer;
extern SDL_Rect player_hitrect1;
extern SDL_Rect player_hitrect2;

int sdl_init (void);
void sdl_read_input (void);

int check_axis (SDL_Rect a, SDL_Rect b);

int textures_init (void);
int textures_load (int texture);

extern int level_change_timer;
void level_up (void);
void level_change_pause (void);

void combo_increment (void);
void combo_decrement (void);
void combo_set_level (int level);
void combo_hit (int size);

//resources.c
int fonts_load (void);
int render_score (void);
int audio_init (void);

//powerup.c
typedef enum
{
    POWERUP_NONE,
    POWERUP_HEALTH,
    POWERUP_COIN,
    POWERUP_SLOW,
    POWERUP_MEGASHOT
}powerup_t;

void powerups_init (void);
void powerup_add (powerup_t type, int xpos, int ypos);
void powerups_draw (void);
void powerup_smartbomb (void);
void powerups_textures_init (void);
extern int megashot_active;

void explosions_init (void);
void explosion_add (int xpos, int ypos);
void explosion_superbomb (int xpos, int ypos);
void explosions_draw (void);
extern int trippy_mode;

typedef enum
{
    ALIGN_NONE,
    ALIGN_RIGHT,
    ALIGN_CENTRE,
    ALIGN_TCENTRE

} align_t;

void msg_draw (void);
void msg_level_up (void);
void msg_init (void);
void msg_show (char *msg, int xpos, int ypos, int time, TTF_Font *font, align_t align, SDL_Colour colour);

//hiscore
void hiscore_init (void);
int hiscore_save (void);
int hiscore_check (void);

extern long hiscore;

extern TTF_Font *font1;
extern TTF_Font *font2;
extern TTF_Font *font3;
extern SDL_Color red;
extern SDL_Color green;
extern SDL_Color blue;
extern SDL_Color black;
extern SDL_Color white;





//enemy.c

//bg.c
void stars_init (void);
void stars_draw (void);
