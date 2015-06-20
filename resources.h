int music_init (void);
int fonts_init (void);
int audio_init (void);

#define NUM_MUSIC 5

typedef enum
{
    MUSIC_MENU,
    MUSIC_T2K,
    MUSIC_BUBBLEBOBBLE,
    MUSIC_CYBERRID

} music_t;

extern Mix_Music *music[NUM_MUSIC];

extern Mix_Chunk *laser1;
extern Mix_Chunk *explosion;
extern Mix_Chunk *playerhit;
extern Mix_Chunk *death;
extern Mix_Chunk *comboup;
extern Mix_Chunk *combodown;
extern Mix_Chunk *levelup;
extern Mix_Chunk *speedup;
extern Mix_Chunk *alarm;
extern Mix_Chunk *health1;
extern Mix_Chunk *kaching;
extern Mix_Chunk *slowdown;
extern Mix_Chunk *alarmreverse;
extern Mix_Chunk *tink;
extern Mix_Chunk *siren;
extern Mix_Chunk *snd_barrel;

extern SDL_Texture *logo_tex;
