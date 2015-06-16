#include "spang.h"

#define NUM_TEXTURE_PACKS 3
#define NUM_TEXTURES 9

SDL_Texture *ball_tex = NULL;
SDL_Texture *bullet_tex = NULL;
SDL_Texture *bg_tex = NULL;
SDL_Texture *ship_tex;
SDL_Texture *balloon_tex = NULL;

SDL_Texture *health_tex = NULL;
SDL_Texture *coin_tex = NULL;
SDL_Texture *slow_tex = NULL;
SDL_Texture *megashot_tex = NULL;

SDL_Texture *enemy_tex = NULL;

Mix_Chunk *laser1 = NULL;
Mix_Chunk *explosion = NULL;
Mix_Chunk *playerhit = NULL;
Mix_Chunk *death = NULL;
Mix_Chunk *comboup = NULL;
Mix_Chunk *combodown = NULL;
Mix_Chunk *test = NULL;
Mix_Chunk *tink = NULL;
Mix_Chunk *siren = NULL;

Mix_Chunk *snd_barrel = NULL;

Mix_Music *music[NUM_MUSIC];

TTF_Font *font1 = NULL;
TTF_Font *font2 = NULL;
TTF_Font *font3 = NULL;

int texture_error = 0;

struct texture_pack
{
    char *ball;
    char *bullet;
    char *bg;
    char *ship;
    char *health;
    char *coin;
    char *slow;
    char *megashot;
    char *enemy;
};

struct texture_pack texture_default =
{
    .ball = "data/gfx/default/asteroid1.png",
    .bullet = "data/gfx/default/bullet1.png",
    .bg = "data/gfx/default/bg.png",
    .ship = "data/gfx/default/ship.png",
    .health = "data/gfx/default/health.png",
    .coin = "data/gfx/default/coin.png",
    .slow = "data/gfx/default/slow.png",
    .megashot = "data/gfx/default/megashot.png",
    .enemy = "data/gfx/default/enemy.png"
};

struct texture_pack texture_bublbobl =
{
    .ball = "data/gfx/bublbobl/asteroid1.png",
    .bullet = "data/gfx/bublbobl/bullet1.png",
    .bg = "data/gfx/default/bg.png",
    .ship = "data/gfx/bublbobl/ship.png",
    .health = "data/gfx/bublbobl/health.png",
    .coin = "data/gfx/bublbobl/coin.png",
    .slow = "data/gfx/bublbobl/slow.png",
    .megashot = "data/gfx/bublbobl/megashot.png",
    .enemy = "data/gfx/default/enemy.png"

};

struct texture_pack texture_pacman =
{
    .ball = "data/gfx/pacman/asteroid1.png",
    .bullet = "data/gfx/pacman/bullet1.png",
    .bg = "data/gfx/default/bg.png",
    .ship = "data/gfx/pacman/ship.png",
    .health = "data/gfx/pacman/health.png",
    .coin = "data/gfx/pacman/coin.png",
    .slow = "data/gfx/pacman/slow.png",
    .megashot = "data/gfx/pacman/megashot.png",
    .enemy = "data/gfx/default/enemy.png"

};

struct texture_pack texture_packs[NUM_TEXTURE_PACKS];
SDL_Texture *textures[NUM_TEXTURES];

static SDL_Texture* texture_load_img (char *file)
{
    SDL_Texture *texture;
    SDL_Surface *temp_srf = IMG_Load (file);
    if (temp_srf == NULL)
    {
        fprintf (stderr, "Error loading file %s: %s\n", file, SDL_GetError ());
        texture_error = 1;
    }

    texture = SDL_CreateTextureFromSurface (renderer, temp_srf);
    if (texture == NULL)
    {
        fprintf (stderr, "Error creating texture\n");
        texture_error = 1;
    }
    SDL_FreeSurface (temp_srf);
    return texture;
}

int textures_load (int num)
{
    int i;
    texture_error = 0;

    for (i = 0; i < NUM_TEXTURES; i++)
        SDL_DestroyTexture(textures[i]);

    ball_tex = texture_load_img (texture_packs[num].ball);
    bullet_tex = texture_load_img (texture_packs[num].bullet);
    bg_tex = texture_load_img (texture_packs[num].bg);
    ship_tex = texture_load_img (texture_packs[num].ship);
    health_tex = texture_load_img (texture_packs[num].health);
    coin_tex = texture_load_img (texture_packs[num].coin);
    slow_tex = texture_load_img (texture_packs[num].slow);
    megashot_tex = texture_load_img (texture_packs[num].megashot);
    enemy_tex = texture_load_img (texture_packs[num].enemy);

    bonus_tex = texture_load_img ("data/gfx/default/barrel.png");
    balloon_tex = texture_load_img ("data/gfx/default/ball.png");

    //Need to call this to update the pointers to powerup textures
    powerups_textures_init ();

    if (texture_error)
        fprintf (stderr, "Error:  The was an error creating the textures!\n");
    return texture_error;
}

