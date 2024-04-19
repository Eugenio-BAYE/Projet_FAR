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

struct ThreadArgs {
    int dSC_sender;
    int dSC_receiver;
};

void* handle_client(void* args) {
  struct ThreadArgs* t_args = (struct ThreadArgs*)args;
  int dSC_sender = t_args->dSC_sender;
  int dSC_receiver = t_args->dSC_receiver;
  char msg[globalMessageLenght];
  int isRunning = 1;

  while (isRunning) {
    if(receive_from_client(dSC_sender, msg, globalMessageLenght) <= 0) {
      isRunning = 0;
    }
    send(dSC_receiver, msg, sizeof(msg), 0);
    printf("Message sent\n");
  }
}

int main(int argc, char *argv[]) {

  // Global to local variable to enhance performance
  int msgLenght = globalMessageLenght; 

  printf("Starting program\n");

  // Server socket connection
  int dS = new_server_socket(atoi(argv[1]));
  printf("Listening mode\n");
  // Client 1 connections
  int dSC1 = new_client_connection(dS);
  // Client 2 connection 
  int dSC2 =  new_client_connection(dS);

  printf("Start chatting\n");

  pthread_t thread1, thread2;
  struct ThreadArgs args1 = {dSC1, dSC2};
  struct ThreadArgs args2 = {dSC2, dSC1};
  char msg[msgLenght];

  if (pthread_create(&thread1, NULL, handle_client, (void*)&args1) != 0) {
    perror("pthread_create");
    return 1;
  }
  
  if (pthread_create(&thread2, NULL, handle_client, (void*)&args2) != 0){
    perror("pthread_create");
    return 1;
  }

  if(pthread_join(thread1, NULL) != 0 || pthread_join(thread2, NULL) != 0){
    perror("pthread_join");
    return 0;
  }
  
  printf("Shutting down programm\n");
  shutdown(dSC1, 2) ; 
  shutdown(dSC2, 2);
  shutdown(dS, 2) ;
  printf("Bye bye\n");
}
