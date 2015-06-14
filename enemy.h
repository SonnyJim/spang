#define MAX_ENEMIES 32

typedef enum
{
    ENEMY_NONE,
    ENEMY_CENTIPEDE
} enemy_t;

typedef enum
{
    BULLET_NONE,
    BULLET_NORMAL
} bullet_t;

struct enemy
{
    SDL_Rect rect;
    float xvel;
    float yvel;
    int data;
    enemy_t type;
    Uint16 time;
    int strength;
    int hits;
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
int enemy_add (enemy_t type, int xpos, int ypos, float xvel, float yvel, int strength);
void enemy_bullet_remove (int bullet);


