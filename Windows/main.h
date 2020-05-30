#pragma once

#include "lib\xsocket\xsocket.c"
#include "lib\utils\utils.c"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXBUFFER 1024

#define PORT 13333

#define NEW_GAME "cmdnewgame1x"
#define CLOSE_SOCKET "cmdclosesocket1x"
#define CLEAR_CONSOLE "cmdclearconsole1x"
#define FINISH_GAME "cmdfinishgame1x"

const char trustedLetters[] = "abcdefghijklmnopqrstuvwxyz";

void startGame();
void spectGame();

void getPhraseAUX(char * _Str, char * _Dest);