#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>

/* new_client_connection : Accepts a new client 
 * Parameters : int server_socket (file descriptor of the socket)
 * Returns : int client_socket (file descriptor for the client)
 * Postconditions : Blocking function, can't continue without a new user connecting
 */
int new_client_connection(int server_socket) {
  struct sockaddr_in client_addr;
  socklen_t client_addr_len = sizeof(client_addr);
  int client_socket = accept(server_socket, (struct sockaddr*) &client_addr, &client_addr_len);
  printf("New client connected\n");
  return client_socket;
}

/* receive_from_client : Get a message from a client in msg
 * Preconditions : Ensure messages will fit in var "msg"
 * Parameters : int dSC (descriptor of socket client), char *msg[] (char list in wich the message is returned)
 */
int receive_message(int dSC, char msg[], int msgLenght){
  // Empty msg of all content
  memset(msg, '\0', msgLenght);
  int received_size = recv(dSC, msg, msgLenght, 0);
  return received_size;
}

/* receive_from_client : Get a message from a client in msg
 * Preconditions : Ensure messages will fit in var "msg"
 * Parameters : int dSC (descriptor of socket client), char *msg[] (char list in wich the message is returned)
 */

int receive_msg_size(int dSC, size_t *size) {
  size_t message_size;
  printf("Create message size\n");
  int received_size = recv(dSC, &message_size, sizeof(message_size), 0);
  if (received_size > 0) {
    printf("Message size received: %d\n", (int)message_size);
    *size = message_size;
  } else if (received_size == 0) {
    printf("Client disconnected.\n");
  } else {
    perror("recv");
  }
  return received_size;
}

