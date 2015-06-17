#define NUM_HISCORES 1024
#define HISCORE_FILE "spang_scores.data"
#define HTML_FILE "/var/www/spang.html"

#include <stdio.h>
#include <string.h>

extern int running;
int score_new = 0;

struct score_t
{
    char initials[3];
    long score;
};

struct score_t scores[NUM_HISCORES];

static void scores_generate_html (void)
{
    FILE *fp;
    char buffer[1024];
    int i;
    fp = fopen (HTML_FILE, "w");
    if (fp == NULL)
    {
        fprintf (stderr, "Error opening HTML file for writing\n");
        return;
    }
    
    //Generate header
    fputs ("<!DOCTYPE html>\n", fp);
    fputs ("<title>Spang! Hiscores</title>\n", fp);
    fputs ("<html><body>", fp);
    for (i = 0; i < NUM_HISCORES; i++)
    {
        if (scores[i].score > 0)
        {
            if (i == 0)
            {
                fputs ( "==============<br>", fp);
                fputs ( "Grand Champion<br>", fp);
                fputs ( "==============<br>", fp);

                sprintf (buffer, "%s %li<br><br>", scores[i].initials, scores[i].score);
                fputs (buffer, fp);
            }
            else
            {
                sprintf (buffer, "%i: %s %li<br>", i, scores[i].initials, scores[i].score);
                fputs (buffer, fp);
            }
        }
    }

    //Generate footer
    fputs ("</body></html>", fp);
    fclose (fp);
}

static void scores_init (void)
{
    int i;
    
    fprintf (stdout, "Initialising hiscore table\n");
    for (i = 0; i < NUM_HISCORES; i++)
    {
        scores[i].score = 0;
        strncpy (scores[i].initials, "", 3);
    }
}

static void scores_load (void)
{
    FILE *fp;

    fp = fopen (HISCORE_FILE, "r");
    if (fp == NULL)
    {
        fprintf (stderr, "Error opening hiscore file for writing\n");
        return;
    }
    
    fread (&scores, sizeof(scores), 1, fp);
    fprintf (stdout, "Scores loaded\n");
    fclose (fp);
}

static void scores_save (void)
{
    FILE *fp;

    fp = fopen (HISCORE_FILE, "w");
    if (fp == NULL)
    {
        fprintf (stderr, "Error opening hiscore file for reading\n");
        return;
    }
    
    fwrite (&scores, sizeof(scores), 1, fp);
    fprintf (stdout, "Scores saved\n");
    fclose (fp);
}

static void scores_print (void)
{
    int i;

    for (i = 0; i < NUM_HISCORES; i++)
    {
        if (scores[i].score > 0)
        {
            if (i == 0)
            {
                fprintf (stdout, "==============\n");
                fprintf (stdout, "Grand Champion\n");
                fprintf (stdout, "==============\n");
                fprintf (stdout, "%s %li\n\n", scores[i].initials, scores[i].score);
            }
            else
                fprintf (stdout, "%i, %s %li\n", i, scores[i].initials, scores[i].score);
        }
    }
}

void scores_write (void *ptr)
{
    scores_init ();
    scores_load ();
    scores_generate_html ();
    while (running)
    {
        if (score_new)
        {
            fprintf (stdout, "New score received\n");
            scores_print ();
            score_new = 0;
        }
    }
}

static void score_added (void)
{
    scores_save ();
    scores_print ();
    score_new = 1;
    scores_generate_html ();
}

void score_receive (char* initials, long score)
{
    int i, j;
    int count;
    
    count = 0;

    //Only allow 10 high scores per player
    for (i = 0; i < NUM_HISCORES; i++)
    {
        if (strncmp (scores[i].initials, initials, 3) == 0)
                count++;
        if (count == 10)
        {
            if (scores[i].score >= score)
            {
                fprintf (stdout, "Not inserting score, lower that previous 20 scores\n");
                return;
            }
            else
                break;
        }
    }

    for (i = 0; i < NUM_HISCORES; i++)
    {
        if (score > scores[i].score)
        {
            fprintf (stdout, "Inserting %s at %i with %li\n", initials, i, score);
            for (j = NUM_HISCORES; j > i; j--)
            {
                scores[j].score = scores[j - 1].score;
                strncpy (scores[j].initials, scores[j-1].initials, 3);
            }
            scores[i].score = score;
            strncpy (scores[i].initials, initials, 3);
            score_added ();
            return;
        }
    }
    fprintf (stderr, "Error: score_receive got to the end before adding a score?\n");
}
