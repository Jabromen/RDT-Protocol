#include "udpsockets.h"
#include <time.h>
#include "definitions.h"
#include "udpPacket.h"

#ifdef __APPLE__
#include <mach/semaphore.h>
#include <mach/task.h>
#define sem_init(a,b,c)     semaphore_create(mach_task_self(), (semaphore_t *)a, SYNC_POLICY_FIFO, c)
#define sem_destroy(a)      semaphore_destroy(mach_task_self(), *((semaphore_t *)a))
#define sem_post(a)         semaphore_signal(*((semaphore_t *)a))
#define sem_wait(a)         semaphore_wait(*((semaphore_t *)a))
#define sem_t               semaphore_t
#else
#include <semaphore.h>
#endif

typedef struct NetTraffic
{
	int SenderPackets;
    int ReceiverPackets;
    int DelayedPackets;
    int CorruptPackets;
    int DroppedPackets;

} NetTraffic;

int PacketDropped(int droppedPercent);
void RecordDropped(NetTraffic *Traffic);
int corrupt(int errorPercent);
void RecordCorrupt(NetTraffic *Traffic);
int randomInt(int testVal);
int PacketDelayed(int delayedPercent);
void RecordDelayed(NetTraffic *Traffic);
void RecordNetworkTraffic(NetTraffic *Traffic);
void StartDelayThread();
void SendPacketToReceiver();
void PrintStats(NetTraffic *Traffic);
void *DelayThread(void *param);

sem_t netTraf;

int main (int argc, char** argv) {
    char buffer[PACKET_LENGTH];
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
    
    
    
    if(sem_init(&netTraf, 0, 1) != 0) {
		printf("Sem init failed.\n");
		exit(EXIT_FAILURE);
    
    int i=0;
    
    
    int recvlen;
    
    srand(time(NULL));
    
    // Create UDP socket
    udpsocket_t *sckt = initUdpSocket(my_port);

    if (!sckt)
        exit(EXIT_FAILURE);
        
    while(1){
        
        //Print Network Stats every 2 packets
        for(i=0;i<2;i++){
        recvlen=receiveMessage(buffer, PACKET_LENGTH, sckt);
        RecordNetworkTraffic(Traffic);
        
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
    }
    PrintStats(Traffic);
    }
}
}

int PacketDropped(int droppedPercent){
    return randomInt(droppedPercent);
    
}

void RecordDropped(NetTraffic *Traffic){
    sem_wait(&netTraf);
    Traffic->DroppedPackets++;
    sem_post(&netTraf);
}

int corrupt(int errorPercent){
    return randomInt(errorPercent);
}

void RecordCorrupt(NetTraffic *Traffic){
    sem_wait(&netTraf);
    Traffic->CorruptPackets++;
    sem_post(&netTraf);
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
    sem_wait(&netTraf);
    Traffic->DelayedPackets++;
    sem_post(&netTraf);
}



void RecordNetworkTraffic(NetTraffic *Traffic){
    sem_wait(&netTraf);
    if(senderMessage()){
        Traffic->SenderPackets++;
    }
    else{
        Traffic->ReceiverPackets++;
    }
    sem_post(&netTraf);
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

void SendPacketToReceiver(){
    
}

void *DelayThread(void *param) {
    struct timespec timer,timer2;
    timer.tv_sec=0;
    timer.tv_nsec=rand()%10000000;
    timer2.tv_sec=0;
    timer2.tv_nsec=0;
    nanosleep(&timer,&timer2);

    sendPacketToReceiver();
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