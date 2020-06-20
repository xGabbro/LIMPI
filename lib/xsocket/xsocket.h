#pragma once

#include <stdlib.h>
#include <stdio.h>

#define socket_exception const char *

#ifdef _WIN32
    #include <winsock2.h>

    #pragma comment (lib, "ws2_32.lib")

    #define WSA_VERSION 0x0202

    extern SOCKET serverSocket;
    extern SOCKET clientSocket;
#else
    #include <arpa/inet.h>
    #include <sys/socket.h>
    #include <unistd.h>

    extern int serverSocket;
    extern int clientSocket;
#endif

class XSOCKET {


    public:
        void setupSocketForServer(int _PORT);
        void setupSocketForClient(int _PORT, char * _IP);

        void sendMessage(const char * buffer, int size);
        void receiveMessage(char * buffer, int size);

        void close();

    private:

    protected:

};

void clearInput();
void waitForUserInput();