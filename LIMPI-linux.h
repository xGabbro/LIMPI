#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include <ctype.h>

#include <arpa/inet.h>
#include <sys/socket.h>

#define PORT 13333
#define MAX_BUFFER 1024

#define CLEAR_COMMAND "cmdclear"
#define CLOSE_SOCKET "cmdclosesocket"

int serverfd = 0;
int clientfd = 0;

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