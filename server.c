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

// ------------------------------------------------------
int clients[MAX_CLIENT]; // Array to store client socket descriptors
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER; // Mutex for thread-safe access to clients array
int nbr_of_clients = 0;

struct handle_client_args {
  int dSC_sender;
};
 /* broadcast_message : Send a message to every client except the one in parameter
  * Precondition : Sender must be in the global clients listening, mutex must be free in order to access the global client list 
  * Parameters : int sender(file descriptor of the sender), char* message(message to broadcast to the list), int message_size (size of the message)
  * Returns : Nothing
  */
void broadcast_message(int sender, char *message, int message_size) {
  printf("Message broadcasting : %s\n", message);
  pthread_mutex_lock(&mutex);
  for (int i = 0; i < MAX_CLIENT; i++) {
    if (clients[i] != 0 && clients[i] != sender) {
      send(clients[i], message, message_size, 0);
    }
  }
  pthread_mutex_unlock(&mutex);
}
/* handle_client : Thread-dedicated function to handle each client
 * Precondition : No more than <MAX_CLIENT> clients to handle
 * Parameters : struct handle_client_args -> int dSC_sender (file descriptor of the sender client)
 * Returns : Nothing
 */ 
void* handle_client(void* args) {
  struct handle_client_args* t_args = (struct handle_client_args*)args;
  int dSC_sender = t_args->dSC_sender;

  while (1) {
    char msg[globalMessageLenght];

    if(receive_message(dSC_sender, msg, globalMessageLenght) <= 0) {
      pthread_mutex_lock(&mutex);
      for (int i = 0; i < MAX_CLIENT; i++) {
        if (clients[i] == dSC_sender) {
          clients[i] = 0;
          break;
        }
      }
      nbr_of_clients--;
      pthread_mutex_unlock(&mutex);
      close(dSC_sender);
      printf("Client disconnected\n");
      pthread_exit(NULL);
      break;
    }
    broadcast_message(dSC_sender, msg, globalMessageLenght);
  }
}

int main(int argc, char *argv[]) {
  if(argc != 2){
    printf("Usage : %s <port>\n", argv[0]);
    perror("Can't run program");
    exit(EXIT_FAILURE);
  }

  printf("Starting program\n");

  for (int i = 0; i < MAX_CLIENT; i++) {
    clients[i] = 0;
  }

  // Server socket connection
  int dS = new_server_socket(atoi(argv[1]));
  printf("Waiting for clients\n");

  while (1){
    int dSC = new_client_connection(dS);
    while(nbr_of_clients>9){
      perror("Too much clients to handle");
      exit(EXIT_FAILURE);
    }
    
    pthread_mutex_lock(&mutex);
    nbr_of_clients++;
    pthread_mutex_unlock(&mutex);
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

    struct handle_client_args arg = {dSC};

    if (pthread_create(&thread, NULL, handle_client, (void*)&arg) != 0) {
      perror("pthread_create");
      return 1;
    }
  }

  printf("Shutting down programm\n");
  close(dS) ;
  printf("Bye bye\n");
}
