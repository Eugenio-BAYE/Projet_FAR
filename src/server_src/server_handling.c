#ifndef SERVER_HANDLING_H
#define SERVER_HANDLING_H

#endif // !SERVER_HANDLING_H
#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdlib.h>

/* new_server_socket : Create a new TCP server socket
 * Parameters : int port (socket port)
 * Returns : int dS (file descriptor, non-negative)
 * Postconditions : Non-blocking, can throw errors
 */
int new_server_socket(int port){
  // Socket creating
  // "socket()" : is not blocking, catchng errors:
  int dS = socket(PF_INET, SOCK_STREAM, 0);
  if (dS==-1){
    perror("Error creating socket\n");
    exit(EXIT_FAILURE);
  }
  printf("Socket created\n");

  // Set SO_REUSEADDR socket option to reuse port
  int opt = 1;
  if (setsockopt(dS, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
    perror("setsockopt");
    exit(EXIT_FAILURE);
  }

  // Structure 'ad' of type 'sockaddr_in' to store socket infos
  struct sockaddr_in ad;
  ad.sin_family = AF_INET; // Set adress to IPv4 
  ad.sin_addr.s_addr = INADDR_ANY ; // Allow connections from any IP
  ad.sin_port = htons(port) ; // Set port number from arg 1

  // Socket binding
  // "bind()" is not blocking, catching errors :
  if (bind(dS, (struct sockaddr*)&ad, sizeof(ad))==-1){
    perror("Error binding socket\n");
    exit(EXIT_FAILURE);
  }
  printf("Socket binded\n");

  // Listening mode
  // "listen()" is not blocking, catching errors :
  if (listen(dS, 2)==-1){
    perror("Error binding socket\n");
    exit(EXIT_FAILURE);
  }

  return dS;
}
