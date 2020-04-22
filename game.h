extern int gamestate;

enum state_t
{
    GAME_AMODE,
    GAME_RUNNING,
    GAME_ENDLEVEL,
    GAME_BONUS,
    GAME_OVER,
    GAME_HSENTRY,
    GAME_CONFIG,
    GAME_CONFIG_INPUT,
    GAME_SELECT_RECORD,
    GAME_DEMO
};

void game_pause (void);
void game_unpause (void);
void game_start (void);
void game_loop (void);
void gameover_loop (void);
extern void level_endlevel_loop (void);
extern void bonus_loop (void);
