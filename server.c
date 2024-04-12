// Include packages
#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
// Include files
#include "src/client_handling.h" 
// ------------------------------------------------------

#define globalMessageLenght 256

int main(int argc, char *argv[]) {
  printf("Starting program\n");

  int msgLenght = globalMessageLenght;
  // Socket creating
  int dS = socket(PF_INET, SOCK_STREAM, 0);
  if (dS==-1){
    printf("ERROR : Failed to create socket\n");
  }
  printf("Socket created\n");

  // Structure 'ad' of type 'sockaddr_in' to store socket infos
  struct sockaddr_in ad;
  ad.sin_family = AF_INET; // Set adress to IPv4 
  ad.sin_addr.s_addr = INADDR_ANY ; // Allow connections from any IP
  // Set port number from command line argument (first argument)
  ad.sin_port = htons(atoi(argv[1])) ;
  // ---

  // Socket binding
  if (bind(dS, (struct sockaddr*)&ad, sizeof(ad))==-1){
    printf("Socket binding...\n");
  }
  printf("Socket binded\n");

  // Listening mode
  if (listen(dS, 2)==-1){
    printf("ERROR : Failed to enter listening mode");
  }
  printf("Listening mode\n");

  // Structure 'aC' of type 'sockaddr_in' to store client adresses
  struct sockaddr_in aC1, aC2;
  socklen_t lg1 = sizeof(struct sockaddr_in);
  socklen_t lg2 = sizeof(struct sockaddr_in);

  // Client 1 connections
  int dSC1 = new_client_connection(dS);
  // Client 2 connection 
  int dSC2 =  new_client_connection(dS);

  printf("Start chatting\n");

  while(1){
    char msg1[msgLenght];
    memset(msg1, '\0', sizeof(msg1)); // Initialisze msg1 to null
    // Receive from client 1
    if (recv(dSC1, msg1, sizeof(msg1), 0)>=0){
      printf("Message received from client 1\n");
      // Send to client 2
      send(dSC2, msg1, sizeof(msg1), 0);
      printf("Message sent to client 2\n");
    }

    char msg2[msgLenght];
    memset(msg2, '\0', sizeof(msg2)); // Initialize msg2 to null
    // Receive from client 2
    if (recv(dSC2, msg2, sizeof(msg2), 0)>=0){
      printf("Message received from client 1\n");
      // Send to client 1
      send(dSC1, msg2, sizeof(msg2), 0);
      printf("Message sent to client 2\n");
    }
  }

  printf("Shutting down programm\n");
  shutdown(dSC1, 2) ; 
  shutdown(dSC2, 2);
  shutdown(dS, 2) ;
  printf("Bye bye\n");
}
