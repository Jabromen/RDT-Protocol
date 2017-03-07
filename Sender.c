#include "udpsockets.h"

int main (int argc, char** argv) {
    
    // Check if enough arguments
    if (argc < 3) {
        printf("ERROR: Not enough arguments. Use format:\n"
               "\"portNum numberOfHosts\"\n");
        exit(EXIT_FAILURE);
    }

    GetMessage();
    SegmentMessage();
    SpawnSendThread();
    SpawnAckThread();

    exit(EXIT_SUCCESS);
}

void GetMessage(){
    
}
void SegmentMessage();
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