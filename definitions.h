/**
 * This file includes the definitions and libraries used in this project.
 *
 * @author Jeffrey Bromen
 * @author Raymond Fleming
 * @date 3/8/2017
 * @info Systems and Networks II
 * @info Project 2
 *
 */

#ifndef _DEFINITIONS_H
#define _DEFINITIONS_H

#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

// Bit masks for segment header
#define MASK_CORRUPT  128  // 10000000
#define MASK_SEQUENCE 127  // 01111111

// Number of bytes in segments and packets
#define SEGMENT_LENGTH 10
#define PACKET_LENGTH  54

// Constants used in recording network traffic
#define IP_SIZE         4
#define PORT_SIZE       2
#define NUM_HOSTS      20

// Number of bytes in message buffers
#define MAX_MESSAGE_SIZE 512
#define BUFFER_SIZE      512

// Timeout values for sender
#define SEND_TIMEOUT_SEC  0
#define SEND_TIMEOUT_USEC 50000

// Scaling for network delay vs sender timout
#define NETWORK_DELAY_SCALE 5

// Switch statement values for sender
#define SEND_0 1
#define SEND_1 2

// Switch statement values for receiver
#define RECV_0 1
#define RECV_1 2

#endif // _DEFINITIONS_H