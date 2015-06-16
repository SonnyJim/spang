#define MAX_ENEMIES 128
#define CENTIPEDE_WIDTH 40
#define BALL_WIDTH 20

typedef enum
{
    ENEMY_NONE,
    ENEMY_CENTIPEDE,
    ENEMY_HOMING,
    ENEMY_BALL
} enemy_t;

typedef enum
{
    BULLET_NONE,
    BULLET_NORMAL,
    BULLET_HOMING
} bullet_t;

struct enemy
{
    SDL_Rect rect;
    float xvel;
    float yvel;
    int data;
    enemy_t type;
    bullet_t bullet;
    Uint32 time;
    int strength;
    int hits;
    int size;
};

struct enemy enemies[MAX_ENEMIES];

struct enemy_bullet
{
    SDL_Rect rect;
    float xvel;
    float yvel;
    bullet_t type;
};

struct enemy_bullet enemy_bullets[MAX_ENEMIES];

void enemy_draw (void);
void enemy_hit (int num);
void enemy_remove (int enemy);
int enemy_add (enemy_t type, bullet_t bullet, int xpos, int ypos, float xvel, float yvel, int strength, int size);
void enemy_bullet_remove (int bullet);
void enemy_init (void);


