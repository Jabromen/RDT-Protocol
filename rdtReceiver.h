/**
 * This file describes the functions to be implemented by an RDT receiver.
 *
 * @author Jeffrey Bromen
 * @author Raymond Fleming
 * @date 2/22/2017
 * @info Systems and Networks II
 * @info Project 2
 *
 */

#ifndef _RDT_RECEIVER_H
#define _RDT_RECEIVER_H

#include "definitions.h"
#include "udpPacket.h"

/**
 * Receives a message from an RDT sender on a specified port.
 *
 * @param port - the number of the port on which the receiver listens to receive messages
 */
void receiveMessage (int port);

/**
 * Function contains the sequence of steps for the receiver state.
 * Handles the receiving of a single packet of a particular sequence.
 *
 * @param message - buffer containing the message being received. packet contents will be appended to the end.
 * @param fd      - file descriptor for the socket being used.
 * @param seqNum  - the current sequence number being received.
 *
 * @return returns 1 if correctly received packet, 0 otherwise.
 */
int recvState(char *message, int fd, int seqNum);

#endif // _RDT_RECEIVER_H