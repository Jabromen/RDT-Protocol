#include "udpsockets.h"

int main (int argc, char** argv) {
    char message[MAX_MESSAGE_SIZE] = "";
	char line[BUFFER_SIZE] = "";
	int numSegments=0;
	
    // Check if enough arguments
    if (argc < 3) {
        printf("ERROR: Not enough arguments. Use format:\n"
               "\"portNum numberOfHosts\"\n");
        exit(EXIT_FAILURE);
    }

    GetMessage(message);
    numSegments= numberOfSegments(const char *message);
    SpawnSendThread();
    SpawnAckThread();

    exit(EXIT_SUCCESS);
}

void GetMessage(char *message){
    

	// Prompt user for message
	printf("Enter the message to send, max size is %d\n",MAX_MESSAGE_SIZE);

	// Read user message from stdin
	while (1) {
		fgets(line, BUFFER_SIZE, stdin);
		if (!strcmp(line, "\n")) {
			break;
		}
		strcat(message, line);
		memset(line, 0, strlen(line));
	}

	// If no message was entered, don't save it.
	if (!strcmp(message, "\0"))
		return;
}



void SendThread(){
    GetWindowLock();
    SendNextPacket();
    StartTimeout();
}

void SpawnAckThread(){
    WaitForAck();
    if(!PacketCorrupt())
    {
        StopTimeout();
        GiveWindowLock();
    }
}

void GetWindowLock(){
    
}

void SendNextPacket(){
    
}

void StartTimeout(){
    
}

void StopTimeout(){
    
}

void GiveWindowLock(){
    
}