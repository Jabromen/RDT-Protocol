/**
 * This file includes the implementation of the functions used in the network process.
 *
 * @author Jeffrey Bromen
 * @author Raymond Fleming
 * @date 4/12/2017
 * @info Systems and Networks II
 * @info Project 2
 *
 */

#include "rdtNetwork.h"
#include <pthread.h>

void receiveMessage (int port,int lostPercent,int delayedPercent,int errorPercent) {
    
    char buffer[PACKET_LENGTH];
    int fd;
    int i=0;
        
    //Seed the RNG
    srand(time(NULL));

    //Create needed structs
    struct addressList *List=malloc(sizeof(addressList));
    struct socketPacket *fdPacket=malloc(sizeof(socketPacket));

    NetTraffic *Traffic = initializeNetworkTraffic();
    
    //initialize address list
    initializeAddressList(List);
    
    //initialize socket
    if ((fd = initializeSocket(port)) < 0)
	{
		fprintf(stderr, "Socket initialization failed\n");
		
	}
    
    //run forever
    while(1){
        
        //Print Network Stats every 2 packets
        for(i=0;i<2;i++){

            //Receive packet
            recv(fd, buffer, PACKET_LENGTH, 0);

            fdPacket->fd=&fd;
            fdPacket->packet=buffer;
       
            //Check if this is a new host
            newHost(buffer,List);
            
            //Record sender/receiver data
            RecordNetworkTraffic(Traffic,buffer,List);
    
            //If packet should be dropped, don't do anything further besides record
            if(!PacketDropped(lostPercent))
            {
                //Check if corrupt and set corruption bit and record if needed
                if(corrupt(errorPercent))
                {
                    corruptPacket(buffer);
                    RecordCorrupt(Traffic);
                }
                
                //Check if packet is delayed and record, then start delay if needed
                if(PacketDelayed(delayedPercent))
                {
                    RecordDelayed(Traffic);
                    StartDelayThread(fdPacket);
                }
                
                //If not delayed, go ahead and forward packet
                else{
                    SendPacketToReceiver(fdPacket);

                }
            }
            else{
                RecordDropped(Traffic);
            }
        }
        
        //Printing stats every 2 packets
        PrintStats(Traffic);
    }
}

NetTraffic *initializeNetworkTraffic() {
    NetTraffic *Traffic = (NetTraffic *) malloc(sizeof(NetTraffic));

    Traffic->SenderPackets = 0;
    Traffic->ReceiverPackets = 0;
    Traffic->DelayedPackets = 0;
    Traffic->CorruptPackets = 0;
    Traffic->DroppedPackets = 0;

    return Traffic;
}

void initializeAddressList(addressList *List){

    int i=0;

    //Zero sender/receiver counts
    List->numberofReceivers=0;
    List->numberofSenders=0;

    //Zero all address values
    for(i=0;i<NUM_HOSTS;i++){
        memset(List->recIPArray[i], 0, INET_ADDRSTRLEN);
        memset(List->sendIPArray[i], 0, INET_ADDRSTRLEN);
        List->sendPortArray[i] = 0;
        List->recPortArray[i] = 0;
    }
    
}

void newHost(char *packet,addressList *List){
    
    char sendbuffer[BUFFER_SIZE];
    char recbuffer[BUFFER_SIZE];

    // Get source address
    getSourceIP(sendbuffer,packet);
    
    // Get source port
    int sendPort=getSourcePort(packet);

    // Get destination address
    getDestinationIP(recbuffer,packet);
    
    // Get destination port
    int recPort=getDestinationPort(packet);
    
    //Check if source is a new address
    int testSource=addressInList(sendbuffer,sendPort,List);

    //If this is a new source, add both sender and receiver
    if(testSource==0){
        addHost("Sndr",sendbuffer,sendPort,List);
        addHost("Rcvr",recbuffer,recPort,List);
    }
}

int addressInList(char *buffer,int Port,addressList *List) {
    int i = 0;
    
    //Check if passed address is in sender list
    while (i < List->numberofSenders) {
        if (!strcmp(buffer,List->sendIPArray[i]) && List->sendPortArray[i] == Port) {
            //If found, mark as sender
            return 1;
        }
        i++;
    }
    //Zero index for second loop
    i = 0;

    //Check if passed address is in receiver list
    while (i < List->numberofReceivers) {
        if (!strcmp(buffer,List->recIPArray[i]) && List->recPortArray[i] == Port){
            //If found, mark as receiver
            return 2;
        }
        i++;
    }
    
    return 0;
}

