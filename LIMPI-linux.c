#include "LIMPI-linux.h"

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

		client_addr_size = sizeof(client_addr);
		clientfd = accept(serverfd, (struct sockaddr*)&client_addr, &client_addr_size);
		printf("%s:%d connesso\n", inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));

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

		close(clientfd);
		close(serverfd);
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

		close(clientfd);
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
		system("clear");

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
			system("clear");
			printf("----LOG----\n");
		}
		else if ( strcmp(buffer, CLOSE_SOCKET) == 0 ) {
			break;
		}
		else printf("%s", buffer);


	} while(1);

}

void setupSocketForServer() {
	struct sockaddr_in server;

	//Creazione socket
	serverfd = socket(AF_INET, SOCK_STREAM, 0);
	if (serverfd < 0) {
		printError("Impossibile creare un socket!");
	}
	printf("Socket creato! \n");

	//Preparazione socket
	server.sin_family = AF_INET;
	server.sin_addr.s_addr = INADDR_ANY;
	server.sin_port = htons(PORT);

	//Bind server
	if ( bind(serverfd, (struct sockaddr*)&server, sizeof(server)) < 0 ) {
		printError("Bind del socket fallito!");
	}
	printf("Bind del socket fatto! \n");

	//Inizio ad ascoltare
	if ( listen(serverfd, 1) < 0 ) {
		printError("Impossibile mettersi in ascolto sul server!");
	}
	printf("In ascolto sul server!");

}

void setupSocketForClient() {
	struct sockaddr_in client;

	//Chiedo l'ip
	char ip[16];
	printf("Inserisci l'ip della stanza: ");
	scanf("%s", ip);
	printf("\n");

	//Creazione socket
	clientfd = socket(AF_INET, SOCK_STREAM, 0);
	if (clientfd < 0) {
		printError("Impossibile creare un socket!");
	}
	printf("Socket creato! \n");

	//Preparazione socket
	client.sin_family = AF_INET;
	client.sin_addr.s_addr = inet_addr(ip);
	client.sin_port = htons(PORT);

	//Connessione al server
	if ( connect(clientfd, (struct sockaddr*)&client, sizeof(client)) < 0 ) {
		printError("Impossibile connettersi al Server!");
	}
	printf("Connessione al server riuscita! \n");

}

void reciveMessage(char * buffer) {
	buffer[0] = '\0';
	int result = recv(clientfd, buffer, MAX_BUFFER, 0);
	if (result < 0) {
		printError("Impossibile comunicare con il server!");
	}
	buffer[result] = '\0';
}

void sendMessage(char * buffer) {
	int result = send(clientfd, buffer, MAX_BUFFER, 0);
	if (result < 0) {
		printError("Impossibile comunicare con il server!");
	}
}

void printError(char * error) {
	printf("Errore: %s\n", error);
	exit(0);
}
