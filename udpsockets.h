#ifndef _UDPSOCKETS_H
#define _UDPSOCKETS_H

#include <stdio.h>
#include <sys/socket.h>
#include <string.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <netdb.h>
#include <unistd.h>

#define NETWORK_BUFF_SIZE 256

typedef struct udpsocket_t {

	int fd;

	socklen_t addrlen;

	struct sockaddr_in myaddr;
	struct sockaddr_in destaddr;
	struct sockaddr_in remaddr;

} udpsocket_t;

typedef struct addrport_t {

	u_long  oldaddr;
	u_short oldport;
	u_long  newaddr;
	u_short newport;

} addrport_t;

typedef struct tokn_message_t {

	char *tokn_buffer;

	int argc;
	char *argv[6];

} tokn_message_t;

/*
 * Creates and initializes a client udpsocket_t with a destination address and port set.
 */
udpsocket_t *initUdpSocketClient(const char *hostname, u_short dest_port, u_short my_port);

/*
 * Creates and initializes a udpsocket_t struct with no destination set.
 */
udpsocket_t *initUdpSocket(u_short my_port);

/*
 * Sets the destination address and port of a udpsocket_t struct.
 */
void setUdpDestination(u_long dest_addr, u_short dest_port, udpsocket_t *sckt);

/*
 * Formats an address string in the form:
 * "myAddress myPort destinationAddress destinationPort"
 */
void makeAddrString(char *buffer, const char *prefix, struct sockaddr_in *myaddr, struct sockaddr_in *destaddr);

int parseMessage(tokn_message_t *tm, addrport_t *ap);

int checkDestination(udpsocket_t *sckt, addrport_t *ap);

/*
 * Sends a message string to the destination set in sckt struct.
 */
int sendMessage(const char *message, udpsocket_t *sckt);

/*
 * 
 */
int receiveMessage(char *buffer, const int buffSize, udpsocket_t *sckt);

void closeSocket(udpsocket_t *sckt);

tokn_message_t *initToknMessageStruct();

void clearToknMessage(tokn_message_t *tm);

int tokenizeMessage(const char *message, tokn_message_t *tm);

int compareAddresses(udpsocket_t *sckt, addrport_t *ap);

void freeToknMessage(tokn_message_t *tm);

#endif