// Testing

#include <assert.h>

#include "udpPacket.h"

void printByte(char byte)
{
	printf("%d%d%d%d%d%d%d%d", (byte & 128) >> 7, 
	                           (byte & 64)  >> 6, 
	                           (byte & 32)  >> 5, 
	                           (byte & 16)  >> 4, 
	                           (byte & 8)   >> 3, 
	                           (byte & 4)   >> 2, 
	                           (byte & 2)   >> 1, 
	                           (byte & 1)         );
}

int main(void)
{

	const char *message1 = "Hello, World!\0"; // 2
	const char *message2 = "Lorem ipsum dolor sit amet, consectetur adipisicing elit." 
	                       "Accusantium natus assumenda similique excepturi et dolorem, "
	                       "quibusdam deleniti, eius, quisquam ratione mollitia autem "
	                       "alias repellendus. Quae corporis sint, placeat earum et?\0"; // 26
	const char *message3 = "Hey\0"; // 1
	const char *message4 = "How many segments are needed to send this message?\0"; // 6
	const char *message5 = "Six segments are needed.\0"; // 3

	assert(numberOfSegments(message1) == 2);
	assert(numberOfSegments(message2) == 26);
	assert(numberOfSegments(message3) == 1);
	assert(numberOfSegments(message4) == 6);
	assert(numberOfSegments(message5) == 3);

	char *packet1 = (char *) malloc(54);
	memset(packet1+44, 0+23, 1);
	char *packet2 = (char *) malloc(54);
	memset(packet2+44, 128+47, 1);

	printByte(packet1[44]); printf("\n");
	printByte(packet2[44]); printf("\n");

	assert(isCorrupt(packet1) == 0);
	assert(isCorrupt(packet2) == 1);

	assert(sequenceNumber(packet1) == 23);
	assert(sequenceNumber(packet2) == 47);

	char *content = "CoNtEnT!\0";

	char segmentBuffer[SEGMENT_LENGTH];
	char packetBuffer[PACKET_LENGTH];

	printf("%s\n", content);

	makeSegment(segmentBuffer, 43, content);
	printByte(segmentBuffer[0]);
	printf(" %s\n", segmentBuffer+1);

	makePacket(packetBuffer, "192.168.0.100", 60000, "127.0.0.1", 60001, segmentBuffer);

	char IPBuffer[16];
	getSourceIP(IPBuffer, packetBuffer);
	printf("%s\n", IPBuffer);

	printf("%d\n", getSourcePort(packetBuffer));

	getDestinationIP(IPBuffer, packetBuffer);
	printf("%s\n", IPBuffer);

	printf("%d\n", getDestinationPort(packetBuffer));

	getSegmentContent(segmentBuffer, packetBuffer);
	printf("%s\n", segmentBuffer);

	printf("%d\n", sequenceNumber(packetBuffer));

	printf("%d\n", isCorrupt(packetBuffer));
	
	printByte(getSegmentHeader(packetBuffer)); printf("\n");

	corruptPacket(packetBuffer);

	printf("%d\n", sequenceNumber(packetBuffer));
	printf("%d\n", isCorrupt(packetBuffer));
	
	printByte(getSegmentHeader(packetBuffer)); printf("\n");

	return 0;
}