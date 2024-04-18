CC = gcc
CFLAGS = -o
SRCSSERV = src/server_src/*.c

all: server client

server : server.c
	$(CC) $(CFLAGS) $@ $^ $(SRCSSERV)

client : client.c
	$(CC) $(CFLAGS) $@ $^ 

clean:
	rm -f server client
