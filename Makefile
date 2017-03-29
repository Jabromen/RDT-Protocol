CFLAGS = -g -Wall
CC = gcc

all: test sender receiver

test: rdtSender.c udpPacket.c test.c
	$(CC) $(CFLAGS) rdtSender.c udpPacket.c -lm test.c -o test

sender: rdtSender.c udpPacket.c
	$(CC) $(CFLAGS) rdtSender.c udpPacket.c -lm sender.c -o sender

receiver: rdtReceiver.c udpPacket.c
	$(CC) $(CFLAGS) rdtReceiver.c udpPacket.c -lm receiver.c -o receiver

.PHONY: clean
clean:
	rm test sender receiver