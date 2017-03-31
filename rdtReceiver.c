/**
 * This file includes the implementation of the receiver RDT protocol
 *
 * @author Jeffrey Bromen
 * @author Raymond Fleming
 * @date 3/30/2017
 * @info Systems and Networks II
 * @info Project 2
 *
 */

#include "rdtReceiver.h"

void receiveMessage (int port)
{
	int fd, state;

	char message[MAX_MESSAGE_SIZE] = "";
	// Create and bind socket
	if ((fd = initializeSocket(port)) < 0)
	{
		fprintf(stderr, "Socket initialization failed\n");
		return;
	}
	// Set initial state
	state = RECV_0;

	while (1)
	{
		switch (state)
		{
			case RECV_0:
				// Receiving sequence 0
				if (recvState(message, fd, 0))
				{
					state = RECV_1;
				}
				break;

			case RECV_1:
				// Receiving sequence 1
				if (recvState(message, fd, 1))
				{
					state = RECV_0;
				}
				break;
		}
		// Print cumulative message
		printf("%s\n", message);
	}
}

int recvState(char *message, int fd, int seqNum)
{
	struct sockaddr retAddr;
	socklen_t addrlen = sizeof(retAddr);

	char recvContent[SEGMENT_LENGTH] = "";
	char segment[SEGMENT_LENGTH] = "";
	char packet[PACKET_LENGTH] = "";

	char recvBuffer[PACKET_LENGTH] = "";
	char srcAddress[INET_ADDRSTRLEN] = "";
	char destAddress[INET_ADDRSTRLEN] = "";

	int srcPort, destPort;
	int returnValue = 0;

	// Receive packet and store return address
	recvfrom(fd, recvBuffer, PACKET_LENGTH, 0, &retAddr, &addrlen);

	// If corrupt, don't send an ACK
	if (!isCorrupt(recvBuffer))
	{
		// If out-of-sequence, drop packet and make out-of-sequence ACK segment
		if (sequenceNumber(recvBuffer) != seqNum)
		{
			makeSegment(segment, ((seqNum + 1) % 2), "ACK");
		}
		// If correct sequence, retrieve packet contents and make correct ACK segment
		else
		{
			getSegmentContent(recvContent, recvBuffer);
			strcat(message, recvContent);

			makeSegment(segment, seqNum, "ACK");
			// Return 1 to move to next state
			returnValue = 1;
		}
		// Retrieve the source and destination address/port from header
		getSourceIP(srcAddress, recvBuffer);
		srcPort = getSourcePort(recvBuffer);
		getDestinationIP(destAddress, recvBuffer);
		destPort = getDestinationPort(recvBuffer);
		// Swap the source and destination address in header when making ACK packet
		makePacket(packet, destAddress, destPort, srcAddress, srcPort, segment);
		// Send packet to the return address
		sendto(fd, packet, PACKET_LENGTH, 0, &retAddr, addrlen);
	}
	// Return 1 if moving to next state, 0 if repeating current state
	return returnValue;
}