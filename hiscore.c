#include "spang.h"
int hiscore_position;
struct hiscore_t hiscores[NUM_HISCORES];

void hiscore_draw (void)
{
    int i;
    char buffer[128] = "";
    SDL_Color color;

    for (i = 0; i < NUM_HISCORES; i++)
    {
        if (gamestate == GAME_HSENTRY && i == hiscore_position)
            color = red;
        else
            color = green;
        render_string (hiscores[i].initials, (screen_width / 2) - 120, 120 + (i * 30), color, font1);

        sprintf (buffer, "%ld", hiscores[i].score);
        render_string (buffer, (screen_width / 2) + 10, 120 + (i * 30), color, font1);
    }
}

static int hiscore_load (void)
{
    FILE *fp;

    fp = fopen (HISCORE_FILE, "r");
    if (fp == NULL)
    {
        fprintf (stderr, "Error opening hiscore file\n");
        return 1;
    }
    fread (&hiscores, sizeof(hiscores), 1, fp);
    fclose (fp);
    return 0;
}

int hiscore_save (void)
{
    FILE *fp;
    fp = fopen (HISCORE_FILE, "w");
    if (fp == NULL)
    {
        fprintf (stderr, "Error saving hiscore file\n");
        return 1;
    }
    fwrite (&hiscores, sizeof(hiscores), 1, fp);
    fclose (fp);
    return 0;
}

static void hiscore_reset (void)
{
    int i;
    for (i = 0; i < NUM_HISCORES; i++)
    {
        strcpy (hiscores[i].initials, "FEK");
        hiscores[i].score = 10000;
    }
}

int hiscore_add (void)
{
    int i, j;
    for (i = 0; i < NUM_HISCORES; i++)
    {
        if (player.score > hiscores[i].score)
        {
            for (j = NUM_HISCORES; j > i; j--)
            {
                hiscores[j].score = hiscores[j - 1].score;
                strcpy (hiscores[j].initials, hiscores[j-1].initials);
            }
            hiscores[i].score = player.score;
            return i;
        }
    }
    fprintf (stderr, "Error: hiscore_add got to the end before adding a score?\n");
    return NUM_HISCORES + 1;
}

int hiscore_check (void)
{
    int i;
    for (i = 0; i < NUM_HISCORES; i++)
    {
        if (player.score > hiscores[i].score)
            return 1;
    }
    return 0;
}

void hiscore_init (void)
{
    if (hiscore_load ())
    {
        fprintf (stderr, "Error opening hiscore file, creating a blank one\n");
        hiscore_reset ();
        hiscore_save ();
    }
}
