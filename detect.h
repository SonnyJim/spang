
int check_axis (SDL_Rect a, SDL_Rect b);
int check_axis_x (SDL_Rect a, SDL_Rect b);
//void detect_playerhit (void);
void detect_bullet (int num);
//void detect_playerhit (void (*function)(int), SDL_Rect rect, int object_num);
void detect_ballhit (SDL_Rect rect, int ball);
void detect_enemy_bullet (SDL_Rect rect, int bullet);
void detect_enemyhit (SDL_Rect rect, int enemy);