int textures_init (void)
{
    texture_packs[0] = texture_default;
    texture_packs[1] = texture_bublbobl;
    texture_packs[2] = texture_pacman;

    textures[0] = ball_tex;
    textures[1] = bullet_tex;
    textures[2] = bg_tex;
    textures[3] = ship_tex;
    textures[4] = health_tex;
    textures[5] = coin_tex;
    textures[6] = slow_tex;
    textures[7] = megashot_tex;
    textures[8] = enemy_tex;
    return textures_load (0);
}

int fonts_init (void)
{
    font1 = TTF_OpenFont( "data/fonts/8bitwonder.ttf", 20 );
    font2 = TTF_OpenFont( "data/fonts/8bitwonder.ttf", 40 );
    font3 = TTF_OpenFont( "data/fonts/8bitwonder.ttf", 12 );
    if (font1 == NULL)
    {
        fprintf (stderr, "Error loading font1\n");
        return 1;
    }
    //score_rect.x = 0;
    //score_rect.y = 0;
    return 0;
}

static int music_load (char *file, music_t num)
{
    if (num > NUM_MUSIC)
    {
        fprintf (stderr, "Error: music_load out of range %i\n", num);
        return 1;
    }
    music[num] = NULL;
    music[num] = Mix_LoadMUS (file);
    if (music[num] == NULL)
    {
        fprintf (stderr, "Error: Mix_LoadMUS error %s\n", Mix_GetError ());
        return 1;
    }
    return 0;
}

int music_init (void)
{
    int ret;
    ret = music_load ("data/sfx/music/t2kremix.mod", MUSIC_T2K);
    ret = music_load ("data/sfx/music/bubble_bobble_tribute.xm", MUSIC_BUBBLEBOBBLE);
    ret = music_load ("data/sfx/music/cyberrid.mod", MUSIC_CYBERRID);
    ret = music_load ("data/sfx/music/pacman.mod", MUSIC_PACMAN);

    return ret;
}

int audio_init (void)
{
    laser1 = Mix_LoadWAV("data/sfx/laser1.wav");
    if (laser1 == NULL)
        return 1;
    explosion = Mix_LoadWAV ("data/sfx/explosion1.wav");
    if (explosion == NULL)
        return 1;
    playerhit = Mix_LoadWAV ("data/sfx/hit.wav");
    if (playerhit == NULL)
        return 1;
    death = Mix_LoadWAV ("data/sfx/death.wav");
    if (death == NULL)
        return 1;
    comboup = Mix_LoadWAV ("data/sfx/comboup.wav");
    if (comboup == NULL)
        return 1;
    combodown = Mix_LoadWAV ("data/sfx/combodown.wav");
    if (combodown == NULL)
        return 1;
    levelup = Mix_LoadWAV ("data/sfx/levelup.wav");
    if (levelup == NULL)
        return 1;
    speedup = Mix_LoadWAV ("data/sfx/speedup.wav");
    if (speedup == NULL)
        return 1;
    alarm = Mix_LoadWAV ("data/sfx/alarm1.wav");
    if (alarm == NULL)
        return 1;
    health1 = Mix_LoadWAV ("data/sfx/health.wav");
    if (health1 == NULL)
        return 1;
    kaching = Mix_LoadWAV ("data/sfx/kaching.wav");
    if (kaching == NULL)
        return 1;
    slowdown = Mix_LoadWAV ("data/sfx/slowdown.wav");
    if (slowdown == NULL)
        return 1;
    alarmreverse = Mix_LoadWAV ("data/sfx/alarmreverse.wav");
    if (alarmreverse == NULL)
        return 1;
    tink = Mix_LoadWAV ("data/sfx/tink.wav");
    if (tink == NULL)
        return 1;
    siren = Mix_LoadWAV ("data/sfx/siren.wav");
    if (siren == NULL)
        return 1;
    snd_barrel = Mix_LoadWAV ("data/sfx/barrel.wav");
    if (snd_barrel == NULL)
        return 1;
    return 0;
}
