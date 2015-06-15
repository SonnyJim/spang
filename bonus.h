#define BONUS_BARRELS 8
struct barrel_t
{
    SDL_Rect rect;
    float xvel;
    float yvel;
    double angle;
    float speed;
    int angle_vel;
    int active;
    int size;
    int hits;
    int strength;
};

struct barrel_t barrels[BONUS_BARRELS];

extern SDL_Texture *bonus_tex;

extern int bonus_level_active;

void bonus_barrel_hit (struct barrel_t *barrel, struct bullet_t *bullet);
void bonus_draw (void);
void bonus_level_start (void);
void bonus_level_stop (void);
