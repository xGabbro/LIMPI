#include "xsocket.h"

#ifdef _WIN32
    SOCKET serverSocket;
    SOCKET clientSocket;
#else
    int serverSocket;
    int clientSocket;
#endif

void setupSocketForServer(int _PORT)
{

#ifdef _WIN32
    //Inizializzazione di winsock
    WSADATA wsa;
    if (WSAStartup(WSA_VERSION, &wsa) != NO_ERROR)
    {

        printf("Errore nell'inizializzare WinSock2! \n");
        waitForUserInput();
        exit(1);
    }

    printf("-> WSA inizializzato correttamente! \n");

#endif

    //Creo il socket
    serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket == 0)
    {

        printf("Impossibile creare il socket! \n");
        waitForUserInput();
        exit(1);
    }

    printf("-> Socket creato correttamente! \n");

    //Proprità del server
    struct sockaddr_in serverInfo;

    serverInfo.sin_family = AF_INET;
    serverInfo.sin_port = htons(_PORT);
    serverInfo.sin_addr.s_addr = INADDR_ANY;

    //Bind del server
    if (bind(serverSocket, (struct sockaddr*)&serverInfo, sizeof(serverInfo)) == -1)
    {

        printf("Impossibile connettere il server al socket! \n");
        waitForUserInput();
        exit(1);
    }

    //Metto in ascolto il server
    if (listen(serverSocket, 1) == -1)
    {

        printf("Impossibile mettere in ascolto il server! \n");
        waitForUserInput();
        exit(1);
    }

    //Aspetto che qualcuno si connetta

    printf("-> Sto aspettando che qualcuno si connetta... \n");

    clientSocket = accept(serverSocket, NULL, NULL);
    if (clientSocket == -1)
    {

        printf("Impossibile stabilire una connessione co il client! \n");
        waitForUserInput();
        exit(1);
    }

}

void setupSocketForClient(int _PORT, char *_IP)
{

#ifdef _WIN32
    //Inizializzazione di winsock
    WSADATA wsa;
    if (WSAStartup(WSA_VERSION, &wsa) != NO_ERROR)
    {

        printf("Errore nell'inizializzare WinSock2! \n");
        waitForUserInput();
        exit(1);
    }

    printf("-> WSA inizializzato correttamente! \n");
#endif

    //Creo il socket
    clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocket == 0)
    {

        printf("Impossibile creare il socket! \n");
        waitForUserInput();
        exit(1);
    }

    printf("-> Socket creato correttamente! \n");

    //Proprità del client
    struct sockaddr_in clientInfo;

    clientInfo.sin_family = AF_INET;
    clientInfo.sin_port = htons(_PORT);
    clientInfo.sin_addr.s_addr = inet_addr(_IP);

    printf("-> Mi connetto al server... \n");

    if (connect(clientSocket, (struct sockaddr*)&clientInfo, sizeof(clientInfo)) == -1)
    {

        printf("Impossibile stabilire una connessione con il server! \n");
        waitForUserInput();
        exit(1);
    }

}

void sendMessage(const char *buffer, int size)
{
    int result;

    result = send(clientSocket, buffer, size + 1, 0);
}

void receiveMessage(char *buffer, int size)
{
    int result;

    memset(buffer, 0, size);

    result = recv(clientSocket, buffer, size - 1, 0);

}

void exitPLS()
{

#ifdef _WIN32
    closesocket(serverSocket);
    closesocket(clientSocket);

    WSACleanup();
#else
    close(serverSocket);
    close(clientSocket);
#endif

}

void waitForUserInput()
{
    clearInput();
    printf("Premere spazio per continuare . . .\n");
    getchar();
    
}

void clearInput()
{
  int ch;

  do
    ch = fgetc(stdin); 
  while ( ch != EOF && ch != '\n' ); 

  clearerr(stdin);
}
