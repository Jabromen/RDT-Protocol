#include "rdtSender.h"

int main(int argc, char **argv)
{
	if (argc <= 5)
	{
		fprintf(stderr, "ERROR: Not enough arugments\n");
		exit(EXIT_FAILURE);
	}

	int localport = atoi(argv[1]);
	char *desthost = argv[2];
	int destport = atoi(argv[3]);
	char *netwhost = argv[4];
	int netwport = atoi(argv[5]);
	char message[MAX_MESSAGE_SIZE]="";
	writeUserMessage(message);
	sendMessage(localport, netwhost, netwport, desthost, destport, message);


	exit(EXIT_SUCCESS);
}