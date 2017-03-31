
#include "udpsockets.h"
#include <time.h>
#include "definitions.h"
#include "udpPacket.h"
#include <math.h>


typedef struct NetTraffic
{
	int SenderPackets;
    int ReceiverPackets;
    int DelayedPackets;
    int CorruptPackets;
    int DroppedPackets;

} NetTraffic;

typedef struct addressList
{
    int numberofSenders;
    int numberofReceivers;
    char recIPArray[NUM_HOSTS][IP_SIZE];
    int recPortArray[NUM_HOSTS];
    char sendIPArray[NUM_HOSTS][IP_SIZE];
    int sendPortArray[NUM_HOSTS];
} addressList;

int PacketDropped(int droppedPercent);
void RecordDropped(NetTraffic *Traffic);
int corrupt(int errorPercent);
void RecordCorrupt(NetTraffic *Traffic);
int randomInt(int testVal);
int PacketDelayed(int delayedPercent);
void RecordDelayed(NetTraffic *Traffic);
void RecordNetworkTraffic(NetTraffic *Traffic,char *packet,addressList *List);
void StartDelayThread();
void SendPacketToReceiver();
void PrintStats(NetTraffic *Traffic);
void *DelayThread(void *param);
int senderMessage(char *packet,addressList *List);
void newHost(char *packet,addressList *List);
void addHost(char *toggle,char *buffer,int Port,addressList *List);
int addressInList(char *buffer,int Port,addressList *List);
void initializeAddressList(addressList *List);


int main (int argc, char** argv) {
    char buffer[PACKET_LENGTH];
    char sourceIP[IP_SIZE]="123";
    char destinationIP[IP_SIZE]="321";
    int sourcePort=5;
    int destinationPort=5;
    char segment[SEGMENT_LENGTH]="Test";
    

    
    struct addressList *List=malloc(sizeof(addressList));
    
    // Check if enough arguments
    if (argc < 5) {
        printf("ERROR: Not enough arguments. Use format:\n"
               "\"portNum numberOfHosts\"\n");
        exit(EXIT_FAILURE);
    }
    
    
    NetTraffic *Traffic = (NetTraffic *) malloc(sizeof(NetTraffic));
    
    u_short my_port  = (u_short) atoi(argv[1]);
    int lostPercent=(int)atoi(argv[2]);
    int delayedPercent=(int)atoi(argv[3]);
    int errorPercent=(int)atoi(argv[4]);
    int err;
    int fd;
    
    fprintf(stderr,"\nMy Port is %d\nlostPercent is %d",(int)my_port,lostPercent);
    fprintf(stderr,"\ndelayedPercent is %d\nerrorPercent is %d\n",delayedPercent,lostPercent);

    initializeAddressList(List);
    if ((fd = initializeSocket(my_port, 0)) < 0)
	{
		fprintf(stderr, "Socket initialization failed\n");
		return 1;
	}
	
    
    int i=0;
    
    
    int recvlen;
    
    srand(time(NULL));
    

        
    while(1){
        
  //      Print Network Stats every 2 packets
        for(i=0;i<2;i++){
            recvlen = recv(fd, buffer, PACKET_LENGTH, 0);
            
            
            
            newHost(buffer,List);
            fprintf(stderr,"\n\nRecording Traffic\n");
            RecordNetworkTraffic(Traffic,buffer,List);
    
            if(!PacketDropped(lostPercent))
            {
                if(corrupt(errorPercent))
                {
                    corruptPacket(buffer);
                    RecordCorrupt(Traffic);
                }
                if(PacketDelayed(delayedPercent))
                {
                    RecordDelayed(Traffic);
                    StartDelayThread(Traffic);
                }
                else{
                    SendPacketToReceiver();
                }
            }
            else{
                RecordDropped(Traffic);
            }
            fprintf(stderr,"\nFirst sender port is %d\nfirst receiver port is %d\nSenders:\n",List->sendPortArray[0],List->recPortArray[0]);
            for(i=0;i<List->numberofSenders;i++){
                fprintf(stderr,"IP: %s. Port: %d\n",List->sendIPArray[i],List->sendPortArray[i]);
            }
            fprintf(stderr,"\nReceivers:\n");
            for(i=0;i<List->numberofReceivers;i++){
                fprintf(stderr,"IP: %s. Port: %d\n",List->recIPArray[i],List->recPortArray[i]);
            }
        }
        PrintStats(Traffic);
    }
}


void initializeAddressList(addressList *List){
    int i=0;
    int j=0;
//    fprintf(stderr,"\nIn initialize\n");
    List->numberofReceivers=0;
    List->numberofSenders=0;
//    fprintf(stderr,"\nNumhosts = %d\n",List->numberofHosts);
    for(i=0;i<NUM_HOSTS;i++){
        for(j=0;j<IP_SIZE;j++){
  //          fprintf(stderr,"\nWriting null to [%d][%d]\n",i,j);
            List->recIPArray[i][j]='\0';
            List->sendIPArray[i][j]='\0';
        }
        List->recPortArray[i]=0;
    }
    
}

