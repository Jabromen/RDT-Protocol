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

// Timeout values for sender
#define SEND_TIMEOUT_SEC  1
#define SEND_TIMEOUT_USEC 0

// Switch statement values for sender
#define SEND_0 1
#define ACK_0  2
#define SEND_1 3
#define ACK_1  4

// Switch statement values for receiver
#define RECV_0 1
#define RECV_1 2

#endif // _DEFINITIONS_H