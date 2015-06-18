#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_ttf.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "balls.h"
#include "enemy.h"
#include "player.h"
#include "bullets.h"
#include "detect.h"
#include "level.h"
#include "bonus.h"
#include "overlay.h"
#include "resources.h"
#include "amode.h"
#include "sdl.h"
#include "game.h"
#include "hiscore.h"
#include "hsentry.h"
#include "powerup.h"
#include "config.h"
#include "record.h"
#include "random.h"

extern int screen_width;
extern int screen_height;
extern int running;
extern int running;
extern int draw_hitbox;
extern int score_bonus;
extern int paused;



enum
{
    SND_NONE,
    SND_LASER,
    SND_SIREN,
    SND_EXPLOSION,
    SND_COMBO,
    SND_DEATH,
    SND_MUSIC
};

extern SDL_Texture *ball_tex;
extern SDL_Texture *bullet_tex;
extern SDL_Texture *ship_tex;
extern SDL_Texture *bg_tex;
extern SDL_Texture *balloon_tex;

extern SDL_Texture *health_tex;
extern SDL_Texture *coin_tex;
extern SDL_Texture *slow_tex;
extern SDL_Texture *megashot_tex;

extern SDL_Texture *enemy_tex;


extern SDL_Renderer *renderer;
extern SDL_Rect player_hitrect1;
extern SDL_Rect player_hitrect2;

int sdl_init (void);
void sdl_close (void);

void sdl_read_input (void);

int check_axis (SDL_Rect a, SDL_Rect b);

int textures_init (void);
int textures_load (int texture);

void combo_increment (void);
void combo_decrement (void);
void combo_set_level (int level);
void combo_hit (int size);

//resources.c

int render_score (void);

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


extern TTF_Font *font1;
extern TTF_Font *font2;
extern TTF_Font *font3;
extern SDL_Color red;
extern SDL_Color green;
extern SDL_Color blue;
extern SDL_Color black;
extern SDL_Color white;

//bg.c
void stars_draw (void);
void stars_toggle_rotation (void);

int input_keyrepeat (void);
void input_joystick_init (void);
void input_joystick_close (void);

int network_send_score (char* initials, long score);

