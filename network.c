#include "rdtNetwork.h"

int main(int argc, char **argv)
{
    //Check for valid number of arguments
    if (argc < 5) {
        printf("ERROR: Not enough arguments. Use format:\n"
               "\"portNum numberOfHosts\"\n");
        exit(EXIT_FAILURE);
    }
    
    //store arguments
    u_short my_port  = (u_short) atoi(argv[1]);
    int lostPercent=(int)atoi(argv[2]);
    int delayedPercent=(int)atoi(argv[3]);
    int errorPercent=(int)atoi(argv[4]);

    //Start receive message function
	receiveMessage(my_port,lostPercent,delayedPercent,errorPercent);

	exit(EXIT_SUCCESS);
}