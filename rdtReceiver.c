#include "udpsockets.h"
#include "definitions.h"
#include "udpPacket.h"

void SendAck(const char *packet, udpsocket_t *sckt);
void ParsePayload(const char *packet,char *message);
void printMessage(const char *message);

int main (int argc, char** argv) {
    
    char message[MAX_MESSAGE_SIZE] = "";
    // Check if enough arguments
    if (argc < 2) {
        printf("ERROR: Not enough arguments. Use format:\n"
               "\"portNum numberOfHosts\"\n");
        exit(EXIT_FAILURE);
    }
    
    // Parse command line arguments
    u_short my_port  = (u_short) atoi(argv[1]);

    // Create UDP socket
    udpsocket_t *sckt = initUdpSocket(my_port);

    if (!sckt)
        exit(EXIT_FAILURE);




    // Buffer to receive and send messages
    char buffer[PACKET_LENGTH];
    // Number of bytes received
    int recvlen;
    
    while(!messageComplete())
    {
        memset(buffer, 0, PACKET_LENGTH);
        recvlen = receiveMessage(buffer, PACKET_LENGTH, sckt);
        
        if(!isCorrupt(buffer))
        {
            SendAck(buffer,sckt);
            ParsePayload(buffer,message);
        }
    }
    PrintMessage();
    

    exit(EXIT_SUCCESS);
}


void SendAck(const char *packet, udpsocket_t *sckt){
    
    char buffer[MAX_MESSAGE_SIZE];
    char srcIP[IP_SIZE];
    int srcPort;
    char destIP[IP_SIZE];
    int destPort;
    char segment[SEGMENT_LENGTH];
    int sequenceNum;
    
 //   bzero(buffer);
    
    getDestinationIP(srcIP,packet);
    srcPort=getDestinationPort(packet);
    getSourceIP(destIP,packet);
    destPort=getSourcePort(packet);
    
    
    makePacket(buffer,srcIP, srcPort, destIP,destPort,segment);
    sendMessage(buffer, sckt);
}

void ParsePayload(const char *packet,char *message){
    char buffer[SEGMENT_LENGTH];
    
    getSegmentContent(buffer,packet);
    
    strcat(message,buffer);
}
void printMessage(const char *message){
    printf("Message received is :\n[%s]",message);
}