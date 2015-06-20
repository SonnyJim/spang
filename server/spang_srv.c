#include <stdio.h>
#include <stdlib.h>
#include <string.h>
 
#include "SDL_net.h"
#include "SDL_thread.h"

#define BUFFLEN (sizeof (char) * 3) + sizeof(long)
#define SPANG_PORT 5459

int running = 0;

static int receive_score (void *ptr);
void score_receive (char* initials, long score);
int scores_write (void *ptr);

int main(int argc, char **argv)
{

    SDL_Thread *thread1;
    SDL_Thread *thread2;
    int receive_score_return;

    fprintf (stdout, "Spang hiscore server\n");
    running = 1;
    
    thread1 = SDL_CreateThread (scores_write, "scores_write", (void *) NULL);
    if (NULL == thread1)
    {
        fprintf (stderr, "Error creating scores_write thread: %s\n", SDL_GetError ());
        return 1;
    }

    thread2 = SDL_CreateThread (receive_score, "receive_score", (void *) NULL);
    if (NULL == thread2)
    {
        fprintf (stderr, "Error creating receive_score thread: %s\n", SDL_GetError ());
        running = 0;
        return 1;
    }
    else
    {
        fprintf (stdout, "Waiting for connections\n");
        SDL_WaitThread (thread2, &receive_score_return);
        fprintf (stdout, "receive_score returned %i\n", receive_score_return);
    }
    return 0;
}

static int receive_score (void *ptr)
{
    TCPsocket sd, csd; /* Socket descriptor, Client socket descriptor */
	IPaddress ip, *remoteIP;

    char initials[3] = "";
    long score = 0;
	char buffer[BUFFLEN];
    
	if (SDLNet_Init() < 0)
	{
		fprintf(stderr, "SDLNet_Init: %s\n", SDLNet_GetError());
		return(EXIT_FAILURE);
	}
 
	if (SDLNet_ResolveHost(&ip, NULL, SPANG_PORT) < 0)
	{
		fprintf(stderr, "SDLNet_ResolveHost: %s\n", SDLNet_GetError());
		return(EXIT_FAILURE);
	}
 
	if (!(sd = SDLNet_TCP_Open(&ip)))
	{
		fprintf(stderr, "SDLNet_TCP_Open: %s\n", SDLNet_GetError());
		return(EXIT_FAILURE);
	}
 
	while (running)
	{
		if ((csd = SDLNet_TCP_Accept(sd)))
		{
			if ((remoteIP = SDLNet_TCP_GetPeerAddress(csd)))
				printf("Host connected: %x %d\n", SDLNet_Read32(&remoteIP->host), SDLNet_Read16(&remoteIP->port));
			else
				fprintf(stderr, "SDLNet_TCP_GetPeerAddress: %s\n", SDLNet_GetError());
            
            fprintf (stdout, "IP: %s\n", SDLNet_ResolveIP (remoteIP));
			if (SDLNet_TCP_Recv(csd, buffer, BUFFLEN) > 0)
			{
                strncpy (initials, buffer, 3);
                memcpy (&score, buffer + 3, sizeof(score));
                fprintf (stdout, "Received data %s %li\n", initials, score);
                score_receive (initials, score);
			}
			SDLNet_TCP_Close(csd);
		}
	}
 
	SDLNet_TCP_Close(sd);
	SDLNet_Quit();
 
	return EXIT_SUCCESS;
}
