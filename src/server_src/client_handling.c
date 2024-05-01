#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>

#define MAX_CLIENT 10

static int clients[MAX_CLIENT];
static int nbr_of_clients = 0;
static pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

int get_nbr_of_clients(){
  return nbr_of_clients;
}

int get_max_client(){
  return MAX_CLIENT;
}

void free_client_list(){
  for (int i = 0; i < MAX_CLIENT; i++){
    clients[i]=0;
  }
}
/* can_accept_new_client : Checks if there is room for a new client
 * Parameters : int server_socket (file descriptor of the socket)
 * Returns : int(boolean) 1 if there is room, 0 else
 */
int can_accept_new_client(){
  if(nbr_of_clients < MAX_CLIENT){
    return 1;
  }
  else{
    return 0;
  }
}
/* add_new_client : Add a client to the list
 */
void add_new_client(int dSC){
  nbr_of_clients++;
  pthread_mutex_lock(&mutex);
  for (int i = 0; i < MAX_CLIENT; i++){
    if(clients[i]==0){
      clients[i]=dSC;
      break;
    }
  }
  pthread_mutex_unlock(&mutex);
}
/* broadcast_message : Send a message to every client except the one in parameter
  * Precondition : Sender must be in the global clients listening, mutex must be free in order to access the global client list 
  * Parameters : int sender(file descriptor of the sender), char* message(message to broadcast to the list), int message_size (size of the message)
  * Return : Nothing
  */
void broadcast_message(int sender, char *message, int message_size) {
  printf("Message broadcasting : %s\n", message);
  pthread_mutex_lock(&mutex); // Lock mutex for thread safety
  for (int i = 0; i < MAX_CLIENT; i++) {
    if (clients[i] != 0 && clients[i] != sender) {
      send(clients[i], message, message_size, 0);
    }
  }
  pthread_mutex_unlock(&mutex);
}

/* broadcast_size: Send the size to every client except the one in parameter
  * Precondition : Sender must be in the global clients listening, mutex must be free in order to access the global client list 
  * Parameters : int sender(file descriptor of the sender), size_t inputLength (size to broadcast to the list) 
  * Return : Nothing
  */
void broadcast_size(int sender, size_t inputLength) {
  pthread_mutex_lock(&mutex); // Lock mutex for thread safety
  for (int i = 0; i < MAX_CLIENT; i++) {
    if (clients[i] != 0 && clients[i] != sender) {
      if (send(clients[i], &inputLength, sizeof(size_t), 0) == -1) {
        perror("Error sending size");
        // Handle error if needed
      } else {
        puts("Input length sent");
      }
    }
  }
  pthread_mutex_unlock(&mutex); // Unlock mutex
}

void remove_client(int dSC){
  pthread_mutex_lock(&mutex);
  for (int i = 0; i < MAX_CLIENT; i++) {
    if (clients[i] == dSC) {
      clients[i] = 0;
      break;
    }
  }
  pthread_mutex_unlock(&mutex);
  nbr_of_clients--;
  close(dSC);
  printf("Client disconnected\n");
}

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

