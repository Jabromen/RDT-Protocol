/**
 * This file describes the functions to be implemented by an RDT receiver.
 *
 * @author Jeffrey Bromen
 * @author Raymond Fleming
 * @date 2/22/2017
 * @info Systems and Networks II
 * @info Project 2
 *
 */

#ifndef _RDT_RECEIVER_H
#define _RDT_RECEIVER_H

#include <time.h>
#include "definitions.h"
#include "udpPacket.h"
#include <math.h>
#include <pthread.h>

//Struct to store network traffic data
typedef struct NetTraffic
{
	int SenderPackets;
    int ReceiverPackets;
    int DelayedPackets;
    int CorruptPackets;
    int DroppedPackets;

} NetTraffic;

typedef struct socketPacket{
    int *fd;
    char *packet;
} socketPacket;

//Struct to store address lists of senders and receivers for recording purposes
typedef struct addressList
{
    int numberofSenders;
    int numberofReceivers;
    char recIPArray[NUM_HOSTS][IP_SIZE];
    int recPortArray[NUM_HOSTS];
    char sendIPArray[NUM_HOSTS][IP_SIZE];
    int sendPortArray[NUM_HOSTS];
} addressList;

/**
 * Allocates memory for and initializes NetTraffic struct.
 *
 * @return: pointer to NetTraffic struct
 */
NetTraffic *initializeNetworkTraffic();

/**
 * Determine if packet should be dropped
 * 
 * @param droppedPercent - integer value indicating percentage of dropped packets from commandline argument
 *
 * @return 0, if packet is not dropped and 1 otherwise
 */
int PacketDropped(int droppedPercent);

/**
 * Record that a packet was dropped into the Traffic struct
 * 
 * @param Traffic - Struct storing network traffic data
 *
 * @return void
 */
void RecordDropped(NetTraffic *Traffic);

/**
 * Determine if a packet should be corrupt
 * 
 * @param errorPercent - integer value indicating percentage of corrupt packets from commandline argument
 *
 * @return 0, packet is not corrupt. Return 1 otherwise
 */
int corrupt(int errorPercent);

/**
 * Record that a packet was corrupted into the Traffic struct
 * 
 * @param Traffic - Struct storing network traffic data
 *
 * @return void
 */
void RecordCorrupt(NetTraffic *Traffic);

/**
 * Test a random integer (between 0 and 100) against a test value
 * 
 * @param testVal - Percentage dropped/corrupt/delayed
 *
 * @return 0 for value between 0 and testval, and 1 for testval to 100
 */
int randomInt(int testVal);

/**
 * Determine if a packet should be delayed
 * 
 * @param localPort - 
 *
 * @return 0, 
 */
int PacketDelayed(int delayedPercent);

/**
 * Record that a packet was delayed into the Traffic struct
 * 
 * @param Traffic - Struct storing network traffic data
 *
 * @return void
 */
void RecordDelayed(NetTraffic *Traffic);

/**
 * Record sender and receiver packet traffic into Traffic struct
 * 
 * @param Traffic - Struct storing network traffic data
 * @param packet - char packet buffer
 * @param List - struct of known addresses for senders/receivers
 *
 * @return void 
 */
void RecordNetworkTraffic(NetTraffic *Traffic,char *packet,addressList *List);

/**
 * Start a delay thread for a packet
 * 
 * @param socketPacket - packed data structure containing packet and socket pointers 
 *
 * @return void
 */
void StartDelayThread(socketPacket *fdPacket);

/**
 * Print current traffic statistics to console
 * 
 * @param Traffic - Struct storing network traffic data 
 *
 * @return void
 */
void PrintStats(NetTraffic *Traffic);

/**
 * Delay for a random amount of time, then send packet to receiver
 * 
 * @param param - pointer to a socketPacket - packed data structure containing packet and socket pointers 
 *
 * @return void
 */
void *DelayThread(void *param);

/**
 * determine if passed message is from an unknown sender
 * 
 * @param packet - char buffer containing packet
 * @param List - struct of addresses for known senders and receivers
 *
 * @return 0, if not a message from an unknown sender and 1 if it is
 */
int senderMessage(char *packet,addressList *List);

/**
 * Determine if a message of from an unknown host
 * 
 * @param packet - char buffer containing packet
 * @param List - struct of addresses for known senders and receivers
 *
 * @return void
 */
void newHost(char *packet,addressList *List);

/**
 * add a new host to the address lists
 * 
 * @param toggle - switch for Sender, "Sndr" or Receiver, "Rcvr"
 * @param Port - integer value of port number of host
 * @param List - struct of addresses for known senders and receivers
 *
 * @return void
 */
void addHost(char *toggle,char *buffer,int Port,addressList *List);

/**
 * Determine if an address is in either list
 * 
 * @param buffer - string containing address to test
 * @param Port - integer value of port number of host
 * @param List - struct of addresses for known senders and receivers
 *
 * @return 0, if address is not found, 1 if it is a known sender, 2 if it is a known receiver
 */
int addressInList(char *buffer,int Port,addressList *List);

/**
 * Initialize the address lists and counts to zero
 * 
 * @param List - struct of addresses for known senders and receivers
 *
 * @return void
 */
void initializeAddressList(addressList *List);

/**
 * Forward the packet to the destination
 * 
 * @param block - packed data structure containing packet and socket pointer 
 *
 * @return void
 */
void SendPacketToReceiver(socketPacket *fdPacket);

/**
 * Receives a message from an RDT sender on a specified port.
 *
 * @param port - the number of the port on which the receiver listens to receive messages
 * @param lostPercent - int value describing percentage of lost packets
 * @param delayedPercent - int value describing percentage of delayed packets
 * @param errorPercent - int value describing percentage of corrupt packets
 *
 * @return the complete text message received by a sender or NULL if an error occurred
 */
void receiveMessage (int port,int lostPercent,int delayedPercent, int errorPercent);


#endif // _RDT_RECEIVER_H