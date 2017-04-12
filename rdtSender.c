/**
 * This file includes the implementation of the sender RDT protocol
 *
 * @author Jeffrey Bromen
 * @author Raymond Fleming
 * @date 3/30/2017
 * @info Systems and Networks II
 * @info Project 2
 *
 */

#include "rdtSender.h"

int sendMessage(int localPort, char* netwhost, int netwPort, char* desthost, int destPort, const char* message)
{
	int fd, i, state;

	char ownAddress[INET_ADDRSTRLEN];
	char netwAddress[INET_ADDRSTRLEN];
	char destAddress[INET_ADDRSTRLEN];

	char segment[SEGMENT_LENGTH];
	char packet[PACKET_LENGTH];
	char recvBuffer[PACKET_LENGTH];

	message_segments_t *segments;
	// Get IP addresses of sender, network, and receiver in dot format
	getOwnAddress(ownAddress);
	getAddress(destAddress, desthost);
	getAddress(netwAddress, netwhost);
	// Create and bind socket
	if ((fd = initializeSocket(localPort)) < 0)
	{
		fprintf(stderr, "Socket initialization failed\n");
		return -1;
	}
	// Divide message into segments to send in packets
	segments = segmentMessage(message);
	// Set initial segment number and state
	i = 0;
	state = SEND_0;
	// Loop until all segments are sent and acknowledged
	while (i < segments->length)
	{
		switch (state)
		{
			case SEND_0:
				// Make packet with sequence 0
				makeSegment(segment, 0, segments->segment[i]);
				makePacket(packet, ownAddress, localPort, destAddress, destPort, segment);

				state = ACK_0;
				break;

			case ACK_0:
				// Send packet with sequence 0 and wait for ACK with sequence 0
				ackState(recvBuffer, packet, fd, 0, netwAddress, netwPort);

				state = SEND_1;
				i++;
				break;

			case SEND_1:
				// Make packet with sequence 1
				makeSegment(segment, 1, segments->segment[i]);
				makePacket(packet, ownAddress, localPort, destAddress, destPort, segment);

				state = ACK_1;
				break;

			case ACK_1:
				// Send packet with sequence 1 and wait for ACK with sequence 1
				ackState(recvBuffer, packet, fd, 1, netwAddress, netwPort);

				state = SEND_0;
				i++;
				break;
		}
	}
	freeMessageSegments(segments);
	close(fd);

	return 0;
}

void ackState(char *recvBuffer, const char *packet, int fd, int seqNum, const char *sendAddress, int sendPort)
{
	int selectval = 0;
	fd_set fdset;

	struct timeval timeout;

	while (1)
	{
		// If first loop, send packet and start timer. Or if timed out, resend packet and restart timer.
		if (selectval == 0)
		{
			sendPacket(fd, packet, sendAddress, sendPort);

			setTimeout(&timeout, SEND_TIMEOUT_SEC, SEND_TIMEOUT_USEC);

			FD_ZERO(&fdset);
			FD_SET(fd, &fdset);
			selectval = select(fd+1, &fdset, NULL, NULL, &timeout);
		}
		// Check if ready to receive a packet.
		if (selectval == 1)
		{
			recv(fd, recvBuffer, PACKET_LENGTH, 0);	
			// If corrupt or out-of-sequence, do nothing and resume timer.
			if (isCorrupt(recvBuffer) || sequenceNumber(recvBuffer) != seqNum)
			{
				FD_ZERO(&fdset);
				FD_SET(fd, &fdset);
				selectval = select(fd+1, &fdset, NULL, NULL, &timeout);
			}
			// If not corrupt and correct sequence, exit the loop.
			else
			{
				break;
			}
		}
		// Exit loop if select statement returns an error.
		if (selectval == -1)
		{
			fprintf(stderr, "select() error.\n");
			break;
		}
	}
}

void setTimeout(struct timeval *tv, int sec, int usec)
{
	tv->tv_sec = sec;
	tv->tv_usec = usec;
}

void getUserMessage(char *message) {

	char line[BUFFER_SIZE] = "";

	// Clear message buffer
	memset(message, 0, MAX_MESSAGE_SIZE);

	// Prompt user for message
	printf("\nEnter message to send to receiver (hit enter twice to confirm message):\n");

	// Read user message from stdin
	while (1) {
		// Read line from stdin
		fgets(line, BUFFER_SIZE, stdin);
		// Break if enter key is hit twice after a message was entered
		if (!strcmp(line, "\n") && strcmp(message, "\0")) {
			break;
		}
		// Append line to end of message
		strcat(message, line);
		// Clear line buffer
		memset(line, 0, strlen(line));
	}
}