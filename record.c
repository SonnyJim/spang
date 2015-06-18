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

void record_save (void)
{
    FILE *fp;

    fp = fopen ("spang.rec", "w");
    if (fp == NULL)
    {
        fprintf (stderr, "Error opening record file for writing\n");
        return;
    }

    fwrite (&record_buffer, frame_counter, sizeof (record_t), fp);
    fclose (fp);
}

void record_load (void)
{
    FILE *fp;
    int filesize;

    fp = fopen ("spang.rec", "r");
    if (fp == NULL)
    {
        fprintf (stderr, "Error opening record file for reading\n");
        return;
    }

    fseek (fp, 0, SEEK_END);
    filesize = ftell (fp);
    fseek (fp, 0, SEEK_SET);

    fread (&record_buffer, filesize, 1, fp);
    fclose (fp);
}

void record_start (void)
{
    frame_counter = 0;
    rec_buffer_counter = 0;
    memset (record_buffer, 0, sizeof (record_buffer));
    record_state = REC_REC;
}

void playback_start (void)
{
    record_load ();
    frame_counter = 0;
    rec_buffer_counter = 0;
    record_state = REC_PLAYING;
    fprintf (stdout, "Starting playback\n");
}

void playback_stop (void)
{
    record_state = REC_STOP;
    hsentry_playback = 1;
}

void record_stop (void)
{
    fprintf (stdout, "Stopping recording\n");
    record_save ();
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
    if (record_buffer[rec_buffer_counter].frame == frame_counter)
    {
        memcpy (&input_mask, &record_buffer[rec_buffer_counter].input, sizeof(Uint8));
        rec_buffer_counter++;
    }
}
