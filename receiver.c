/**
 * This file includes the implementation of the receiver executable.
 *
 * @author Jeffrey Bromen
 * @author Raymond Fleming
 * @date 4/12/2017
 * @info Systems and Networks II
 * @info Project 2
 *
 */

#include "rdtReceiver.h"

int main(int argc, char **argv)
{
	if (argc < 2)
	{
		fprintf(stderr, "ERROR: Not enough arguments\n"
		                "Usage: \"recvPort\"\n");
		exit(EXIT_FAILURE);
	}

	int port = atoi(argv[1]);

	char myAddr[INET_ADDRSTRLEN] = "";
	getOwnAddress(myAddr);
	printf("IP Address:  %s\n"
	       "Port Number: %d\n", myAddr, port);

	receiveMessage(port);

	exit(EXIT_SUCCESS);
}