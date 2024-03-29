#include "spang.h"
#include <string.h>

#define NUM_MSGS 32

SDL_Color red = {255, 0, 0};
SDL_Color green = {0, 255, 0};
SDL_Color blue = {0, 0, 255};
SDL_Color black = {0, 0, 0};
SDL_Color white = {255, 255, 255};

//TODO Could probably optimise the hell out of this as I'm creating and destroying textures each frame

struct msg_t
{
    Uint32 start_time;
    int show_time;
    char text[1024];
    SDL_Rect rect;
    SDL_Color colour;
    int active;
    TTF_Font *font;
    align_t align;
};

struct msg_t msgs[NUM_MSGS];

SDL_Surface *score_srf = NULL;
SDL_Texture *score_tex = NULL;
SDL_Surface *level_srf = NULL;
SDL_Texture *level_tex = NULL;

SDL_Rect score_rect;
SDL_Rect level_rect;

int TextWidth, TextHeight = 0;


void render_string (char *msg, int xpos, int ypos, SDL_Color colour, TTF_Font *font)
{
    SDL_Surface *srf = NULL;
    SDL_Texture *tex = NULL;
    SDL_Rect rect;

    srf = TTF_RenderText_Blended ( font, msg, colour);
    tex = SDL_CreateTextureFromSurface (renderer, srf);
    SDL_FreeSurface (srf);

    rect.x = xpos;
    rect.y = ypos;

    SDL_QueryTexture (tex, NULL, NULL, &rect.w, &rect.h);
    SDL_RenderCopy (renderer, tex, NULL, &rect);
    SDL_DestroyTexture (tex);

}

static void render_string_tcentre (char *msg, int xpos, int ypos, SDL_Color colour, TTF_Font *font)
{
    score_srf = TTF_RenderText_Blended ( font, msg, colour);
    score_tex = SDL_CreateTextureFromSurface (renderer, score_srf);
    SDL_FreeSurface (score_srf);
    TTF_SizeText (font, msg, &TextWidth, &TextHeight);
    score_rect.x = xpos - (TextWidth / 2);
    score_rect.y = ypos - (TextHeight / 2);
    SDL_QueryTexture (score_tex, NULL, NULL, &score_rect.w, &score_rect.h);
    SDL_RenderCopy (renderer, score_tex, NULL, &score_rect);
    SDL_DestroyTexture (score_tex);
}

void render_string_centre (char *msg, int ypos, SDL_Color colour, TTF_Font *font)
{
    score_srf = TTF_RenderText_Blended ( font, msg, colour);
    score_tex = SDL_CreateTextureFromSurface (renderer, score_srf);
    SDL_FreeSurface (score_srf);
    TTF_SizeText (font, msg, &TextWidth, &TextHeight);
    score_rect.x = (screen_width / 2 ) - (TextWidth / 2);
    score_rect.y = ypos;
    SDL_QueryTexture (score_tex, NULL, NULL, &score_rect.w, &score_rect.h);
    SDL_RenderCopy (renderer, score_tex, NULL, &score_rect);
    SDL_DestroyTexture (score_tex);
}

static void render_string_right (char *msg, int ypos, SDL_Color colour, TTF_Font *font)
{
    score_srf = TTF_RenderText_Blended ( font, msg, colour);
    score_tex = SDL_CreateTextureFromSurface (renderer, score_srf);
    SDL_FreeSurface (score_srf);
    TTF_SizeText (font1, msg, &TextWidth, &TextHeight);
    score_rect.x = screen_width - TextWidth;
    score_rect.y = ypos;
    SDL_QueryTexture (score_tex, NULL, NULL, &score_rect.w, &score_rect.h);
    SDL_RenderCopy (renderer, score_tex, NULL, &score_rect);
    SDL_DestroyTexture (score_tex);
}

