#include "LIMPI-windows.h"

int main() {
	char choose;
	
	printf("---------------- \n");
	printf("LIMPI by xGabbro \n");
	printf("sas              \n");
	printf("---------------- \n\n\n");
	
	printf("[1] crea una stanza     \n");
	printf("[2] entra in una stanza \n\n");

	printf("Scelta: ");
	scanf("%c", &choose);

	if (choose == '1') {
		setupSocketForServer();

		
		sockclient = accept(sock, NULL, NULL);

		char choose;
		printf("---------------------- \n");
		printf("[1] Indovina il server \n");
		printf("[2] Indovina il client \n");
		printf("---------------------- \n\n");

		printf("Scelta: ");
		scanf(" %c", &choose);

		if (choose == '1') {

			sendMessage("1");
			startGame();
		
		}
		else if (choose == '2') {
			sendMessage("2");

			char phrase[1024];
			printf("Inserisci la frase: ");
			scanf(" %[^\n]", phrase);
			sendMessage(phrase);

			spectGame();

		}

		closesocket(sockclient);
		closesocket(sock);
	}
	else if (choose == '2') {
		setupSocketForClient();

		reciveMessage(buffer);

		if ( strcmp(buffer, "1") == 0 ) {

			char phrase[1024];

			printf("Inserisci la frase: ");
			scanf(" %[^\n]", phrase);
			sendMessage(phrase);

			spectGame();

		}
		else if ( strcmp(buffer, "2") == 0 ) {

			startGame();

		}

		closesocket(sockclient);
	}

	return 0;
}

void startGame() {

	//Inizializzazione stringe

	char * phrase;
	char * phraseAUX;

	reciveMessage(buffer);

	phrase = (char *)malloc(strlen(buffer) * sizeof(char) + 1);
	phraseAUX = (char *)malloc(strlen(buffer) * sizeof(char) + 1);


	strcpy(phrase, "");
	strcpy(phraseAUX, "");

	strcpy(phrase, buffer);

	//Creazione del vettore ausiliario
	int flag;
	for (int i = 0; i <= strlen(phraseAUX); i++) {
		flag = 0;

		for (int alphIndex = 0; alphIndex < strlen(alphabet); alphIndex += 1) {

			if (phrase[i] == alphabet[alphIndex]) {

				phraseAUX[i] = '_';

				flag = 1;

				break;
			}

		}

		if (!flag) phraseAUX[i] = phrase[i];

	}

	//Inizio gioco
	int lives = 5;
	char c;
	while(1) {
		//system("clear");
		cc();

		//Vedo se ho perso
		if (lives == 0) {

			printf("Hai perso! La frase era: %s \n", phrase);

			sendMessage("Il tuo amico non capisce un cazzo, hai vinto! \n");

			break;
		}

		//Vedo se ho vinto
		if ( strcmp(phraseAUX, phrase) == 0 ) {

			printf("%s \n", phraseAUX);
			printf("Hai vinto! \n");

			sendMessage("Hai scelto una parola troppo semplice, coion! \n");

			break;
		}

		//Stato gioco

		char temp[3];
		for (int i = 0; i < strlen(phraseAUX); i++) {
			temp[0] = phraseAUX[i];
			temp[1] = ' ';
			temp[2] = '\0';
			sendMessage(temp);
			printf("%c ", phraseAUX[i]);
	
		}
		printf("\n");
		sendMessage("\n");

		sprintf(temp, "%d", lives);
		temp[2] = '\0';

		printf("Vite rimaste: %d \n", lives);

		sendMessage("Vite rimaste: ");
		sendMessage(temp);
		sendMessage("\n");

		//SAS
		printf("Inserisci il carattere: ");
		scanf(" %c", &c);
		printf("\n");

		c = tolower(c);

		sendMessage(CLEAR_COMMAND);
		//Verifico che la lettera non sia utilizzata
		flag = 0;
		for (int i = 0; i < strlen(alphabetAUX); i++) {

			if (c == alphabetAUX[i]) {

				printf("Lettera già utilizzata!");

				sendMessage("Il tuo amico non capisce un cazzo, ha riinserito la stessa lettera! \n");

				flag = 1;

				break;
			}

		}

		if (flag) continue;
		else strncat(alphabetAUX, &c, 1);

		//ses
		flag = 0;
		for (int i = 0; i < strlen(phrase); i++) {

			if (c == tolower(phrase[i])) {

				phraseAUX[i] = phrase[i];
				flag = 1;

			}

		}

		if (!flag) lives -= 1;


	}

	sendMessage(CLOSE_SOCKET);
}

void spectGame() {
	system("clear");

	printf("----LOG----\n");
	do {
		
		reciveMessage(buffer);

		if ( strcmp(buffer, CLEAR_COMMAND) == 0 ) {
			//system("clear");
			cc();
			printf("----LOG----\n");
		}
		else if ( strcmp(buffer, CLOSE_SOCKET) == 0 ) {
			break;
		}
		else printf("%s", buffer);


	} while(1);

}

void setupSocketForServer() {

	WSADATA	WSAData;
	struct sockaddr_in saddr; 

	if (WSAStartup (MAKEWORD(2,2), &WSAData))
	{
		printf ("Errore nell'inizializzare le winsock.");
		return -1;
	}

	sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (sock == INVALID_SOCKET)
	{
		printf ("Errore nella creazione del socket.");
		WSACleanup ();
		return -1;
	}
	
	saddr.sin_family = AF_INET;
	saddr.sin_addr.s_addr = 0;
	saddr.sin_port = htons(PORT);

	// Chiamata alla funzione bind.
	if ( bind(sock, (struct sockaddr *) &saddr, sizeof(saddr) ))
	{
		printf ("Errore la porta 6000 è già occupata.");
		closesocket (sock);
		WSACleanup ();
		return -1;
	}
	
	if (listen(sock, 1))
	{
		printf ("Impossibile mettere in ascolto il socket.");
		closesocket (sock);
		WSACleanup ();
		return -1;
	}

}

void setupSocketForClient() {
	WSADATA	WSAData;
	struct sockaddr_in saddr; 

	char ip[16];
	printf("ip: ");
	scanf("%s", ip);
	printf("\n");

	if (WSAStartup (MAKEWORD(2,2), &WSAData))
	{
		printf ("Errore nell'inizializzare le winsock.");
		return -1;
	}

	sock = socket (AF_INET, SOCK_STREAM, IPPROTO_TCP);

	if (sock == INVALID_SOCKET)
	{
		printf ("Errore nella creazione del socket.");
		WSACleanup ();
		return -1;
	}

	// Si connette alla porta 6000.
	((struct sockaddr_in *)(ip->ai_addr))->sin_port = htons (6000);

	if (connect (sock, (struct sockaddr *)(ip->ai_addr), sizeof (struct sockaddr_in)))
	{
		printf ("Impossibile connettersi.");
		closesocket (sockclient);
		WSACleanup ();
		return -1;
	}

}

void reciveMessage(char * buffer) {
	buffer[0] = '\0';
	int result = recv(sockclient, buffer, MAX_BUFFER, 0);
	if (result < 0) {
		printError("Impossibile comunicare con il server!");
	}
	buffer[result] = '\0';
}

void sendMessage(char * buffer) {
	int result = send(sockclient, buffer, MAX_BUFFER, 0);
	if (result < 0) {
		printError("Impossibile comunicare con il server!");
	}
}

void printError(char * error) {
	printf("Errore: %s\n", error);
	exit(0);
}
