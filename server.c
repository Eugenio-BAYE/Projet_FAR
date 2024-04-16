// Include packages
#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>
// Include files
#include "src/server_src/client_handling.h" 
#include "src/server_src/server_handling.h"
// ------------------------------------------------------

#define globalMessageLenght 256

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

  char msg[msgLenght];

  while(1){
    // Receive from client 1
    receive_from_client(dSC1,msg,msgLenght);
    // Send to client 2
    send(dSC2, msg, sizeof(msg), 0);
    printf("Message sent to client 1\n");

    // Receive from client 2
    receive_from_client(dSC2,msg,msgLenght);
    // Send to client 1
    send(dSC1, msg, sizeof(msg), 0);
    printf("Message sent to client 2\n");
  }

  printf("Shutting down programm\n");
  shutdown(dSC1, 2) ; 
  shutdown(dSC2, 2);
  shutdown(dS, 2) ;
  printf("Bye bye\n");
}
