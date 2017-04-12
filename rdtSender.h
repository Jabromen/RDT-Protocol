/**
 * This file describes the functions to be implemented by an RDT sender.
 *
 * @author Jeffrey Bromen
 * @author Raymond Fleming
 * @date 2/22/2017
 * @info Systems and Networks II
 * @info Project 2
 *
 */

#ifndef _RDT_SENDER_H
#define _RDT_SENDER_H

#include "definitions.h"
#include "udpPacket.h"

/**
 * Sends a message to an RDT receiver on a specified host and port using a network simulator
 * program for transporting messages to the receiver.
 * 
 * @param localPort - the local port to bind the socket
 * @param netwHost  - the name of the host that runs the network simulator
 * @param netwPort  - the number of the port on which the network simulator listens
 * @param destHost  - the name of the host that runs the receiver
 * @param destPort  - the number of the port on which the receiver listens
 * @param message   - the entire text message to be sent to the receiver; the message is \0 terminated
 *
 * @return 0, if no error; otherwise, a negative number indicating the error
 */
int sendMessage(int localPort, char* netwhost, int netwPort, char* desthost, int destPort, const char* message);

/**
 * Helper function for sending a packet and waiting until it is acknowledged.
 *
 * @param recvBuffer  - buffer that holds received packet
 * @param packet      - packet being sent
 * @param fd          - file descriptor for socket being used
 * @param seqNum      - sequence number of packet being sent
 * @param sendAddress - IP address of the network or receiver that the packet is being sent to
 * @param sendPort    - port number of the network or receiver that the packet is being sent to
 */
void sendState(char *recvBuffer, const char *packet, int fd, int seqNum, const char *sendAddress, int sendPort);

/**
 * Sets the parameters of a timeval structure.
 *
 * @param tv   - timeval structure being set
 * @parma sec  - number of seconds
 * @param usec - number of microseconds
 */
void setTimeout(struct timeval *tv, int sec, int usec);

/**
 * Gets message to send to Receiver from User
 * 
 * @param message - buffer where user message will be stored
 */
void getUserMessage(char *message);

#endif // _RDT_SENDER_H




