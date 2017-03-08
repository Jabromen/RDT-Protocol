CFLAGS = -g -Wall
CC = gcc

test: udpPacket.c test.c
	$(CC) $(CFLAGS) udpPacket.c -lm test.c -o test

.PHONY: clean
clean:
	rm test