void addHost(char *toggle,char *buffer,int Port,addressList *List){

    //Check if toggle is sender
    if(!strcmp(toggle,"Sndr")){
        //Add sender's address and port to list
        strcpy(List->sendIPArray[List->numberofSenders],buffer);
        List->sendPortArray[List->numberofSenders]=Port;
        List->numberofSenders++;
    }
    else //this is a receiver
    {
        //Add receiver's address and port to list
        strcpy(List->recIPArray[List->numberofReceivers],buffer);
        List->recPortArray[List->numberofReceivers]=Port;
        List->numberofReceivers++;
    }


}

int PacketDropped(int droppedPercent){
    return randomInt(droppedPercent);    
}

void RecordDropped(NetTraffic *Traffic){
    //Increment dropped counter
    Traffic->DroppedPackets++;
}

int corrupt(int errorPercent){
    return randomInt(errorPercent);
}

void RecordCorrupt(NetTraffic *Traffic){
    //Increment corrupt counter
    Traffic->CorruptPackets++;
}


int randomInt(int testVal){   
    //Get random value between 0 and 100
    int randVal=rand()%100;
    
    //Test if random is less than test value
    if(randVal<=testVal){
        return 1;
    }
    else{
        return 0;
    }
}

int PacketDelayed(int delayedPercent){
    return randomInt(delayedPercent);
     
}

void RecordDelayed(NetTraffic *Traffic){
    //Increment delayed counter
    Traffic->DelayedPackets++;
}

void RecordNetworkTraffic(NetTraffic *Traffic,char *packet,addressList *List){

    // If this is a sender packet
    if(senderMessage(packet,List)) {
        Traffic->SenderPackets++;
    }
    // If this is a receiver packet
    else {
        Traffic->ReceiverPackets++;
    }
}

void StartDelayThread(socketPacket *fdPacket){

    int err;
    pthread_t delay_thread;
    
    //Create thread and error test
    if ((err = pthread_create(&delay_thread, NULL, &DelayThread, fdPacket))) {
		fprintf(stderr, "Can't create Delay Thread: [%s]\n", strerror(err));
	}  
}

int senderMessage(char *packet,addressList *List){

    char buffer[BUFFER_SIZE];
    
    //Extract source address
    getSourceIP(buffer,packet);
    
    //Extract source port
    int Port=getSourcePort(packet);
    
    //Check if address is in sender list
    return addressInList(buffer,Port,List) == 1 ? 1 : 0;
}

void SendPacketToReceiver(socketPacket *fdPacket){
    int fd=*fdPacket->fd;
    char dest[BUFFER_SIZE];
    int port=getDestinationPort(fdPacket->packet);
    
    getDestinationIP(dest,fdPacket->packet);


    //Forward packet
    sendPacket(fd,fdPacket->packet,dest,port);
}

void *DelayThread(void *param) {

    //Decapsulate parameter
    struct socketPacket *fdPacket=param;

    // Sleep to delay packet
    usleep(NETWORK_DELAY_SCALE * (SEND_TIMEOUT_SEC * 10e6 + SEND_TIMEOUT_USEC));
    
    //After sleep delay, forward packet
    SendPacketToReceiver(fdPacket);

    pthread_exit(NULL);
}

void PrintStats(NetTraffic *Traffic){
    //Calculate total number of packets (sender+receiver)
    int TotalPackets=Traffic->SenderPackets+Traffic->ReceiverPackets;
    printf("\nCurrent Stats:\n");
    printf("Total Packets:    %d\n",TotalPackets);
    printf("Sender Packets:   %d\n",Traffic->SenderPackets);
    printf("Receiver Packets: %d\n",Traffic->ReceiverPackets);
    printf("Delayed Packets:  %d\n",Traffic->DelayedPackets);
    printf("Dropped Packets:  %d\n",Traffic->DroppedPackets);
    printf("Corrupt Packets:  %d\n",Traffic->CorruptPackets);
}
