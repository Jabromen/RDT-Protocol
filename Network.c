#include "udpsockets.h"

int main (int argc, char** argv) {
    
    // Check if enough arguments
    if (argc < 3) {
        printf("ERROR: Not enough arguments. Use format:\n"
               "\"portNum numberOfHosts\"\n");
        exit(EXIT_FAILURE);
    }

    WaitForPacket();
    if(!PacketDropped())
    {
        setCorruptionBit();
        if(PacketDelayed())
        {
            StartDelayThread();
        }
        RecordNetworkTraffic();
        SendPacketToReceiver();
    }

    exit(EXIT_SUCCESS);
}
void PacketDropped(){
    
}
void setCorruptionBit(){
    
}
void StartDelayThread(){
    
}
void RecordNetworkTraffic(){
    
}
void SendPacketToReceiver(){
    
}