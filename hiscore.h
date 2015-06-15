#define HISCORE_FILE "spang.hi"
#define NUM_HISCORES 10

struct hiscore_t
{
    long score;
    char initials[3];
};

struct hiscore_t hiscores[NUM_HISCORES];
extern int hiscore_position;

int hiscore_add (void);
void hiscore_draw (void);
void hiscore_init (void);
int hiscore_save (void);
int hiscore_check (void);
