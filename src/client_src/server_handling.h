#ifndef SERVER_HANDLING_H
#define SERVER_HANDLING_H

#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <signal.h>

#define MSG_LENGTH 256

typedef struct {
    int dS;
} thread_args;

extern int dS;
extern int is_running;
extern int port;
extern char *addr;

int get_port();
void set_port(int port);
const char* get_addr();
void set_addr(const char *addr);
int get_dS();
void set_dS(int value);
int get_is_running();
void set_is_running(int value);

void handle_sigint(int sig);
void handle_local_sigint(int sig);

// Fonctions de gestion de socket
int connect_socket(const char *ip, int port);

// Fonctions de communication
int receive_memset(int dSC, char msg[], int msgLength);
int send_msg(int dS, char* buffer, size_t input_length);
int receive_msg(int dS);

// Fonctions de thread
void* loop_send_msg(void* args);
void* loop_receive_msg(void* args);

#endif // SERVER_HANDLING_H
