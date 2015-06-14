#define MAX_BALLS 1024
struct ball
{
    SDL_Rect rect;
    float xvel;
    float yvel;
    double angle;
    int angle_vel;
    int size;
    int strength;
    int hits;
};

struct ball balls[MAX_BALLS];

void ball_add (int size, int xpos, int ypos, int speed, int direction, int strength);
void ball_hit (int ball_num);
void balls_init_all (void);
void balls_draw (void);
void balls_check (void);
void balls_speed_change (void);
