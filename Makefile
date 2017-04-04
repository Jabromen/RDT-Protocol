CFLAGS = -g -Wall
CC = gcc

all: network sender receiver

network: rdtNetwork.c udpPacket.c
	$(CC) $(CFLAGS) -pthread rdtNetwork.c udpPacket.c -lm network.c -o network

sender: rdtSender.c udpPacket.c
	$(CC) $(CFLAGS) rdtSender.c udpPacket.c -lm sender.c -o sender

receiver: rdtReceiver.c udpPacket.c
	$(CC) $(CFLAGS) rdtReceiver.c udpPacket.c -lm receiver.c -o receiver

.PHONY: clean
clean:
	rm test sender receiver
