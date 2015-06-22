enum
{
    REC_STOP,
    REC_REC,
    REC_PLAY,
    REC_PLAYING
};

enum
{
    REC_NONE = 0,
    REC_LEFT = 1,
    REC_RIGHT = 2,
    REC_FIRE = 4,
    REC_BOMB = 8,
    REC_EOF = 16
};

typedef struct
{
    Uint8 input;
    Uint32 frame;
} record_t;

extern Uint8 input_mask;
extern Uint8 old_input_mask;
extern Uint32 frame_counter;
extern Uint32 rec_buffer_counter;
extern int record_state;

extern record_t record_buffer[];

void record_start (void);
void record_stop (void);
void record_frame (void);

void playback_frame (void);
void playback_select (void);
void playback_loop (void);
void playback_quit (void);
void playback_stop (void);
int record_load (char *file);
int playback_dump (void);
void playback_dump2 (char *file);

void playback_input_left (void);
void playback_input_right (void);
void playback_input_fire (void);

int zlib_deflate_to_file (char* src, int srclen, char* filename);
int zlib_inflate_from_file (char* filename, char* dst, int bufflen);
