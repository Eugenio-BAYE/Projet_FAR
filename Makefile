CC = gcc
CFLAGS = -o
SRCSSERV = src/server_src/*.c

all: server

server : server.c
	$(CC) $(CFLAGS) $@ $^ $(SRCSSERV)
