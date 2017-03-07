#include "udpsockets.h"

int main (int argc, char** argv) {
    
    // Check if enough arguments
    if (argc < 3) {
        printf("ERROR: Not enough arguments. Use format:\n"
               "\"portNum numberOfHosts\"\n");
        exit(EXIT_FAILURE);
    }

    while(!messageComplete())
    {
        WaitForPacket();
        ParsePacket();
        if(!packetCorrupt)
        {
            SendAck();
            ParsePayload();
        }
    }
    PrintMessage();
    

    exit(EXIT_SUCCESS);
}

void WaitForPacket(){
    
}
void ParsePacket(){
    
}
int PacketCorrupt(){
    
}
void SendAck(){
    
}
void ParsePayload(){
    
}