#include "main.h"

int main()
{
    srand(time(NULL));

    // init
    int choose;

    char * buffer = new char[MAXBUFFER]();

    // Server o Client?
    while (1)
    {
        printf("[1] Crea una stanza (richiede porta sbloccata) \n");
        printf("[2] Entra in una stanza \n\n");

        printf(" => ");
        scanf("%s", buffer);
        printf("\n");

        choose = atoi(buffer);

        if (choose == 1)
        {
            imServer = true;
            break;
        }
        else if (choose == 2)
        {
            imServer = false;
            break;
        }

        clearConsole(false);
    }

    //Setup iniziale
    if (imServer)
    {

        try
        {
            xsocket.setupSocketForServer(PORT);
        }
        catch (socket_exception e)
        {
            printf("%s \n", e);
        }

        // Randomizzo il giocatore che inizierà per primo
        if (rand() % 2 == 1)
        {
            xsocket.sendMessage("serverstart", 13);
            myRound = true;
        }
        else
        {
            xsocket.sendMessage("clientstart", 13);
            myRound = false;
        }
    }
    else
    {
        char * ip = new char[16]();

        printf("|| Inserisci l'ip: ");
        scanf("%s", ip);
        printf("\n");

        try
        {
            xsocket.setupSocketForClient(PORT, ip);
        }
        catch (socket_exception e)
        {
            printf("%s \n", e);
        }

        xsocket.receiveMessage(buffer, MAXBUFFER);
        if (string_compare(buffer, "serverstart"))
            myRound = false;
        else
            myRound = true;
    }

    //Loop
    while (1)
    {

        clearConsole(false);

        if (imServer)
        {

            printf("[1] Inizia una nuova partita \n");
            printf("[2] Esci \n");
            printf(" => ");
            scanf("%s", buffer);

            printf("\n");

            choose = atoi(buffer);
            if (choose == 1)
            {

                try
                {
                    xsocket.sendMessage(buffer, string_lenght(buffer));
                }
                catch (socket_exception e)
                {
                    printf("%s \n", e);
                }
            }
            else if (choose == 2)
            {
                //Esci
                xsocket.sendMessage(CLOSE_SOCKET, string_lenght(CLOSE_SOCKET));

                break;
            }
        }
        else
        {

            xsocket.receiveMessage(buffer, MAXBUFFER);

            if (string_compare(buffer, NEW_GAME))
            {
                //Nuova partita

                clearConsole(true);

        
            }
            else if (string_compare(buffer, CLOSE_SOCKET))
            {
                //Esci

                break;
            }
        }

        //Divido i giocatori in spettatore e giocatore
        if (myRound)
        {
            printf("Il server sta impostando il gioco... \n");

            clearConsole(true);

            bool win = startGame(NULL, NULL);

            if (win) my_points += 1;
            else opponent_points += 1;

        }
        else
        {
            char * phrase = new char[MAXBUFFER];

            printf("Inserisci la frase: ");
            scanf(" %[^\n]", phrase);
            printf("\n");

            xsocket.sendMessage(phrase, string_lenght(phrase));

            printf("Inserisci il numero di vite: ");
            scanf("%s", buffer);
            printf("\n");

            xsocket.sendMessage(buffer, string_lenght(buffer));

            int lives = atoi(buffer);

            clearConsole(true);

            bool win = startGame(phrase, lives);
    
            if (win) my_points += 1;
            else opponent_points += 1;

        }

        myRound = !myRound;
    }

    delete[] buffer;

    //Chiudo i socket e wsa
    xsocket.close();

    return 0;
}

