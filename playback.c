#include "spang.h"
#include <stdio.h>
#include <sys/types.h>
#include <dirent.h>

#define MAX_NUM_FILES 1024

int num_files;
int file_index;
char **filelist = NULL;

static int playback_readdir (void)
{
    DIR *dp;
    struct dirent *ep;
    int max_strlen = 0;
    int i;

    dp = opendir ("./recordings");

    num_files = 0;
    if (dp != NULL)
    {
        while (ep = readdir (dp))
        {
            if (strncmp (".rec", ep->d_name + strlen(ep->d_name) - 4, 4) == 0)
            {
                if (strlen (ep->d_name) > max_strlen)
                    max_strlen = strlen (ep->d_name);
                num_files++;
            }
        }

        if (num_files == 0)
        {
            fprintf (stderr, "Didn't find any .rec files in recordings\n");
            return 1;
        }
        //Dynamically size filelist array
        if (filelist == NULL)
            filelist = malloc (sizeof (char) * num_files);

        if (filelist == NULL)
        {
            fprintf (stderr, "Error setting up filelist\n");
            return 2;
        }

        for (i = 0; i < num_files; i++)
        {

            filelist[i] = malloc (sizeof (char) * max_strlen);

            if (filelist[i] == NULL)
            {
                fprintf (stderr, "Error setting up filelist\n");
                return 2;
            }
        }

        rewinddir (dp);

        i = 0;
        while (ep = readdir (dp))
        {

            if (strncmp (".rec", ep->d_name + strlen(ep->d_name) - 4, 4) == 0)
            {
                strcpy (filelist[i], ep->d_name);
                i++;
            }
        }
        closedir (dp);
    }
    else
    {
        fprintf (stderr, "Couldn't open the directory\n");
        return 1;
    }
    return 0;
}

static void playback_free (void)
{
    int i;
/*
    for (i = 0; i < num_files; i++)
        free (filelist[i]);
    free (filelist);
    */
}

static void playback_init (void)
{
    if (playback_readdir ())
    {
        record_state = REC_STOP;
        gamestate = GAME_AMODE;
    }
}

void playback_quit (void)
{
    gamestate = GAME_AMODE;
    playback_free ();
}

static void playback_start (char *file)
{
    char buffer[1024];

    sprintf (buffer, "recordings/%s", file);
    playback_free ();
    record_load (buffer);
    frame_counter = 0;
    rec_buffer_counter = 0;
    record_state = REC_PLAYING;
    input_mask = 0;
    fprintf (stdout, "Starting playback\n");
    gamestate = GAME_RUNNING;
    game_start ();
}

void playback_select (void)
{
    playback_init ();
    gamestate = GAME_SELECT_RECORD;
}

void playback_stop (void)
{
    fprintf (stdout, "Playback stopped\n");
    record_state = REC_STOP;
    gamestate = GAME_OVER;
    hsentry_playback = 1;
}

void playback_select_start (void)
{
    playback_init ();
}

void playback_input_left (void)
{
    if (input_keyrepeat())
        return;
    if (file_index < num_files - 1)
        file_index++;
    else
        file_index = 0;
}
void playback_input_right (void)
{
    if (input_keyrepeat())
        return;
    if (file_index > 0)
        file_index--;
    else
        file_index = num_files - 1;
}

void playback_input_fire (void)
{
    if (input_keyrepeat () || num_files <= 0)
        return;
    playback_start (filelist[file_index]);
}

void playback_loop (void)
{
    char buffer[1024];
    if (num_files <= 0)
    {
        if (level_change_timer == 0)
            level_change_timer = 60;
        render_string_centre ("No files found", screen_height /2, red, font4);
        level_change_timer--;
        if (level_change_timer == 0)
        {
            playback_stop ();
            gamestate = GAME_AMODE;
        }
        return;
    }
    if (file_index < 0)
        file_index = 0;
    render_string_centre ("Playback", 0, green, font2);
    sprintf (buffer, "Select file %s", filelist[file_index]);
    render_string_centre (buffer, screen_height /2, green, font4);
}
