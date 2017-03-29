/* 
	This is not completely finished, but it does function mostly correctly.

	TO DO:
		Change timeout implementation
		Refactor switch statements into individual functions to clean up code
 
	As it is, time timeout resets whenever it receives any packet.
	We want it to continue the same timeout if it receives a corrupt or out of sequence packet.

*/

#include "rdtSender.h"

int sendMessage(int localPort, char* netwhost, int netwPort, char* desthost, int destPort, const char* message)
{
	int fd, i, state, recvlen;

	char ownAddress[INET_ADDRSTRLEN];
	char netwAddress[INET_ADDRSTRLEN];
	char destAddress[INET_ADDRSTRLEN];

	char segment[SEGMENT_LENGTH];
	char packet[PACKET_LENGTH];
	char recvBuffer[PACKET_LENGTH];

	message_segments_t *segments;

	getOwnAddress(ownAddress);
	getAddress(destAddress, desthost);
	getAddress(netwAddress, netwhost);

	if ((fd = initializeSocket(localPort, 1)) < 0)
	{
		fprintf(stderr, "Socket initialization failed\n");
		return -1;
	}

	segments = segmentMessage(message);
	i = 0;
	state = SEND_0;

	while (i < segments->length)
	{
		switch (state)
		{
			case SEND_0:
				makeSegment(segment, 0, segments->segment[i]);
				makePacket(packet, ownAddress, localPort, destAddress, destPort, segment);
				sendPacket(fd, packet, netwAddress, netwPort);
				state = ACK_0;
				break;

			case ACK_0:
				recvlen = recv(fd, recvBuffer, PACKET_LENGTH, 0);

				if (recvlen < 0)
				{
					sendPacket(fd, packet, netwAddress, netwPort);
				}
				else if (isCorrupt(recvBuffer) || sequenceNumber(recvBuffer) != 0)
				{
					// Do Nothing
				}
				else if (sequenceNumber(recvBuffer) == 0)
				{
					state = SEND_1;
					i++;
				}
				break;

			case SEND_1:
				makeSegment(segment, 1, segments->segment[i]);
				makePacket(packet, ownAddress, localPort, destAddress, destPort, segment);
				sendPacket(fd, packet, netwAddress, netwPort);
				state = ACK_1;
				break;

			case ACK_1:
				recvlen = recv(fd, recvBuffer, PACKET_LENGTH, 0);

				if (recvlen < 0)
				{
					sendPacket(fd, packet, netwAddress, netwPort);
				}
				else if (isCorrupt(recvBuffer) || sequenceNumber(recvBuffer) != 1)
				{
					// Do Nothing
				}
				else if (sequenceNumber(recvBuffer) == 1)
				{
					state = SEND_0;
					i++;
				}
				break;
		}
	}

	return 0;
}