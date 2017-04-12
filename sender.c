/**
 * This file includes the implementation of the sender executable.
 *
 * @author Jeffrey Bromen
 * @author Raymond Fleming
 * @date 4/12/2017
 * @info Systems and Networks II
 * @info Project 2
 *
 */

#include "rdtSender.h"

int main(int argc, char **argv)
{
	if (argc <= 5)
	{
		fprintf(stderr, "ERROR: Not enough arugments.\n"
		                "Usage: \"sendPort recvAddr recvPort netwAddr netwPort\"\n");
		exit(EXIT_FAILURE);
	}

	// Get network parameters
	int localport = atoi(argv[1]);
	char *desthost = argv[2];
	int destport = atoi(argv[3]);
	char *netwhost = argv[4];
	int netwport = atoi(argv[5]);

	char myAddr[INET_ADDRSTRLEN] = "";
	getOwnAddress(myAddr);
	printf("IP Address:  %s\n"
	       "Port Number: %d\n", myAddr, localport);

	char message[MAX_MESSAGE_SIZE]="";
	// Prompt user to enter message to send
	getUserMessage(message);
	// Send message
	sendMessage(localport, netwhost, netwport, desthost, destport, message);

	exit(EXIT_SUCCESS);
}