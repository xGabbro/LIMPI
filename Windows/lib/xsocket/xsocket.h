#pragma once

#include <stdio.h>
#include <winsock2.h>

#pragma comment (lib, "ws2_32.lib")

#define WSA_VERSION 0x0202

SOCKET serverSocket;
SOCKET clientSocket;

void setupSocketForServer(int _PORT);
void setupSocketForClient(int _PORT, char * _IP);

void sendMessage(char * buffer, int size);
void receiveMessage(char * buffer, int size);

void exitPLS();

void waitForUserInput();