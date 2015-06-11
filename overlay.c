#include "spang.h"
TTF_Font *font1 = NULL;
SDL_Color green = { 0, 255, 0 };
SDL_Color black = {0,0,0};
SDL_Color red = {255, 0, 0};

Uint32 level_show_time = 0;

SDL_Surface *score_srf = NULL;
SDL_Texture *score_tex = NULL;
SDL_Surface *level_srf = NULL;
SDL_Texture *level_tex = NULL;

SDL_Rect score_rect;
SDL_Rect level_rect;

int TextWidth, TextHeight = 0;

int fonts_load (void)
{
    font1 = TTF_OpenFont( "data/fonts/8bitwonder.ttf", 20 );

    if (font1 == NULL)
    {
        fprintf (stderr, "Error loading font1\n");
        return 1;
    }
    score_rect.x = 0;
    score_rect.y = 0;
    return 0;
}

void show_level_up (void)
{
    if (level_show_time == 0)
    {
        char level_str[64] = "";
        sprintf (level_str, "LEVEL %i", player.level);
        level_srf = TTF_RenderText_Blended ( font1, level_str, green);
        if (level_srf == NULL)
        {
            fprintf (stderr, "Error rendering score text to surface\n");
            return;
        }

        level_tex = SDL_CreateTextureFromSurface (renderer, level_srf);
        if (level_tex == NULL)
        {
            fprintf (stderr, "Error creating score texture\n");
            return;
        }
        SDL_QueryTexture (score_tex, NULL, NULL, &level_rect.w, &level_rect.h);
        level_rect.x = (screen_width / 2) - (level_rect.w / 2);
        level_rect.y = screen_height / 2;
        level_show_time = SDL_GetTicks ();
    }
    SDL_RenderCopy (renderer, score_tex, NULL, &level_rect);
}

static void render_string (char *msg, int xpos, int ypos)
{
    score_srf = TTF_RenderText_Blended ( font1, msg, green);
    score_tex = SDL_CreateTextureFromSurface (renderer, score_srf);
    score_rect.x = xpos;
    score_rect.y = ypos;
    SDL_QueryTexture (score_tex, NULL, NULL, &score_rect.w, &score_rect.h);
    SDL_RenderCopy (renderer, score_tex, NULL, &score_rect);
}

static void render_string_centre (char *msg, int ypos, SDL_Color colour)
{
    score_srf = TTF_RenderText_Blended ( font1, msg, colour);
    score_tex = SDL_CreateTextureFromSurface (renderer, score_srf);
    TTF_SizeText (font1, msg, &TextWidth, &TextHeight);
    score_rect.x = (screen_width / 2 ) - (TextWidth / 2);
    score_rect.y = ypos;
    SDL_QueryTexture (score_tex, NULL, NULL, &score_rect.w, &score_rect.h);
    SDL_RenderCopy (renderer, score_tex, NULL, &score_rect);
}

static void render_string_right (char *msg, int ypos)
{
    if (megashot_active)
        score_srf = TTF_RenderText_Blended ( font1, msg, red);
    else
        score_srf = TTF_RenderText_Blended ( font1, msg, green);
    score_tex = SDL_CreateTextureFromSurface (renderer, score_srf);
    TTF_SizeText (font1, msg, &TextWidth, &TextHeight);
    score_rect.x = screen_width - TextWidth;
    score_rect.y = ypos;
    SDL_QueryTexture (score_tex, NULL, NULL, &score_rect.w, &score_rect.h);
    SDL_RenderCopy (renderer, score_tex, NULL, &score_rect);
}

int render_score (void)
{
    char score_str[1024] = "";
    sprintf (score_str, "Health %i", player.health);
    render_string (score_str, 0, 0);
    sprintf (score_str, "%li", player.score);
    render_string_centre (score_str, 0, green);
    sprintf (score_str, "Laser %i", player.combo_level + 1);
    render_string_right (score_str, 0);
    sprintf (score_str, "( %i )", score_bonus);
    if (score_bonus == 800)
        render_string_centre (score_str, TextHeight, red);
    else
        render_string_centre (score_str, TextHeight, green);
    return 0;
}
