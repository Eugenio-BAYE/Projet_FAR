CC = gcc
CFLAGS = -o
SRCS = src/*.c

all: server

server : server.c
	$(CC) $(CFLAGS) $@ $^ $(SRCS)