void newHost(char *packet,addressList *List){
    
//    fprintf(stderr,"\n\nin newHost\n\n");
    
    char sendbuffer[BUFFER_SIZE];
    getSourceIP(sendbuffer,packet);
    int sendPort=getSourcePort(packet);
    
    char recbuffer[BUFFER_SIZE];
    
    getDestinationIP(recbuffer,packet);
    int recPort=getDestinationPort(packet);
    
    int testSource=addressInList(sendbuffer,sendPort,List);
    int testDest=addressInList(recbuffer,recPort,List);
    
    if(testSource==0){
        addHost("Sndr",sendbuffer,sendPort,List);
        addHost("Rcvr",recbuffer,recPort,List);
    }
    
    

    fprintf(stderr,"\nDone with newHost\n");
}

int addressInList(char *buffer,int Port,addressList *List)
{
    int addressFound=0;
    int i=0;
    
    fprintf(stderr,"\nTesting Sender. numHosts is %d. compare %d to %d\n",List->numberofSenders,Port,List->sendPortArray[0]);
    while(List->sendPortArray[i]!=0)
    {
        fprintf(stderr,"\nTesting port %d to %d\n",List->sendPortArray[i],Port);
        if(List->sendPortArray[i]==Port){
            fprintf(stderr,"\nPorts match testing %s and %s\n",buffer,List->sendIPArray[i]);
            if(strcmp(buffer,List->sendIPArray[i])==0){
                fprintf(stderr,"\nMatch found, sender\n");
                addressFound=1;
            }
        }
        i++;
    }
    i=0;
    fprintf(stderr,"\nTesting Receiver. Reccount is %d\n",List->numberofReceivers);
    while(List->sendPortArray[i]!=0)
    {
        fprintf(stderr,"\nTesting port %d to %d\n",List->recPortArray[i],Port);
        if(List->recPortArray[i]==Port){
            if(strcmp(buffer,List->recIPArray[i])==0){
                fprintf(stderr,"\nMatch found, receiver\n");
                addressFound=2;
            }
        }
        i++;
    }
    
    fprintf(stderr,"\nReturning %d\n",addressFound);
    return addressFound;
    
}


void addHost(char *toggle,char *buffer,int Port,addressList *List){
//    fprintf(stderr,"\n\n In addNode\n");
    
    
    if(strcmp(toggle,"Sndr")==0){
        strcpy(List->sendIPArray[List->numberofSenders],buffer);
        List->sendPortArray[List->numberofSenders]=Port;
        List->numberofSenders++;
    }
    else
    {
        strcpy(List->recIPArray[List->numberofReceivers],buffer);
        List->recPortArray[List->numberofReceivers]=Port;
        List->numberofReceivers++;
    }


}

int PacketDropped(int droppedPercent){
    return randomInt(droppedPercent);
    
}

void RecordDropped(NetTraffic *Traffic){
    Traffic->DroppedPackets++;

}

int corrupt(int errorPercent){
    return randomInt(errorPercent);
}

void RecordCorrupt(NetTraffic *Traffic){
    Traffic->CorruptPackets++;
}


int randomInt(int testVal){
    int randVal=rand()%100;
    if(randVal<=testVal){
        return 1;
    }
    else{
        return 0;
    }
}
int PacketDelayed(delayedPercent){
    return randomInt(delayedPercent);
     
}

void RecordDelayed(NetTraffic *Traffic){
    Traffic->DelayedPackets++;
}



void RecordNetworkTraffic(NetTraffic *Traffic,char *packet,addressList *List){
    fprintf(stderr,"\n\nIn RecordNetworkTraffic\n\n");

    
    
    if(senderMessage(packet,List)==1){
        Traffic->SenderPackets++;
    }
    else{
        Traffic->ReceiverPackets++;
    }
}

void StartDelayThread(){
    int err;
    pthread_t delay_thread;
    if ((err = pthread_create(&delay_thread, NULL, &DelayThread, NULL))) {
		fprintf(stderr, "Can't create Network Thread: [%s]\n", strerror(err));
		exit(EXIT_FAILURE);
	}
	pthread_join(delay_thread, NULL);
    
}

int senderMessage(char *packet,addressList *List){
    char buffer[BUFFER_SIZE];
    getSourceIP(buffer,packet);
    int Port=getSourcePort(packet);

    fprintf(stderr,"\nsenderMessage \nSource IP is %s\nPort is %d\n\n",buffer,Port);
    
    if(addressInList(buffer,Port,List)==1){
        return 1;
    }
}

void SendPacketToReceiver(){
    
}

void *DelayThread(void *param) {
    struct timespec timer,timer2;
    timer.tv_sec=0;
    timer.tv_nsec=rand()%10000000;
    timer2.tv_sec=0;
    timer2.tv_nsec=0;
    nanosleep(&timer,&timer2);

    SendPacketToReceiver();
}

void PrintStats(NetTraffic *Traffic){
    int TotalPackets=Traffic->SenderPackets+Traffic->ReceiverPackets;
    printf("\nCurrent Stats:\n");
    printf("Total Packets:    %d\n",TotalPackets);
    printf("Sender Packets:   %d\n",Traffic->SenderPackets);
    printf("Receiver Packets: %d\n",Traffic->ReceiverPackets);
    printf("Delayed Packets:  %d\n",Traffic->DelayedPackets);
    printf("Dropped Packets:  %d\n",Traffic->DroppedPackets);
    printf("Corrupt Packets:  %d\n",Traffic->CorruptPackets);
}