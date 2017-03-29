/*
	This is not complete, but it does function correctly with the sender process.

	TO DO:
		Update code to work with the network process
		Refactor switch statements to clean up code
*/

#include "rdtReceiver.h"

char* receiveMessage (int port)
{
	int fd, state, recvlen;

	char ownAddress[INET_ADDRSTRLEN];
	int ownPort;
	char retAddress[INET_ADDRSTRLEN];
	int retPort;

	char segment[SEGMENT_LENGTH];
	char packet[PACKET_LENGTH];
	char recvBuffer[PACKET_LENGTH];
	char segBuffer[SEGMENT_LENGTH] = "";
	char message[2048] = "";

	if ((fd = initializeSocket(port, 0)) < 0)
	{
		fprintf(stderr, "Socket initialization failed\n");
		return NULL;
	}

	state = RECV_0;

	while (1)
	{
		switch (state)
		{
			case RECV_0:
				recvlen = recv(fd, recvBuffer, PACKET_LENGTH, 0);
				if (isCorrupt(recvBuffer) || sequenceNumber(recvBuffer) != 0)
				{
					// Do nothing
				}
				else if (sequenceNumber(recvBuffer) == 0)
				{
					getSegmentContent(segBuffer, recvBuffer);
					strcat(message, segBuffer);
					printf("%s\n", message);

					makeSegment(segment, 0, "ACK");
					getSourceIP(retAddress, recvBuffer);
					retPort = getSourcePort(recvBuffer);
					getDestinationIP(ownAddress, recvBuffer);
					ownPort = getDestinationPort(recvBuffer);
					makePacket(packet, ownAddress, ownPort, retAddress, retPort, segment);
					sendPacket(fd, packet, retAddress, retPort);

					state = RECV_1;
				}
				break;

			case RECV_1:
				recvlen = recv(fd, recvBuffer, PACKET_LENGTH, 0);
				if (isCorrupt(recvBuffer) || sequenceNumber(recvBuffer) != 1)
				{
					// Do nothing
				}
				else if (sequenceNumber(recvBuffer) == 1)
				{
					getSegmentContent(segBuffer, recvBuffer);
					strcat(message, segBuffer);
					printf("%s\n", message);

					makeSegment(segment, 1, "ACK");
					getSourceIP(retAddress, recvBuffer);
					retPort = getSourcePort(recvBuffer);
					getDestinationIP(ownAddress, recvBuffer);
					ownPort = getDestinationPort(recvBuffer);
					makePacket(packet, ownAddress, ownPort, retAddress, retPort, segment);
					sendPacket(fd, packet, retAddress, retPort);

					state = RECV_0;
				}
				break;
		}
	}

	return message;
}