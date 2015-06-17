#include "spang.h"

void amode_fire (void)
{
    if (!input_keyrepeat())
        game_start ();
}

void amode_draw (void)
{
    render_string_centre ("SPANG", 0, green, font2);
    render_string_centre ("By Sonny Jim", 45, green, font3);
    if (gamestate == GAME_AMODE)
        render_string_centre ("Press Fire to start or Space for Config", screen_height - 100, green, font3);
    else if (gamestate == GAME_HSENTRY)
        render_string_centre ("Enter initials", screen_height - 100, green, font3);
    hiscore_draw ();
    stars_draw ();
}

void amode_loop (void)
{
    amode_draw ();
}
