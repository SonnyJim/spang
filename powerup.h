typedef enum
{
    POWERUP_NONE,
    POWERUP_HEALTH,
    POWERUP_COIN,
    POWERUP_SLOW,
    POWERUP_MEGASHOT
}powerup_t;

void powerups_init (void);
void powerup_add (powerup_t type, int xpos, int ypos);
void powerups_draw (void);
void powerup_smartbomb (void);
void powerups_textures_init (void);
extern int megashot_active;

void powerup_megashot_disable (void);

