#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_ttf.h>

#include <stdio.h>
#include <stdlib.h>

#define NUM_BALLS 1024
#define MAX_BULLETS 100

extern int screen_width;
extern int screen_height;
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
extern Mix_Chunk *test;

enum
{
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

extern SDL_Renderer *renderer;
extern SDL_Rect ball_rects[NUM_BALLS];
extern SDL_Rect bullet_rects[MAX_BULLETS];
extern SDL_Rect player_rect;
extern SDL_Rect player_hitrect1;
extern SDL_Rect player_hitrect2;

int sdl_init (void);
void sdl_read_input (void);

int check_axis (SDL_Rect a, SDL_Rect b);

void bullets_init (void);
void bullets_draw (void);
void bullet_add (void);
void bullet_remove (int num);

void ball_add (int size, int xpos, int ypos, int speed, int direction);
void ball_split (int ball_num);
void balls_init_all (void);
void balls_draw (void);
void balls_check (void);
void balls_speed_change (void);

void detect_collision (int num);

void player_init (void);
void player_move_left (void);
void player_move_right (void);
void player_draw (void);
void player_hit (void);
void player_score (int size);
void player_level_up (void);

void combo_increment (void);
void combo_decrement (void);
void combo_set_level (int level);

int fonts_load (void);
int render_score (void);
void show_level_up (void);

void powerups_init (void);
void powerup_add (int type, int xpos, int ypos);
void powerups_draw (void);
void powerup_smartbomb (void);
extern int megashot_active;



void explosions_init (void);
void explosion_add (int xpos, int ypos);
void explosions_draw (void);
extern int trippy_mode;

enum
{
    POWERUP_NONE,
    POWERUP_HEALTH,
    POWERUP_COIN,
    POWERUP_SLOW,
    POWERUP_MEGASHOT
};

struct ball_t
{
    int xpos;
    int ypos;
    float xvel;
    float yvel;
    double angle;
    int angle_vel;
    int size;
};

struct player_t
{
    int xpos;
    int ypos;
    int lives;
    long score;
    long bullet_delay;
    int bullet_max;
    int bullet_size;
    int health;
    int combo;
    int combo_level;
    Uint32 invulnerable;
    float speed;
    long shots_fired;
    long hits;
    int level;
    Uint32 stage_time;
    int last_size;
    int smartbomb;
};

struct player_t player;

struct ball_t balls[NUM_BALLS];

struct bullet_t
{
    int xpos;
    int ypos;
    float xvel;
    float yvel;
    int size;
};

struct bullet_t bullets[MAX_BULLETS];
