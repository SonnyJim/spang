void player_init (void);
void player_move_left (void);
void player_move_right (void);
void player_draw (void);
void player_hit (void);
void player_score (int size);
void player_trippy_hit (void);
void player_trippy_level_inc (void);
void player_hit_inc (int xpos, int ypos);

void player_ball_destroyed (void);

struct player_t
{
    SDL_Rect rect;
    int lives;
    long score;
    long bullet_delay;
    int bullet_max;
    int bullet_size;
    int health;
    int combo;
    int combo_level;
    Uint32 combo_time;
    float trippy_level;
    Uint32 trippy_time;
    float speed;
    long shots_fired;
    long shots_fired_round;
    long hits;
    long hits_round;
    int level;
    Uint32 stage_time;
    int last_size;
    int smartbomb;
    long destroyed_balls;
    int invuln_time;
    int bonus_level;
    int laps;
};

struct player_t player;
