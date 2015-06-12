#include "spang.h"
#define HISCORE_FILE "spang.hi"
long hiscore = 0;

int hiscore_load (void)
{
    FILE *fp;

    fp = fopen (HISCORE_FILE, "r");
    if (fp == NULL)
    {
        fprintf (stderr, "Error opening hiscore file\n");
        return 1;
    }
    fread (&hiscore, sizeof(hiscore), 1, fp);
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
    fwrite (&hiscore, sizeof(hiscore), 1, fp);
    fclose (fp);
    return 0;
}

int hiscore_check (void)
{
    if (player.score > hiscore)
    {
        hiscore = player.score;
        return 1;
    }
    return 0;
}

void hiscore_init (void)
{
    if (hiscore_load ())
    {
        fprintf (stderr, "Error opening hiscore file, creating a blank one\n");
        hiscore_save ();
    }
    fprintf (stdout, "hiscore: %ld\n", hiscore);
}
