#include "main.h"

int main()
{
    int choose;

    char buffer[MAXBUFFER];
    memset(buffer, 0, sizeof(buffer));

    // Server o Client?
    printf("[1] Server \n");
    printf("[2] Client \n");
    scanf("%s", buffer);

    choose = atoi(buffer);
    if (choose == 1)
    {
        //Server 

        printf("---Server Log--- \n");
        setupSocketForServer(PORT);

        clearConsole();

        while (1)
        {
            //Nuova partita o esci
            printf("[1] Inizia una nuova partita \n");
            printf("[2] Esci \n");
            scanf("%s", buffer);

            choose = atoi(buffer);
            if (choose == 1)
            {
                //Nuova partita
                sendMessage(NEW_GAME, strlen(NEW_GAME));

                //Scelgo chi deve indovinare
                printf("[1] Indovina il server \n");
                printf("[2] Indovina il client \n");
                scanf("%s", buffer);

                choose = atoi(buffer);
                if (choose == 1)
                {

                    sendMessage("1", 2);

                    startGame();
                }
                else if (choose == 2)
                {

                    sendMessage("2", 2);

                    printf("Inserisci la frase: ");
                    scanf(" %[^\n]%*c", buffer);
                    printf("\n");

                    sendMessage(buffer, strlen(buffer));

                    printf("Inserisci il numero di vite: ");
                    scanf("%s", buffer);
                    printf("\n");

                    sendMessage(buffer, strlen(buffer));

                    spectGame();
                }
            }
            else if (choose == 2)
            {
                //Esci

                sendMessage(CLOSE_SOCKET, strlen(CLOSE_SOCKET));
                break;
            }

        }
    }
    else if (choose == 2)
    {
        //Client

        char ip[16];
        printf("Inserisci l'ip: ");
        scanf("%s", ip);
        printf("\n");

        printf("---Client Log--- \n");
        setupSocketForClient(PORT, ip);

        clearConsole();

        while (1)
        {
            printf("Server sceglie cosa fare \n");

            //Devo iniziare una nuova partita o esco?
            receiveMessage(buffer, sizeof(buffer));

            if (strcmp(buffer, NEW_GAME) == 0)
            {
                //Nuova partita

                receiveMessage(buffer, sizeof(buffer));

                choose = atoi(buffer);
                if (choose == 1)
                {
                    clearConsole();

                    printf("Inserisci la frase: ");
                    scanf(" %[^\n]%*c", buffer);
                    printf("\n");

                    printf("%s\n", buffer);

                    sendMessage(buffer, strlen(buffer));

                    printf("Inserisci il numero di vite: ");
                    scanf("%s", buffer);
                    printf("\n");

                    sendMessage(buffer, strlen(buffer));

                    spectGame();
                }
                else if (choose == 2)
                {
                    printf("Il server sta impostando il gioco... \n");

                    startGame();
                }
            }
            else if (strcmp(buffer, CLOSE_SOCKET) == 0)
            {
                //Esci

                break;
            }
        }
    }

    //Chiudo i socket e wsa
    exitPLS();

    return 0;
}

void startGame()
{
    //Setup buffer
    char buffer[MAXBUFFER];
    memset(buffer, 0, sizeof(buffer));

    char buffer2[MAXBUFFER];
    memset(buffer2, 0, sizeof(buffer2));

    //Setup della stringa che conterrà le lettere usate
    char *usedLetters = (char*)malloc(sizeof(trustedLetters) * sizeof(char));
    memset(usedLetters, 0, sizeof(usedLetters));

    //Setup delle stringe per il gioco
    char *phrase;
    char *phraseAUX;

    receiveMessage(buffer, sizeof(buffer));

    phrase = (char*)malloc(strlen(buffer) * sizeof(char) + 1);
    memset(phrase, '\0', sizeof(phrase));

    phraseAUX = (char*)malloc(strlen(buffer) * sizeof(char) + 1);
    memset(phraseAUX, '\0', sizeof(phraseAUX));

    memcpy(phrase, buffer, strlen(buffer) + 1);
    memcpy(phraseAUX, buffer, strlen(buffer) + 1);

    getPhraseAUX(phrase, phraseAUX);

    //Ricevo il numero di vite
    receiveMessage(buffer, sizeof(buffer));

    int lives = atoi(buffer);

    //Inizio del gioco
    char c;
    int flag = 0;
    while (1)
    {

        //Stampo lo status per i 2 giocatori
        spaceString(phraseAUX, buffer);

        sprintf(buffer2, "Vite rimaste: %d\n", lives);

        strcat(buffer, buffer2);

        printf("%s", buffer);
        sendMessage(buffer, strlen(buffer));

        //Vedo se ho perso
        if (lives == 0)
        {

            printf("Hai perso!! \n");
            printf("La frase era: \"%s\" \n", phrase);

            sendMessage("Hai scelto una parola troppo difficile! \n", 43);

            waitForUserInput();

            break;
        }

        //Vedo se ho vinto
        if (strcmp(phrase, phraseAUX) == 0)
        {

            printf("Hai vinto!! \n");
            sendMessage("Hai scelto una parola troppo facile! \n", 43);

            waitForUserInput();

            break;
        }

        //Attendo che il giocatore inserisca un carattere
        printf("Inserisci un carattere: ");
        scanf("%s", buffer);
        printf("\n");

        c = toLowerCase(buffer[0]);

        //Pulisco la console
        clearConsole();
        sendMessage(CLEAR_CONSOLE, strlen(CLEAR_CONSOLE));

        //Vedo se la lettera è già stata utilizzata
        flag = 0;
        for (int i = 0; i < strlen(usedLetters); i++)
        {

            if (c == usedLetters[i])
            {

                printf("Lettera gia' utilizzata! \n");
                sendMessage("Lettera gia' utilizzata! \n", 28);

                waitForUserInput();

                flag = 1;

                break;
            }
        }

        if (flag)
            continue;
        else
            strncat(usedLetters, &c, 1);

        //Sostituisco la lettera nella stringa ausiliaria
        flag = 0;
        for (int i = 0; i < strlen(phrase); i++)
        {

            if (c == toLowerCase(phrase[i]))
            {
                phraseAUX[i] = phrase[i];

                flag = 1;
            }
        }

        if (!flag)
            lives -= 1;
            
    }

    //Invio il messaggio di fine partita
    sendMessage(FINISH_GAME, strlen(FINISH_GAME));
}

void spectGame()
{
    //Setup buffer
    char buffer[MAXBUFFER];
    memset(buffer, 0, sizeof(buffer));

    while (1)
    {
        //Ricevo e vedo cosa fare

        receiveMessage(buffer, sizeof(buffer));

        if (strcmp(buffer, CLEAR_CONSOLE) == 0)
        {
            //Pulisco console
            clearConsole();
        }
        else if (strcmp(buffer, FINISH_GAME) == 0)
        {
            //Fine della partita
            break;
        }
        else
        {
            printf("%s", buffer);
        }
    }
}

void getPhraseAUX(char *_Str, char *_Dest)
{
    int flag;

    for (int i1 = 0; i1 < strlen(_Str); i1 += 1)
    {
        flag = 0;

        for (int i2 = 0; i2 < strlen(trustedLetters); i2 += 1)
        {

            if (toLowerCase(_Str[i1]) == trustedLetters[i2])
            {

                flag = 1;

                break;
            }
        }

        if (flag)
            _Dest[i1] = '_';
        else
            _Dest[i1] = _Str[i1];
    }
}