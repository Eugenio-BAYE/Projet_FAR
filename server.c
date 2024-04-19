// Include packages
#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
// Include files
#include "src/server_src/client_handling.h" 
#include "src/server_src/server_handling.h"
// ------------------------------------------------------

#define globalMessageLenght 256
#define MAX_CLIENT 10

int clients[MAX_CLIENT]; // Array to store client socket descriptors
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER; // Mutex for thread-safe access to clients array
int nbr_of_clients;

struct ThreadArgs {
    int dSC_sender;
    int dSC_receiver;
};

void broadcast_message(int sender, char *message, int message_size) {
  printf("Message broadcasting\n");
    pthread_mutex_lock(&mutex);
    for (int i = 0; i < MAX_CLIENT; i++) {
        if (clients[i] != 0 && clients[i] != sender) {
            send(clients[i], message, message_size, 0);
        }
    }
    pthread_mutex_unlock(&mutex);
}

void* handle_client(void* args) {
  struct ThreadArgs* t_args = (struct ThreadArgs*)args;
  int dSC_sender = t_args->dSC_sender;
  int dSC_receiver = t_args->dSC_receiver;
  int isRunning = 1;
  size_t size = 0;

  while (isRunning) {
    printf("Start listening\n");
    if(0){
      printf("Wrong size\n");
      break;
    }
    printf("Sized received : %zu\n", size);
    size = 256;
    if (size >= 0 && size < 1024){
      printf("Allocating next msg\n");
    
    char msg[256];
    if(receive_message(dSC_sender, msg, size) <= 0) {
      isRunning = 0;
      break;
    }
    broadcast_message(dSC_sender, msg, size);
    printf("Message sent\n");
    }
  }
}

int main(int argc, char *argv[]) {

  // Global to local variable to enhance performance
  int msgLenght = globalMessageLenght; 

  printf("Starting program\n");

  // Server socket connection
  int dS = new_server_socket(atoi(argv[1]));
  printf("Listening mode\n");
  

  while (1){
    int dSC = new_client_connection(dS);
    pthread_t thread;

    // Add client to the clients array
    pthread_mutex_lock(&mutex);
    for (int i = 0; i < MAX_CLIENT; i++) {
        if (clients[i] == 0) {
            clients[i] = dSC;
            break;
        }
    }
    pthread_mutex_unlock(&mutex);
    
    struct ThreadArgs arg = {dSC, dSC};

    if (pthread_create(&thread, NULL, handle_client, (void*)&arg) != 0) {
      perror("pthread_create");
    return 1;
  }
    printf("New thread created\n");
  }
  
  
  printf("Shutting down programm\n");

  shutdown(dS, 2) ;
  printf("Bye bye\n");
}
