/**
 * This file implements the functions used for operating on the UDP packets
 *
 * @author Jeffrey Bromen
 * @author Raymond Fleming
 * @date 3/8/2017
 * @info Systems and Networks II
 * @info Project 2
 *
 */

#include "udpPacket.h"

int numberOfSegments(const char *message)
{
	// Get the number of characters in the message, including NULL terminator.
	int len = strlen(message) + 1;
	// Return the number of segments needed to send message.
	// Subtract 1 from segment length to account for header byte.
	return (int) ceil(len / (SEGMENT_LENGTH - 1.0));
}

int isCorrupt(const char *packet)
{
	char header;
	// Copy byte containing header
	memcpy(&header, packet+44, 1);
	// Return 1 if corrupt flag is set, 0 otherwise
	return ((header & MASK_CORRUPT) > 0) ? 1 : 0; 
}

int sequenceNumber(const char *packet)
{
	char header;
	// Copy byte containing header
	memcpy(&header, packet+44, 1);
	// Return sequence number
	return header & MASK_SEQUENCE;
}

void getSourceIP(char *buffer, const char *packet)
{
	// Extract source IP address
	memcpy(buffer, packet, 16);
}

int getSourcePort(const char *packet)
{
	char port[6];
	// Extract source port number
	memcpy(port, packet+16, 6);
	// Return port number as integer
	return atoi(port);
}

void getDestinationIP(char *buffer, const char *packet)
{
	// Extract destination IP address
	memcpy(buffer, packet+22, 16);
}

int getDestinationPort(const char *packet)
{
	char port[6];
	// Extract destination port number
	memcpy(port, packet+38, 6);
	// Return port number as integer
	return atoi(port);
}

void getSegment(char *buffer, const char *packet)
{
	// Extract entire segment
	memcpy(buffer, packet+44, SEGMENT_LENGTH);
}

char getSegmentHeader(const char *packet)
{
	// Extract segment header
	return packet[44];
}

void getSegmentContent(char *buffer, const char *packet)
{
	// Extract segment content
	memcpy(buffer, packet+45, SEGMENT_LENGTH - 1);
}

void makeSegment(char *buffer, const int sequenceNum, const char *content)
{
	// Clear buffer
	memset(buffer, 0, SEGMENT_LENGTH);
	// Store segment header
	memset(buffer, sequenceNum % 128, 1);
	// Store segment content
	memcpy(buffer+1, content, SEGMENT_LENGTH - 1);
}

void makePacket(char *buffer, const char *srcIP, const int srcPort, const char *destIP, const int destPort, const char *segment)
{
	// Clear buffer
	memset(buffer, 0, PACKET_LENGTH);
	// Store source IP address and port number
	memcpy(buffer, srcIP, 16);
	snprintf(buffer+16, 6, "%d", srcPort);
	// Store destination IP address and port number
	memcpy(buffer+22, destIP, 16);
	snprintf(buffer+38, 6, "%d", destPort);
	// Store segment
	memcpy(buffer+44, segment, SEGMENT_LENGTH);
}

void corruptPacket(char *packet)
{
	// Extract segment header
	char header = getSegmentHeader(packet);
	// Set corrupt flag
	header |= MASK_CORRUPT;
	// Store updated header back in packet
	memset(packet+44, header, 1);
}

message_segments_t *segmentMessage(const char *message)
{
	message_segments_t *segments = (message_segments_t *) malloc(sizeof(message_segments_t));

	segments->length = numberOfSegments(message);

	segments->segment = (char **) malloc(sizeof(char *) * segments->length);

	int i;
	for (i = 0; i < segments->length; i++)
	{
		segments->segment[i] = (char *) malloc(sizeof(char) * SEGMENT_LENGTH);
		strncpy(segments->segment[i], message + (i * (SEGMENT_LENGTH - 1)), (SEGMENT_LENGTH- 1));
		segments->segment[i][SEGMENT_LENGTH] = '\0';
	}

	return segments;
}

void freeMessageSegments(message_segments_t *segments)
{
	int i;
	for (i = 0; i < segments->length; i++)
	{
		free(segments->segment[i]);
	}
	free(segments->segment);
	free(segments);
}