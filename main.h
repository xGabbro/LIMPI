#pragma once

#include <xstring.h>
#include <xsocket.h>

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

XSOCKET xsocket;

#define MAXBUFFER 1024
#define PORT 13333

const char * NEW_GAME = "cmdnewgame1x";
const char * CLOSE_SOCKET = "cmdclosesocket1x";
const char * CLEAR_CONSOLE = "cmdclearconsole1x";
const char * FINISH_GAME = "cmdfinishgame1x";

const char * trustedLetters = "abcdefghijklmnopqrstuvwxyz";

bool imServer;
bool myRound;

int my_points = 0;
int opponent_points = 0;

bool startGame(char * _phrase, int _lives);

void statusToStr(char * _Dest, const char * _phraseAUX, const char * _livesStr, const char * _usedLetters);

void getPhraseAUX(char * _Str, char * _Dest);

void spaceString(char *_Str, char *_Dest);

void clearConsole(bool scores);