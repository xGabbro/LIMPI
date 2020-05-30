#include "xsocket.h"

void setupSocketForServer(int _PORT) {

    //Inizializzazione di winsock
    WSADATA wsa;
    if (WSAStartup(WSA_VERSION, &wsa) != NO_ERROR) {

        printf("Errore nell'inizializzare WinSock2! \n");
        waitForUserInput();
        exit(1);

    }

    printf("-> WSA inizializzato correttamente! \n");

    //Creo il socket
    serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (serverSocket == INVALID_SOCKET) {

        printf("Impossibile creare il socket! \n");
        waitForUserInput();
        exit(1);

    }

    printf("-> Socket creato correttamente! \n");

    //Proprità del server
    SOCKADDR_IN serverInfo;

    serverInfo.sin_family = AF_INET;
    serverInfo.sin_port = htons(_PORT);
    serverInfo.sin_addr.s_addr = INADDR_ANY;

    //Bind del server
    if (bind(serverSocket, (SOCKADDR*)&serverInfo, sizeof(serverInfo)) == SOCKET_ERROR) {

        printf("Impossibile connettere il server al socket! \n");
        waitForUserInput();
        exit(1);

    }

    //Metto in ascolto il server
    if (listen(serverSocket, 1) == SOCKET_ERROR) {

        printf("Impossibile mettere in ascolto il server! \n");
        waitForUserInput();
        exit(1);
        
    }

    //Aspetto che qualcuno si connetta

    printf("-> Sto aspettando che qualcuno si connetta... \n");

    clientSocket = accept(serverSocket, NULL, NULL);
    if (clientSocket == SOCKET_ERROR) {
        
        printf("Impossibile stabilire una connessione co il client! \n");
        waitForUserInput();
        exit(1);

    }

}

void setupSocketForClient(int _PORT, char * _IP) {

    //Inizializzazione di winsock
    WSADATA wsa;
    if (WSAStartup(WSA_VERSION, &wsa) != NO_ERROR) {

        printf("Errore nell'inizializzare WinSock2! \n");
        waitForUserInput();
        exit(1);

    }

    printf("-> WSA inizializzato correttamente! \n");

    //Creo il socket
    clientSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (clientSocket == INVALID_SOCKET) {

        printf("Impossibile creare il socket! \n");
        waitForUserInput();
        exit(1);

    }

    printf("-> Socket creato correttamente! \n");

    //Proprità del client
    SOCKADDR_IN clientInfo;

    clientInfo.sin_family = AF_INET;
    clientInfo.sin_port = htons(_PORT);
    clientInfo.sin_addr.s_addr = inet_addr(_IP);

    printf("-> Mi connetto al server... \n");

    if (connect(clientSocket, (SOCKADDR*)&clientInfo, sizeof(clientInfo)) == SOCKET_ERROR) {

        printf("Impossibile stabilire una connessione con il server! \n");
        waitForUserInput();
        exit(1);

    }

}

void sendMessage(char * buffer, int size) {
    int result;

    result = send(clientSocket, buffer, size + 1, 0);

}

void receiveMessage(char * buffer, int size) {
    int result;

    memset(buffer, 0,  size);

    result = recv(clientSocket, buffer, size - 1, 0);

}

void exitPLS() {

    closesocket(serverSocket);
    closesocket(clientSocket);

    WSACleanup();

}

void waitForUserInput()
{
    printf("Premere un tasto per continuare . . .");
    fflush(stdin);
    getchar();
}
