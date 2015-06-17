#include "spang.h"
#include "SDL2/SDL_joystick.h"

Uint32 input_repeat = 0;
SDL_Joystick* joysticks[4];

int input_keyrepeat (void)
{
    if (input_repeat + 200 < SDL_GetTicks ())
    {
        input_repeat = SDL_GetTicks ();
        return 0;
    }
    else
        return 1;
}

void input_joystick_init (void)
{
    const char *name;
    int i;

    //Load joystick defaults
    joy_cfg.axis1 = 0;
    joy_cfg.axis2 = 1;
    joy_cfg.fire = 0;
    joy_cfg.bomb = 1;
    joy_cfg.pause = 2;
    for (i = 0; i < SDL_NumJoysticks(); ++i)
    {
        name = SDL_JoystickNameForIndex(i);
        SDL_Log(" Found Joystick %d: %s\n", i, name ? name : "Unknown Joystick");
        joysticks[i] = SDL_JoystickOpen(i);
        if (joysticks[i] == NULL) {
            SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "SDL_JoystickOpen(%d) failed: %s\n", i,
                    SDL_GetError());
        }
        /*
        else {
            char guid[64];
            SDL_JoystickGetGUIDString(SDL_JoystickGetGUID(joystick),
                                      guid, sizeof (guid));
            SDL_Log("       axes: %d\n", SDL_JoystickNumAxes(joystick));
            SDL_Log("      balls: %d\n", SDL_JoystickNumBalls(joystick));
            SDL_Log("       hats: %d\n", SDL_JoystickNumHats(joystick));
            SDL_Log("    buttons: %d\n", SDL_JoystickNumButtons(joystick));
            SDL_Log("instance id: %d\n", SDL_JoystickInstanceID(joystick));
            SDL_Log("       guid: %s\n", guid);
        }
        */
    }
}

void input_joystick_close (void)
{
    int i;
    for (i = 0; i < 4; i++)
    {
        if (joysticks[i] != NULL)
            SDL_JoystickClose (joysticks[i]);
    }

}
