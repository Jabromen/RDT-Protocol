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
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#define MASK_CORRUPT  128  // 10000000
#define MASK_SEQUENCE 127  // 01111111

#define SEGMENT_LENGTH 10
#define PACKET_LENGTH  54
#define TIMEOUT_MS     25
#define IP_SIZE         4
#define PORT_SIZE       2

#define MAX_MESSAGE_SIZE 512
#define BUFFER_SIZE      512

#endif // _DEFINITIONS_H