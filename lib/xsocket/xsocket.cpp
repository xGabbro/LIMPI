#include "xsocket.h"

#ifdef _WIN32
    SOCKET serverSocket;
    SOCKET clientSocket;
#else
    int serverSocket;
    int clientSocket;
#endif

void XSOCKET::setupSocketForServer(int _PORT)
{

#ifdef _WIN32
    //Inizializzazione di winsock
    WSADATA wsa;
    if (WSAStartup(WSA_VERSION, &wsa) != NO_ERROR)
        throw "[!] Errore nell'inizializzare WinSock2! \n";

#endif

    //Creo il socket
    serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket == 0)
        throw "[!] Impossibile creare il socket! \n";

    //Proprità del server
    struct sockaddr_in serverInfo;

    serverInfo.sin_family = AF_INET;
    serverInfo.sin_port = htons(_PORT);
    serverInfo.sin_addr.s_addr = INADDR_ANY;

    //Bind del server
    if (bind(serverSocket, (struct sockaddr*)&serverInfo, sizeof(serverInfo)) == -1)
        throw "[!] Impossibile connettere il server al socket! \n";

    //Metto in ascolto il server
    if (listen(serverSocket, 1) == -1)
        throw "[!] Impossibile mettere in ascolto il server! \n";

    //Aspetto che qualcuno si connetta

    clientSocket = accept(serverSocket, NULL, NULL);
    if (clientSocket == -1)
        throw "[!] Impossibile stabilire una connessione co il client! \n";

}

void XSOCKET::setupSocketForClient(int _PORT, char *_IP)
{

#ifdef _WIN32
    //Inizializzazione di winsock
    WSADATA wsa;
    if (WSAStartup(WSA_VERSION, &wsa) != NO_ERROR)
        throw "[!] Errore nell'inizializzare WinSock2! \n";

#endif

    //Creo il socket
    clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocket == 0)
        throw "[!] Impossibile creare il socket! \n";

    //Proprità del client
    struct sockaddr_in clientInfo;

    clientInfo.sin_family = AF_INET;
    clientInfo.sin_port = htons(_PORT);
    clientInfo.sin_addr.s_addr = inet_addr(_IP);

    if (connect(clientSocket, (struct sockaddr*)&clientInfo, sizeof(clientInfo)) == -1)
        throw "[!] Impossibile stabilire una connessione con il server! \n";

}

void XSOCKET::sendMessage(const char *buffer, int size)
{
    int result;

    result = send(clientSocket, buffer, size + 1, 0);
    
    if (result < 0)
        throw "[!] Errore durante l'invio del messaggio! \n";

}

void XSOCKET::receiveMessage(char * buffer, int size)
{
    int result;

    result = recv(clientSocket, buffer, size - 1, 0);

    if (result == 0)
        throw "[!] Errore di disconnessione! \n";
    else if (result == -1)
        throw "[!] Errore durante la ricezione del messaggio! \n";

}

void XSOCKET::close()
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
