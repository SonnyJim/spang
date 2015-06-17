extern int gamestate;

enum state_t
{
    GAME_AMODE,
    GAME_RUNNING,
    GAME_OVER,
    GAME_HSENTRY,
    GAME_CONFIG,
    GAME_CONFIG_INPUT
};

void game_pause (void);
void game_unpause (void);
void game_start (void);
void game_loop (void);
void gameover_loop (void);