int render_score (void)
{
    char score_str[1024] = "";
    sprintf (score_str, "Shields %i", player.health);
    if (player.health > 20)
        render_string (score_str, 0, 0, green, font1);
    else
        render_string (score_str, 0, 0, red, font1);

    sprintf (score_str, "%li", player.score);
    render_string_centre (score_str, 0, green, font1);
    if (player.smartbomb)
    {
        sprintf (score_str, "Supertapper");
        render_string (score_str, 0, TextHeight, green, font1);
    }
    sprintf (score_str, "Laser %i", player.combo_level + 1);
    if (megashot_active)
        render_string_right (score_str, 0, red, font1);
    else
        render_string_right (score_str, 0, green, font1);
    if (gamestate != GAME_BONUS)
        sprintf (score_str, "Level %i", player.level);    else       sprintf (score_str, "Bonus %i", player.bonus_level);
    render_string_right (score_str, 25, green, font1);


    sprintf (score_str, "( %i )", score_bonus);
    if (score_bonus == 800)
        render_string_centre (score_str, TextHeight, red, font3);
    else
        render_string_centre (score_str, TextHeight, green, font3);

    if (paused)
        render_string_right ("Paused", 50, red, font1);
    else if (record_state == REC_REC)
        render_string_right ("Record", 50, red, font1);
    else if (record_state == REC_PLAYING)
        render_string_right ("Playback", 50, green, font1);
    return 0;
}

void msg_init (void)
{
    int i;
    for (i = 0; i < NUM_MSGS; i++)
    {
        msgs[i].start_time = 0;
        msgs[i].show_time = 0;
        msgs[i].rect.x = 0;
        msgs[i].rect.y = 0;
        msgs[i].font = NULL;
        msgs[i].active = 0;
        strcpy (msgs[i].text, "");
        msgs[i].align = ALIGN_NONE;
    }
}

void msg_show (char *msg, int xpos, int ypos, int time, TTF_Font *font, align_t align, SDL_Colour colour)
{
    int i;
    for (i = 0; i < NUM_MSGS; i++)
    {
        if (msgs[i].active == 0)
        {
            msgs[i].start_time = SDL_GetTicks ();
            msgs[i].show_time = time;
            msgs[i].rect.x = xpos;
            msgs[i].rect.y = ypos;
            msgs[i].font = font;
            msgs[i].active = 1;
            msgs[i].align = align;
            msgs[i].colour = colour;
            sprintf (msgs[i].text, "%s", msg);
            TTF_SizeText (msgs[i].font, msgs[i].text, &msgs[i].rect.w, &msgs[i].rect.h);

            return;
        }
    }
}

void msg_level_up (void)
{
    char buffer[32];
    sprintf (buffer, "Level %i", player.level);
    msg_show (buffer, 0, 60 , 3, font2, ALIGN_CENTRE, green);
}

static void msg_draw_aligned (int i)
{
    switch (msgs[i].align)
    {
        case ALIGN_NONE:
            render_string (msgs[i].text, msgs[i].rect.x, msgs[i].rect.y, msgs[i].colour, msgs[i].font);
            break;
        case ALIGN_CENTRE:
            render_string_centre (msgs[i].text, msgs[i].rect.y, msgs[i].colour, msgs[i].font);
            break;
        case ALIGN_RIGHT:
            render_string_right (msgs[i].text, msgs[i].rect.y, msgs[i].colour, msgs[i].font);
            break;
        case ALIGN_TCENTRE:
            render_string_tcentre (msgs[i].text, msgs[i].rect.x, msgs[i].rect.y, msgs[i].colour, msgs[i].font);
    }
}

static void msg_check_overlap (int num)
{
    int i;

    for (i = 0; i < NUM_MSGS; i++)
    {
        if (msgs[i].active && i != num)
        {
            if (check_axis (msgs[i].rect, msgs[num].rect))
                msgs[num].rect.y -= msgs[i].rect.h;
        }
    }
}

void msg_draw (void)
{
    int i;
    for (i = 0; i < NUM_MSGS; i++)
    {
        if (msgs[i].active)
        {
            if (msgs[i].start_time + (msgs[i].show_time * 1000) > SDL_GetTicks ())
            {
                msg_check_overlap (i);
                msg_draw_aligned (i);
            }
            else
                msgs[i].active = 0;
        }
    }
}
