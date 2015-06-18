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
    REC_BOMB = 8
};
extern Uint8 input_mask;
extern Uint8 old_input_mask;
extern Uint32 frame_counter;
extern int record_state;
void record_start (void);
void record_stop (void);
void record_frame (void);
void playback_frame (void);
void playback_start (void);
void playback_stop (void);

