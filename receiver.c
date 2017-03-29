#include "rdtReceiver.h"

int main(int argc, char **argv)
{
	if (argc < 2)
	{
		fprintf(stderr, "ERROR: Not enough arguments\n");
		exit(EXIT_FAILURE);
	}

	int port = atoi(argv[1]);

	receiveMessage(port);

	exit(EXIT_SUCCESS);
}