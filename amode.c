#include "spang.h"
const char* scroller_text =
"\
Welcome to Spang! by Sonny_Jim.  Controls:  Left/Right arrow keys to move, Ctrl to fire, Space for Smartbomb, \
P to pause, Escape to quit,  M to toggle music.                  \
Visit pinball.servebeer.com/spang.html for the online leaderboard.      \
Written in C using the SDL2 library.  Starfield FX based on code by W.P. van Paassen from the SDL2 demo effects collection. \
Music by Volker Tripp and Sonny_Jim.  Game assets from OpenGameArt.org and openfontlibrary.org                       \
";

SDL_Texture *scroller_tex = NULL;
SDL_Surface *scroller_srf = NULL;

struct scroller_t
{
    int index;
    int start;
    int len;
    int charwidth; //width of a single character
    int xwidth; //Width of the screen in characters
    SDL_Rect rect;
    TTF_Font *font;
    char *msg;
    Uint32 delay;
};

struct scroller_t scroller = {.msg = NULL, .charwidth = 0, .index = 0, .len = 0, .font = NULL};

static void amode_scroller_init (void)
{
    Mix_PlayMusic (music[MUSIC_MENU], -1);
    if (scroller.font == NULL)
        scroller.font = font4;
    scroller.index = 0;
    scroller.start = 2;
    //TTF_SizeText (scroller.font, " ", &scroller.rect.w, &scroller.rect.h);
    scroller.rect.w = 20;
    scroller.rect.h = 20;
    scroller.charwidth = scroller.rect.w;
    scroller.len = strlen (scroller_text);
    scroller.rect.x = 0;
    scroller.rect.y = screen_height - scroller.rect.h;
    scroller.rect.w = screen_width;
    scroller.xwidth = (screen_width / scroller.charwidth);
    if (scroller.msg == NULL)
        scroller.msg = malloc (sizeof (scroller.xwidth));
}

static void amode_scroller_build (void)
{
        int i, j;

        if (scroller.start == 1)
        {
            strncpy (scroller.msg, scroller_text, screen_width / scroller.charwidth);
            scroller.msg[screen_width / scroller.charwidth] = '\0';
            scroller.start = 0;
            return;
        }
	size_t scroller_size = sizeof(scroller.msg);
        memset (scroller.msg, 0, scroller_size);
        j = scroller.index;
        for (i = 0; i < scroller.xwidth; i++)
        {
            scroller.msg[i] = scroller_text[j];
            if (++j >= scroller.len)
                j = 0;

        }
        scroller.msg[scroller.xwidth] = '\0';
}

static void amode_scroller_draw (void)
{
    if (scroller.charwidth == 0)
        amode_scroller_init ();

    if (scroller.start == 2)
    {
        scroller.rect.x = screen_width;
        scroller.start = 1;
    }

    scroller.rect.x -= 3;
    if (scroller.rect.x + scroller.charwidth <= 0)
    {
        scroller.index++;
        if (scroller.index >= scroller.len)
            scroller.index = 0;
        scroller.rect.x = 0;
    }
    amode_scroller_build ();



    SDL_FreeSurface (scroller_srf);
    scroller_srf = TTF_RenderText_Solid (scroller.font, scroller.msg, green);
    SDL_DestroyTexture (scroller_tex);
    scroller_tex = SDL_CreateTextureFromSurface (renderer, scroller_srf);
    SDL_RenderCopy (renderer, scroller_tex, NULL, &scroller.rect);
    scroller.delay = frame_counter;
}

void amode_fire (void)
{
    if (input_keyrepeat())
        return;

    if (record_state == REC_PLAY)
        playback_select ();
    else
        game_start ();
}

void amode_draw (void)
{

    SDL_Rect logo_rect;
    logo_rect.w = 395;
    logo_rect.h = 70;
    logo_rect.x = (screen_width / 2) - (logo_rect.w / 2);
    logo_rect.y = 5;
    SDL_RenderCopy (renderer, logo_tex, NULL, &logo_rect);
    //render_string_centre ("SPANG", 0, green, font2);
    //render_string_centre ("By Sonny Jim", 70, green, font3);

    if (gamestate == GAME_AMODE)
    {
        render_string_centre ("Press Fire to start or Space for Config", screen_height - 100, green, font3);
        amode_scroller_draw ();
    }
    else if (gamestate == GAME_HSENTRY)
        render_string_centre ("Enter initials", screen_height - 100, green, font3);

    hiscore_draw ();

    stars_draw ();
}

void amode_loop (void)
{
    amode_draw ();
}
