#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <stdio.h>

#include <string.h>
#include <ctype.h>

// Need to link with Ws2_32.lib, Mswsock.lib, and Advapi32.lib
#pragma comment (lib, "Ws2_32.lib")
#pragma comment (lib, "Mswsock.lib")
#pragma comment (lib, "AdvApi32.lib")

#define PORT 13333
#define MAX_BUFFER 1024

#define CLEAR_COMMAND "cmdclear"
#define CLOSE_SOCKET "cmdclosesocket"

SOCKET sock;
SOCKET sockclient;

struct sockaddr_in client_addr;
int client_addr_size;

char buffer[MAX_BUFFER];

const char alphabet[] = "abcdefghijklmnopqrstuvwxyz";
char alphabetAUX[27] = "";

void startGame();
void spectGame();

void setupSocketForServer();
void setupSocketForClient();

void sendMessage(char * buffer);
void reciveMessage(char * buffer);

void printError(char * error);

void cc()  {

	for (int i = 0; i < 50; i++)
		printf("\n");

}