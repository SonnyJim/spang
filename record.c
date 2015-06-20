#include "spang.h"
#define MAX_FRAMES 4096

typedef struct
{
    Uint8 input;
    Uint32 frame;
} record_t;

int record_state = REC_STOP;

Uint32 frame_counter;
//Whereabouts in the record buffer we are
Uint32 rec_buffer_counter;
Uint8 old_input_mask;
Uint8 input_mask;

record_t record_buffer[MAX_FRAMES];

#define FRAMECHUNK (sizeof (Uint8) + sizeof (Uint32))

void record_save (long score)
{
    FILE *fp;
    char filename[64];
    /*
    char *buffer;
    char *outbuffer;
    long filesize;
    int ret;
    */
    sprintf (filename, "recordings/%li.rec", score);
    fp = fopen (filename, "w");
    if (fp == NULL)
    {
        fprintf (stderr, "Error opening record file for writing\n");
        return;
    }
    /*
    filesize = FRAMECHUNK * rec_buffer_counter;
    buffer = malloc (filesize);
    outbuffer = malloc (filesize);

    if (buffer == NULL)
    {
        fprintf (stderr, "Error:  Couldn't alloc buffer for record_save\n");
        return;
    }
    */
    rec_buffer_counter = 0;
    while (record_buffer[rec_buffer_counter].input != REC_EOF)
    {

        fwrite (&record_buffer[rec_buffer_counter].input, sizeof(Uint8), 1, fp);
        fwrite (&record_buffer[rec_buffer_counter].frame, sizeof(Uint32), 1, fp);
        rec_buffer_counter++;
        /*
        memcpy (buffer + (FRAMECHUNK * rec_buffer_counter), &record_buffer[rec_buffer_counter].input, sizeof(Uint8));
        memcpy (buffer + (FRAMECHUNK * rec_buffer_counter) + sizeof(Uint8), &record_buffer[rec_buffer_counter].frame, sizeof(Uint32));
        rec_buffer_counter++;
        */
    }
    /*
    ret = zlib_deflate_to_file (buffer, filesize, filename);
    //ret = zlib_deflate (buffer, filesize, outbuffer);
    fwrite (buffer, ret, 1, fp);
    fprintf (stdout, "Record Filesize: %li Returned: %i.  Wrote %li frames\n", filesize, ret, rec_buffer_counter);

    free (buffer);
    free (outbuffer);
    */
    fclose (fp);
}

void record_load (char *file)
{
    FILE *fp;
    /*
    int filesize;
    int ret;
    char *inbuffer, *outbuffer;
    */
    fp = fopen (file, "r");
    if (fp == NULL)
    {
        fprintf (stderr, "Error opening %s for reading\n", file);
        gamestate = GAME_AMODE;
        return;
    }
/*
    fseek (fp, 0, SEEK_END);
    filesize = ftell (fp);
    fseek (fp, 0, SEEK_SET);

    inbuffer = malloc (filesize);
    outbuffer = malloc (FRAMECHUNK * MAX_FRAMES);

    if (inbuffer == NULL || outbuffer == NULL)
    {
        fprintf (stderr, "Error mallocing record buffer\n");
        return;
    }

    fread (inbuffer, filesize, 1, fp);
    fclose (fp);

    ret = zlib_inflate (inbuffer, filesize, outbuffer, FRAMECHUNK * MAX_FRAMES);
    fprintf (stdout, "Playback Filesize: %i returned: %i\n", filesize, ret);
*/
    rec_buffer_counter = 0;
    record_buffer[rec_buffer_counter].input = REC_NONE;
    while (record_buffer[rec_buffer_counter].input != REC_EOF)
    {

        fread (&record_buffer[rec_buffer_counter].input, sizeof(Uint8), 1, fp);
        fread (&record_buffer[rec_buffer_counter].frame, sizeof(Uint32), 1, fp);
        rec_buffer_counter++;
        /*
        memcpy (&record_buffer[rec_buffer_counter].input, outbuffer + (FRAMECHUNK * rec_buffer_counter), sizeof(Uint8));
        memcpy (&record_buffer[rec_buffer_counter].frame, outbuffer + (FRAMECHUNK * rec_buffer_counter) + sizeof(Uint8), sizeof(Uint32));
        rec_buffer_counter++;
        */
     }
     /*
    fprintf (stdout, "loaded %li frames\n", rec_buffer_counter);
    free (inbuffer);
    free (outbuffer);
    */
    fclose (fp);
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
        fprintf (stdout, "Playing back %i on frame %i\n", \
                 record_buffer[rec_buffer_counter].input, frame_counter);
        memcpy (&input_mask, &record_buffer[rec_buffer_counter].input, sizeof(Uint8));
        rec_buffer_counter++;
        fprintf (stdout, "Waiting for frame: %i rec_buf %i to play input %i\nCurrently on frame %i and rec_buffer %i\n", \
                 record_buffer[rec_buffer_counter].frame, rec_buffer_counter, record_buffer[rec_buffer_counter].input, frame_counter, rec_buffer_counter - 1);
    }
}
