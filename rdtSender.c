#include "udpsockets.h"
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

void *sendThread(void *param);
void *ACKThread(void *param);
void GetWindowLock();
void GetMessage(char *message,char *line);
void SendNextPacket();
void GiveWindowLock();

sem_t window_lock;
sem_t ACK_Lock;

int main (int argc, char** argv) {
    
    char message[MAX_MESSAGE_SIZE] = "";
	char line[BUFFER_SIZE] = "";
	int numSegments=0;
	int ACK;
	int err;
	
    // Check if enough arguments
    if (argc < 3) {
        printf("ERROR: Not enough arguments. Use format:\n"
               "\"portNum numberOfHosts\"\n");
        exit(EXIT_FAILURE);
    }

    GetMessage(message,line);
    numSegments= numberOfSegments(message);
    
    
   	pthread_t send_thread;
	pthread_t ACK_thread;

    if(sem_init(&window_lock, 0, 0) != 0) {
		printf("Sem init failed.\n");
		exit(EXIT_FAILURE);
	}
	
	if(sem_init(&ACK_Lock, 0, 0) != 0) {
		printf("Sem init failed.\n");
		exit(EXIT_FAILURE);
	}

	if ((err = pthread_create(&send_thread, NULL, &sendThread, NULL))) {
		fprintf(stderr, "Can't create Network Thread: [%s]\n", strerror(err));
		exit(EXIT_FAILURE);
	}

	if ((err = pthread_create(&ACK_thread, NULL, &ACKThread, NULL))) {
		fprintf(stderr, "Can't create UI thread: [%s]\n", strerror(err));
		exit(EXIT_FAILURE);
	}

	// Wait until threads exit
	pthread_join(send_thread, NULL);
	pthread_join(ACK_thread, NULL);
    
    sem_destroy(&window_lock);
    
    exit(EXIT_SUCCESS);
}

void GetMessage(char *message,char *line){
    

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



void *sendThread(void *param) {
	int ACK;
    GetWindowLock();
    SendNextPacket();
    sem_wait(&ACK_Lock);
    ACK=0;
    sem_post(&ACK_Lock);
    StartTimeout();
}

void *ACKThread(void *param){
	int ACK;
    GiveWindowLock();
    WaitForAck();
    if(!PacketCorrupt())
    {
        sem_wait(&ACK_Lock);
        ACK=1;
        sem_post(&ACK_Lock);
        GiveWindowLock();
    }
}

void GetWindowLock(){
    sem_wait(&window_lock);
}

void SendNextPacket(){
//    makePacket();
 //   sendMessage(message,sckt);
}


void GiveWindowLock(){
    sem_post(&window_lock);
}