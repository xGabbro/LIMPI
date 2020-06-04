#pragma once

#include <xsocket.h>
#include <utils.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXBUFFER 1024

#define PORT 13333

const char * NEW_GAME = "cmdnewgame1x";
const char * CLOSE_SOCKET = "cmdclosesocket1x";
const char * CLEAR_CONSOLE = "cmdclearconsole1x";
const char * FINISH_GAME = "cmdfinishgame1x";

const char * trustedLetters = "abcdefghijklmnopqrstuvwxyz";

void startGame();
void spectGame();

void getPhraseAUX(char * _Str, char * _Dest);