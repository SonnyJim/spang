void config_left (void);
void config_right (void);
void config_fire (void);
void config_up (void);
void config_down (void);
void config_esc (void);

void config_loop (void);
int config_load (void);
void config_input_read (int mode, int value);

extern int explosion_type;
extern int bg_rotate;

extern Uint32 total_playtime;

struct joy_cfg_t
{
    int axis1;
    int axis2;
    int fire;
    int bomb;
    int pause;
};

struct joy_cfg_t joy_cfg;
struct joy_cfg_t joy_cfg_new;
