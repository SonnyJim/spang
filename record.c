#include "spang.h"
#define MAX_FRAMES 4096


int record_state = REC_STOP;

Uint32 frame_counter;
//Whereabouts in the record buffer we are
Uint32 rec_buffer_counter;
Uint8 old_input_mask;
Uint8 input_mask;

record_t record_buffer[MAX_FRAMES];

#define FRAMECHUNK (sizeof (Uint8) + sizeof (Uint32))
/*
int record_load_nozlib (char *file)
{
    FILE *fp;


    fp = fopen (file, "rb");
    if (fp == NULL)
    {
        fprintf (stderr, "Error opening %s for reading\n", file);
        gamestate = GAME_AMODE;
        return 1;
    }

    int i;
    for (i = 0; i < MAX_FRAMES; i++)
    {
        record_buffer[i].input = 0;
        record_buffer[i].frame = 0;
    }

    rec_buffer_counter = 0;
    record_buffer[rec_buffer_counter].input = REC_NONE;
    while (rec_buffer_counter < MAX_FRAMES)
    {
        fread (&record_buffer[rec_buffer_counter].input, sizeof(Uint8), 1, fp);
        fread (&record_buffer[rec_buffer_counter].frame, sizeof(Uint32), 1, fp);
        fprintf (stdout, "rec %i input %i frame %i\n", rec_buffer_counter, \
                 record_buffer[rec_buffer_counter].input, record_buffer[rec_buffer_counter].frame);

        if (record_buffer[rec_buffer_counter].input == REC_EOF)
            break;
        else if (record_buffer[rec_buffer_counter].input != REC_EOF && record_buffer[rec_buffer_counter].frame < rec_buffer_counter)
        {
            fprintf (stdout, "Something went wrong when loading the recording?\n");
            fclose (fp);
            return 0;
        }
        rec_buffer_counter++;
    }

    fprintf (stdout, "loaded %li frames\n", rec_buffer_counter);
    fclose (fp);
    return 0;
}

void record_save_nozlib (long score)
{
    FILE *fp;
    char filename[64];

    sprintf (filename, "recordings/%li.rec", score);

    fp = fopen (filename, "wb");
    if (fp == NULL)
    {
        fprintf (stderr, "Error opening record file for writing\n");
        return;
    }


    rec_buffer_counter = 0;
    while (record_buffer[rec_buffer_counter].input != REC_EOF)
    {
        fwrite (&record_buffer[rec_buffer_counter].input, sizeof(Uint8), 1, fp);
        fwrite (&record_buffer[rec_buffer_counter].frame, sizeof(Uint32), 1, fp);
        rec_buffer_counter++;
    }

    //Write the EOF
    record_buffer[rec_buffer_counter].input = REC_EOF;
    record_buffer[rec_buffer_counter].frame = rec_buffer_counter;
    fwrite (&record_buffer[rec_buffer_counter].input, sizeof (Uint8),1, fp);
    fwrite (&rec_buffer_counter, sizeof (Uint32),1, fp);

    fprintf (stdout, "Recorded %i frames to %s\n", rec_buffer_counter, filename);
    fclose (fp);
}
*/
void record_save (long score)
{
    char filename[64];
    char* buffer = NULL;
    int ret, filesize;

    sprintf (filename, "recordings/%li.rec", score);

    filesize = (FRAMECHUNK * rec_buffer_counter) + FRAMECHUNK;
    buffer = malloc (filesize);

    if (buffer == NULL)
    {
        fprintf (stderr, "Error:  Couldn't alloc buffer for record_save\n");
        return;
    }

    rec_buffer_counter = 0;
    while (record_buffer[rec_buffer_counter].input != REC_EOF)
    {
        //Copy record struct contents to buffer
        memcpy (buffer + (FRAMECHUNK * rec_buffer_counter), &record_buffer[rec_buffer_counter].input, sizeof(Uint8));
        memcpy (buffer + (FRAMECHUNK * rec_buffer_counter) + sizeof(Uint8), &record_buffer[rec_buffer_counter].frame, sizeof(Uint32));
        rec_buffer_counter++;
    }

    //Write the EOF to the buffer
    record_buffer[rec_buffer_counter].input = REC_EOF;
    record_buffer[rec_buffer_counter].frame = rec_buffer_counter;
    memcpy (buffer + (FRAMECHUNK * rec_buffer_counter), &record_buffer[rec_buffer_counter].input, sizeof(Uint8));
    memcpy (buffer + (FRAMECHUNK * rec_buffer_counter) + sizeof(Uint8), &rec_buffer_counter, sizeof(Uint32));

    ret = zlib_deflate_to_file (buffer, filesize, filename);
    if (ret == 0)
    {
        fprintf (stderr, "Error deflating record buffer to file\n");
    }
    fprintf (stdout, "Record Filesize: %li Returned: %i.  Wrote %li frames\n", filesize, ret, rec_buffer_counter);

    free (buffer);
}

