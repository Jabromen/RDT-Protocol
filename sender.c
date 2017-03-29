#include "rdtSender.h"

int main(int argc, char **argv)
{
	if (argc <= 6)
	{
		fprintf(stderr, "ERROR: Not enough arugments\n");
		exit(EXIT_FAILURE);
	}

	int localport = atoi(argv[1]);
	char *netwhost = argv[2];
	int netwport = atoi(argv[3]);
	char *desthost = argv[4];
	int destport = atoi(argv[5]);
	char *message = argv[6];

	sendMessage(localport, netwhost, netwport, desthost, destport, message);

	exit(EXIT_SUCCESS);
}