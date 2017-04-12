/**
 * This file includes the implementation of the network executable.
 *
 * @author Jeffrey Bromen
 * @author Raymond Fleming
 * @date 4/12/2017
 * @info Systems and Networks II
 * @info Project 2
 *
 */
#include "rdtNetwork.h"

int main(int argc, char **argv)
{
    //Check for valid number of arguments
    if (argc < 5) {
        printf("ERROR: Not enough arguments.\n"
               "Usage: \"netwPort LostPercent DelayedPercent ErrorPercent\"\n");
        exit(EXIT_FAILURE);
    }
    
    //store arguments
    int my_port  = atoi(argv[1]);
    int lostPercent = atoi(argv[2]);
    int delayedPercent = atoi(argv[3]);
    int errorPercent = atoi(argv[4]);

    char myAddr[INET_ADDRSTRLEN] = "";
    getOwnAddress(myAddr);
    printf("IP Address:  %s\n"
           "Port Number: %d\n", myAddr, my_port);

    //Start receive message function
	receiveMessage(my_port,lostPercent,delayedPercent,errorPercent);

	exit(EXIT_SUCCESS);
}