bool startGame(char * _phrase, int _lives) 
{
    printf("[...] Attendo che l'avversario scelga la frase \n");

    bool win;

    //Setup buffer
    char * buffer = new char[MAXBUFFER]();

    //Setup della stringa che conterrà le lettere usate
    char * usedLetters = new char[string_lenght(trustedLetters) + 1]();

    //Setup delle stringe per il gioco

    char * phrase;
    if (myRound) 
    {
        xsocket.receiveMessage(buffer, MAXBUFFER);
    
        phrase = new char[string_lenght(buffer) + 1]();
        string_copy(phrase, buffer);
    }
    else 
    {
        phrase = new char[string_lenght(_phrase) + 1]();
        string_copy(phrase, _phrase);
    }

    char * phraseAUX = new char[string_lenght(buffer) + 1]();
    getPhraseAUX(phrase, phraseAUX);

    //Ricevo il numero di vite
    int lives;

    if (myRound) {
        xsocket.receiveMessage(buffer, MAXBUFFER);
        lives = atoi(buffer);
    }
    else 
    {
        lives = _lives;
    }

    if (lives < 1 || lives > 10)
        lives = 5;

    //Inizio del gioco
    char c;
    bool flag;
    bool isUsed = true;
    while (1)
    {
        //Stampo lo status per i 2 giocatori
        spaceString(phraseAUX, buffer);

        printf("%s \n", buffer);
        printf("Vite rimaste: %d \n", lives);

        //Vedo se ho perso
        if (lives == 0)
        {

            if (myRound) 
            {
                printf("Hai perso!! \n");
                printf("La frase era: \"%s\" \n", phrase);

                win = false;
            }
            else
            {
                printf("Il tuo avversario è nabbo! \n");

                win = true;
            }

            waitForUserInput();

            break;
        }

        //Vedo se ho vinto
        if (string_compare(phrase, phraseAUX))
        {

            if (myRound)
            {
                printf("Hai vinto!! \n");
                win = true;
            }
            else 
            {
                printf("Il tuo avversario ha vinto... \n");
                win = false;
            }

            waitForUserInput();

            break;
        }

        //Attendo che il giocatore inserisca un carattere

        if (myRound)
        {
            printf("Inserisci un carattere: ");
            scanf("%s", buffer);
            printf("\n");

            xsocket.sendMessage(buffer, string_lenght(buffer));
        }
        else 
        {
            xsocket.receiveMessage(buffer, MAXBUFFER);
        }

        c = char_lowercase(buffer[0]);

        //Pulisco la console
        clearConsole(true);

        //Vedo se la lettera è già stata utilizzata
        flag = false;
        for (int i = 0; i < string_lenght(usedLetters); i++)
        {

            if (c == usedLetters[i])
            {

                if (myRound)
                {
                    printf("La lettera '%c' è già stata utilizzata! \n", c);
                }
                else
                {
                    printf("Il tuo avversario ha reinserito la lettera %c \n", c);
                }

                waitForUserInput();

                flag = true;

                break;
            }
        }

        if (flag)
            continue;
        else
            string_addchar(usedLetters, c);

        //Sostituisco la lettera nella stringa se esiste ausiliaria
        isUsed = false;
        for (int i = 0; i < string_lenght(phrase); i++)
        {

            if (c == char_lowercase(phrase[i]))
            {
                phraseAUX[i] = phrase[i];

                isUsed = true;
            }
        }

        if (!isUsed)
        {
            lives -= 1;
        }
    }

    delete[] buffer;
    delete[] usedLetters;
    delete[] phrase;
    delete[] phraseAUX;

    return win;
}

void getPhraseAUX(char *_Str, char *_Dest)
{
    bool flag;

    for (int i1 = 0; i1 < string_lenght(_Str); i1 += 1)
    {
        flag = false;

        for (int i2 = 0; i2 < string_lenght(trustedLetters); i2 += 1)
        {

            if (char_lowercase(_Str[i1]) == trustedLetters[i2])
            {

                flag = true;

                break;
            }
        }

        if (flag)
            _Dest[i1] = '_';
        else
            _Dest[i1] = _Str[i1];
    }

    _Dest[string_lenght(_Str)] = '\0';
}

void statusToStr(char *_Dest, const char *_phraseAUX, const char *_livesStr, const char *_usedLetters)
{

    char *lettersStr = (char *)calloc(string_lenght(trustedLetters) * 2 + 1, sizeof(char));

    for (int s1 = 0; s1 < string_lenght(trustedLetters); s1 += 1)
    {
        static bool flag;
        flag = false;

        for (int s2 = 0; s2 < string_lenght(_livesStr); s2 += 1)
        {

            if (trustedLetters[s1] == _usedLetters[s2])
            {

                flag = true;

                break;
            }
        }

        if (!flag)
        {
            string_addchar(lettersStr, trustedLetters[s1]);
            string_addchar(lettersStr, ' ');
        }
    }

    _Dest[string_lenght(lettersStr)] = '\0';
}

void spaceString(char *_Str, char *_Dest)
{
    char * finalstr = new char[string_lenght(_Str) * 2 + 1]();

    for (int i = 0; i < string_lenght(_Str); i++)
    {
        string_addchar(finalstr, _Str[i]);
        string_addchar(finalstr, ' ');
    }

    string_copy(_Dest, finalstr);
}

void clearConsole(bool scores)
{

#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif

    if (scores) {
        printf("========================================================= \n");
        printf("|                    LIMPI by Gabbro                    | \n");
        printf("|                                                       | \n");
        printf("|            Punti miei: %02d / punti suoi: %02d            | \n", my_points, opponent_points);
        printf("========================================================= \n\n");
    }
    else {
        printf("========================================================= \n");
        printf("|                    LIMPI by Gabbro                    | \n");
        printf("========================================================= \n\n");
    }

}