#include "spang.h"
TTF_Font *font1 = NULL;
SDL_Color green = { 0, 255, 0 };
SDL_Color black = {0,0,0};

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

int render_score (void)
{
    char score_str[1024] = "";

    sprintf (score_str, "Health %i | Laser %i | Score %ld ", player.health, player.combo_level, player.score);
    score_srf = TTF_RenderText_Blended ( font1, score_str, green);
    if (score_srf == NULL)
    {
        fprintf (stderr, "Error rendering score text to surface\n");
        return 1;
    }

    score_tex = SDL_CreateTextureFromSurface (renderer, score_srf);
    if (score_tex == NULL)
    {
        fprintf (stderr, "Error creating score texture\n");
        return 1;
    }
    SDL_QueryTexture (score_tex, NULL, NULL, &score_rect.w, &score_rect.h);
    SDL_RenderCopy (renderer, score_tex, NULL, &score_rect);
    return 0;
}
