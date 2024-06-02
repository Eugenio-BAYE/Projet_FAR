#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <signal.h>
#include "threads.h"
#include "command.h"


#define msgLength 256
int dS;
int is_running = 1;
int port;
char *addr;
int input_break = 0;

void break_input(){
  input_break = 1;
}

void resume_input(){
  input_break = 0;
}

int get_dS() {
    return dS;
}

void set_dS(int value) {
    dS = value;
}

int get_is_running() {
    return is_running;
}

void set_is_running(int value) {
    is_running = value;
}

void handle_sigint(int sig) {
  printf("\nCtrl+C pressed. Exiting...\n");
  shutdown(get_dS(), 2);
  exit(1);
}

int get_port() {
    return port;
}

void set_port(int p) {
    port = p;
}

const char* get_addr() {
    return addr;
}

void set_addr(const char *a) {
    free(addr);
    addr = strdup(a);
}

void handle_local_sigint(int sig) {
    printf("\nCtrl+C pressed. Exiting...\n");
    exit(1);
}

int connect_socket(const char * arg1, int arg2 ){

    signal(SIGINT, handle_local_sigint);
    // Create the socket
    int dS = socket(PF_INET, SOCK_STREAM, 0);
    printf("Attempting connection with the server...\n");
    if (dS == -1){
        perror("Error creating socket");
        exit(0);
    }
    // Create the adress
    struct sockaddr_in aS;
    aS.sin_family = AF_INET;
    inet_pton(AF_INET,arg1,&(aS.sin_addr)) ;
    aS.sin_port = htons(arg2) ;
    socklen_t lgA = sizeof(struct sockaddr_in) ;
    // Connect the socket
    if (connect(dS, (struct sockaddr *) &aS, lgA) == -1){
        perror("Error connecting socket");
        exit(0);
    }
    puts ("Socket connected");
    return dS;
}

int receive_memset(int dSC, char msg[], int msgLenght){
  memset(msg, '\0', msgLenght);
  int received_size = recv(dSC, msg, msgLenght, 0);
  return received_size;
}

int send_msg(int dS, char* buffer, size_t input_length) {
    buffer[input_length - 1] = '\0';

    int send_size = send(dS, &input_length, sizeof(size_t), 0);
    if (send_size <= 0) {
        if (send_size == 0) {
            puts("Server disconnected when sending size");
        } else {
            perror("Error sending size");
        }
        free(buffer);
        return -1;
    }

    int send_message = send(dS, buffer, input_length, 0);
    if (send_message <= 0) {
        if (send_message == 0) {
            puts("Server disconnected when sending message");
        } else {
            perror("Error sending message");
        }
        free(buffer);
        return -1;
    }

    free(buffer);
    return 0;
}

void* loop_send_msg(void* args) {
    struct thread_args * t_args = (struct thread_args *) args;
    int dS = t_args->dS;

    while (get_is_running() == 1) {
        while(input_break){
      sleep(1);
    }
        char * buffer = malloc(msgLength);
        if (buffer == NULL) {
            perror("Error allocating memory for buffer");
            pthread_exit(0);
        }

        if (fgets(buffer, msgLength, stdin) == NULL) {
            puts("Error reading or end of file detected");
            free(buffer);
            pthread_exit(0);
        }

        size_t input_length = strlen(buffer);
        execute_command(buffer, dS);
        if (send_msg(dS, buffer, input_length) == -1) {
            pthread_exit(0);
        }
    }
    return NULL;
}

int receive_msg(int dS) {
    size_t input_length;

    int receive_size = recv(dS, &input_length, sizeof(size_t), 0);
    if (receive_size <= 0) {
        if (receive_size == 0) {
            puts("Server disconnected");
        } else {
            perror("Error receiving size");
        }
        return -1;
    }

    if (input_length > 1024) {
        fprintf(stderr, "Error: Message size %zu is too large\n", input_length);
        return -1;
    }

    char * msg = malloc(input_length);
    if (msg == NULL) {
        perror("Error allocating memory for msg");
        return -1;
    }

    int receive_message = receive_memset(dS, msg, input_length);
    if (receive_message <= 0) {
        if (receive_message == 0) {
            puts("Server disconnected");
        } else {
            perror("Error receiving message");
        }
        free(msg);
        return -1;
    }
    if (strcmp(msg, "@end") == 0) {
        free(msg);
        return 1;
    }

    puts(msg);
    free(msg);
    return 0;
}

void* loop_receive_msg(void* args) {
    struct thread_args * t_args = (struct thread_args *) args;
    int dS = t_args->dS;

    while (get_is_running() == 1) {
        if (receive_msg(dS) == -1) {
            pthread_exit(0);
        }
    }
    return NULL;
}
