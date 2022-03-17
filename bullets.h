#define MAX_BULLETS 100

struct bullet_t
{
    SDL_Rect rect;
    float xvel;
    float yvel;
    int size;
};

extern struct bullet_t bullets[MAX_BULLETS];

void bullets_init (void);
void bullets_draw (void);
void bullet_add (void);
void bullet_remove (int num);
