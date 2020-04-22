#include "spang.h"

#include "SDL2/SDL_net.h"

#define HOST "pinball.servebeer.com"
#define PORT 5459

int network_send_score (char* initials, long score)
{
    return 0;
	IPaddress ip;		/* Server address */
	TCPsocket sd;		/* Socket descriptor */
	int len = (sizeof (char) * 3) + sizeof (long)   ;
	char buffer[len];

	if (SDLNet_Init() < 0)
	{
		fprintf(stderr, "SDLNet_Init: %s\n", SDLNet_GetError());
		return (EXIT_FAILURE);
	}

	/* Resolve the host we are connecting to */
	if (SDLNet_ResolveHost(&ip, HOST, PORT) < 0)
	{
		fprintf(stderr, "SDLNet_ResolveHost: %s\n", SDLNet_GetError());
		return (EXIT_FAILURE);
	}

	/* Open a connection with the IP provided (listen on the host's port) */
	if (!(sd = SDLNet_TCP_Open(&ip)))
	{
		fprintf(stderr, "SDLNet_TCP_Open: %s\n", SDLNet_GetError());
		return (EXIT_FAILURE);
	}

	/* Build and send message */
    memset (buffer, 0, sizeof(len));
    memcpy (buffer, initials, 3);
    memcpy (buffer+3, &score, sizeof(score));

    if (SDLNet_TCP_Send(sd, (void *)buffer, len) < len)
    {
        fprintf (stderr, "SDLNet_TCP_Send: %s\n", SDLNet_GetError());
        return (EXIT_FAILURE);
    }

	SDLNet_TCP_Close(sd);
	SDLNet_Quit();

	return 0;
}