int record_load (char *file)
{
    char* buffer = NULL;
    int ret;

    buffer = malloc (FRAMECHUNK * MAX_FRAMES);
    if (buffer == NULL)
    {
        fprintf (stdout, "Error mallocing memory for zlib buffer\n");
        return 1;
    }

    ret = zlib_inflate_from_file(file, buffer, FRAMECHUNK * MAX_FRAMES);
    fprintf (stdout, "Zlib inflate returned %i\n", ret);

    int i;
    for (i = 0; i < MAX_FRAMES; i++)
    {
        record_buffer[i].input = 0;
        record_buffer[i].frame = 0;
    }

    rec_buffer_counter = 0;
    record_buffer[rec_buffer_counter].input = REC_NONE;
    while (rec_buffer_counter < MAX_FRAMES)
    {
        memcpy (&record_buffer[rec_buffer_counter].input, buffer + (rec_buffer_counter * FRAMECHUNK), sizeof (Uint8));
        memcpy (&record_buffer[rec_buffer_counter].frame, buffer + (rec_buffer_counter * FRAMECHUNK)+ sizeof (Uint8), sizeof (Uint32));

        if (record_buffer[rec_buffer_counter].input == REC_EOF)
            break;
        if (record_buffer[rec_buffer_counter].frame < rec_buffer_counter)
        {
            fprintf (stdout, "Something went wrong when loading the recording?\n");
            free (buffer);
            return 1;
        }
        rec_buffer_counter++;

     }
    fprintf (stdout, "Loaded %i frames, last frame %i\n", rec_buffer_counter, record_buffer[rec_buffer_counter].frame);
    free (buffer);
    return 0;
}

void record_start (void)
{
    frame_counter = 0;
    rec_buffer_counter = 0;
    memset (record_buffer, 0, sizeof (record_buffer));
    record_state = REC_REC;
}
/*
static void playback_dump_frames (void)
{
    while (1)
    {
        fprintf (stdout, "input=%i frame=%i\n", record_buffer[rec_buffer_counter].input, record_buffer[rec_buffer_counter].frame);
        rec_buffer_counter++;
    }
}
*/


void record_stop (void)
{
    fprintf (stdout, "Stopping recording\n");
    record_buffer[rec_buffer_counter].input = REC_EOF;
    record_save (player.score);
    record_state = REC_STOP;
}

void record_frame (void)
{
    if (input_mask != old_input_mask)
    {
        record_buffer[rec_buffer_counter].input = input_mask;
        record_buffer[rec_buffer_counter].frame = frame_counter;
        rec_buffer_counter++;
        if (rec_buffer_counter > MAX_FRAMES)
        {
            record_stop ();
            fprintf (stderr, "Record buffer full!\n");
            msg_show ("RECORD BUFFER FULL", screen_width /2, screen_height /2, 3, font3, ALIGN_CENTRE, red);
        }
        old_input_mask = input_mask;
    }
}

void playback_frame (void)
{

    if (record_buffer[rec_buffer_counter].input == REC_EOF)
    {
        fprintf (stdout, "Reached end of playback\n");
        playback_stop ();
    }


    if (record_buffer[rec_buffer_counter].frame == frame_counter)
    {
        memcpy (&input_mask, &record_buffer[rec_buffer_counter].input, sizeof(Uint8));
        rec_buffer_counter++;
    }
}
