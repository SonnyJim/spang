#include "spang.h"

const char *hsentry_letters = " ABCDEFGHIJKLMNOPQRSTUVWXYZ";


int hsentry_index;
int hsentry_letter;

int hsentry_initted = 0;
int hsentry_playback = 0;

static void hsentry_update_letter (void)
{
    hiscores[hiscore_position].initials[hsentry_index] = hsentry_letters[hsentry_letter];
    Mix_PlayChannel (-1, comboup, 0);
}

void hsentry_fire (void)
{
    if (input_keyrepeat () || hsentry_playback)
        return;
    Mix_PlayChannel (SND_LASER, speedup, 0);
    if (hsentry_index < 2)
        hsentry_index++;
    else
    {
        hsentry_update_letter ();
        if (strcmp ("   ", hiscores[hiscore_position].initials) == 0)
            strcpy (hiscores[hiscore_position].initials, "FEK");
        hsentry_initted = 0;
        network_send_score (hiscores[hiscore_position].initials, hiscores[hiscore_position].score);
        gamestate = GAME_AMODE;
        return;
    }
    hsentry_update_letter();
}

void hsentry_right (void)
{
    if (input_keyrepeat ())
        return;

    if (hsentry_letter < strlen (hsentry_letters) - 1)
        hsentry_letter++;
    else
        hsentry_letter = 0;
    hsentry_update_letter ();
}

void hsentry_left (void)
{
    if (input_keyrepeat ())
        return;

    if (hsentry_letter > 0)
        hsentry_letter--;
    else
        hsentry_letter = strlen(hsentry_letters) - 1;
    hsentry_update_letter ();
}

static void hsentry_init (void)
{
    hsentry_index = 0;
    hsentry_letter = 0;
    strcpy (hiscores[hiscore_position].initials, "   ");
    hsentry_initted = 1;
}

void hsentry_loop (void)
{
    if (!hsentry_initted)
        hsentry_init ();
    amode_draw ();
}
