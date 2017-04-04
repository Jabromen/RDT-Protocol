#include "rdtNetwork.h"
#include <pthread.h>


void receiveMessage (int my_port,int lostPercent,int delayedPercent,int errorPercent) {
    
    char buffer[PACKET_LENGTH];
    int fd;
    int i=0;


        
    //Seed the RNG
    srand(time(NULL));

    //Create needed structs
    struct addressList *List=malloc(sizeof(addressList));
    struct sendBlock *block=malloc(sizeof(sendBlock));
    NetTraffic *Traffic = (NetTraffic *) malloc(sizeof(NetTraffic));
    

    //initialize address list
    initializeAddressList(List);
    
    //initialize socket
    if ((fd = initializeSocket(my_port)) < 0)
	{
		fprintf(stderr, "Socket initialization failed\n");
		
	}
	
    

    

    

    //run forever
    while(1){
        
        //Print Network Stats every 2 packets
        for(i=0;i<2;i++){
            
            //Receive packet
            recv(fd, buffer, PACKET_LENGTH, 0);
            block->fd=fd;
            strcpy(block->packet,buffer);
            block->destPort=getDestinationPort(buffer);
            getDestinationIP(block->destHost,buffer);
            
            
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
                    StartDelayThread(block);
                }
                
                //If not delayed, go ahead and forward packet
                else{
                    SendPacketToReceiver(block);
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


void initializeAddressList(addressList *List){
    int i=0;
    int j=0;

    //Zero sender/receiver counts
    List->numberofReceivers=0;
    List->numberofSenders=0;

    //Zero all address values
    for(i=0;i<NUM_HOSTS;i++){
        for(j=0;j<IP_SIZE;j++){
            List->recIPArray[i][j]='\0';
            List->sendIPArray[i][j]='\0';
        }
        List->recPortArray[i]=0;
    }
    
}

void newHost(char *packet,addressList *List){
    
    
    char sendbuffer[BUFFER_SIZE];
    getSourceIP(sendbuffer,packet);
    int sendPort=getSourcePort(packet);
    char recbuffer[BUFFER_SIZE];
    
    //Get destination address
    getDestinationIP(recbuffer,packet);
    
    //Get destination port
    int recPort=getDestinationPort(packet);
    
    //Check if source is a new address
    int testSource=addressInList(sendbuffer,sendPort,List);

    //If this is a new source, add both sender and receiver
    if(testSource==0){
        addHost("Sndr",sendbuffer,sendPort,List);
        addHost("Rcvr",recbuffer,recPort,List);
    }
}

int addressInList(char *buffer,int Port,addressList *List)
{
    int addressFound=0;
    int i=0;
    

    //Check if passed address is in sender list
    while(List->sendPortArray[i]!=0)
    {

        if(List->sendPortArray[i]==Port){

            if(strcmp(buffer,List->sendIPArray[i])==0){
                //If found, mark as sender
                addressFound=1;
            }
        }
        i++;
    }
    //Zero index for second loop
    i=0;

    //Check if passed address is in receiver list
    while(List->sendPortArray[i]!=0)
    {

        if(List->recPortArray[i]==Port){
            if(strcmp(buffer,List->recIPArray[i])==0){
                //If found, mark as receiver
                addressFound=2;
            }
        }
        i++;
    }
    
    return addressFound;
    
}


void addHost(char *toggle,char *buffer,int Port,addressList *List){

    //Check if toggle is sender
    if(strcmp(toggle,"Sndr")==0){
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

    //If this is a sender packet, increment number of sender packets
    if(senderMessage(packet,List)==1){
        Traffic->SenderPackets++;
    }
    else{ //This is a receiver packet
        //Increment receiver packets
        Traffic->ReceiverPackets++;
    }
}

void StartDelayThread(sendBlock *block){
    
    int err;
    pthread_t delay_thread;
    
    //Create thread and error test
    if ((err = pthread_create(&delay_thread, NULL, &DelayThread, block))) {
		fprintf(stderr, "Can't create Network Thread: [%s]\n", strerror(err));
		exit(EXIT_FAILURE);
	}
	//Avoid zombie children
	pthread_join(delay_thread, NULL);
    
}


int senderMessage(char *packet,addressList *List){
    char buffer[BUFFER_SIZE];
    
    //Extract source address
    getSourceIP(buffer,packet);
    
    //Extract source port
    int Port=getSourcePort(packet);
    int returnval=0;
    
    //Check if address is in sender list
    if(addressInList(buffer,Port,List)==1){
        returnval=1;
    }
    return returnval;
}

void SendPacketToReceiver(sendBlock *block){
    int sckt;
    if ((sckt = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
	{
		perror("Cannot create socket");
		return -1;
	}
	
    char *packet=block->packet;
    char *dest=block->destHost;
    int port=block->destPort;

    

    //Forward packet
    sendPacket(sckt,packet,dest,port);
}

void *DelayThread(void *param) {
    
    //Decapsulate parameter
    struct sendBlock *block=param;
    
    //Set random timer between 0 and 10 ms
    struct timespec timer,timer2;
    timer.tv_sec=0;
    timer.tv_nsec=rand()%10000000;
    timer2.tv_sec=0;
    timer2.tv_nsec=0;
    nanosleep(&timer,&timer2);
    
    //After sleep delay, forward packet
    SendPacketToReceiver(block);
    return 0;
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