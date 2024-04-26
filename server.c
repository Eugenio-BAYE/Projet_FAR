// Include packages
#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <signal.h>
#include <time.h>
#include <string.h>

// Include files
#include "src/server_src/client_handling.h" 
#include "src/server_src/server_handling.h"
// ------------------------------------------------------

#define MAX_CLIENT 10
#define handle_error(msg) \
do { perror(msg); exit(EXIT_FAILURE); } while (0)
// ------------------------------------------------------

int clients[MAX_CLIENT]; // Array to store client socket descriptors
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER; // Mutex for thread-safe access to clients array
int nbr_of_clients = 0;
int dS ;





int startsWithAt(const char *msg) {
    if (msg != NULL && msg[0] == '@') {
        return 1; //Start with '@'
    }
    return 0;
}

int isExactlySize(const char *msg) {
    if (msg != NULL && strcmp(msg, "@size\n") == 0) {
        return 1;
    }
    return 0;
}

int isExactlyClient(const char *msg) {
    if (msg != NULL && strcmp(msg, "@client\n") == 0) {
        return 1;
    }
    return 0;
}

int isExactlyRandom(const char *msg) {
    if (msg != NULL && strcmp(msg, "@random\n") == 0) {
        return 1;
    }
    return 0;
}

int isExactlyPileOuFace(const char *msg) {
    if (msg != NULL && strcmp(msg, "@pileouface\n") == 0) {
        return 1;
    }
    return 0;
}

char *PileOuFace() {
    int var = rand() % 2;
    if(var == 1){
      return "pile";
    }
    else{
      return "face";
    }
}

int randomInt() {
    return rand() % 1000;
}

void sendMsg(int sD, char *buffer, size_t inputLength){
  int sendSize = send(dS, &inputLength, sizeof(size_t), 0);
  if (sendSize == -1) {
      perror("Error sending size");
      pthread_exit(0);
  }
  if (sendSize == 0) {
  puts("Error, disconnected when sending size");
      pthread_exit(0);
  }
  puts ("Input length sent");


  int sendMessage = send(dS, buffer, inputLength, 0);
  if (sendMessage == -1) {
      perror("Error sending message");
      pthread_exit(0);
  }
  if (sendMessage == 0) {
  puts("Error, disconnected when sending message");
      pthread_exit(0);
  }
  puts ("Message sent");
}






struct handle_client_args {
  int dSC_sender;
};
/* sigint_handler : handle ctrl + c of the program 
 * Parameters : int sig_num (signal number)
 * Return : Nothing
 */
void sigint_handler(int sig_num) {
  printf("\nCtrl+C pressed. Exiting...\n");
  shutdown(dS,2);
  exit(0);
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
/* handle_client : Thread-dedicated function to handle each client
 * Precondition : No more than <MAX_CLIENT> clients to handle
 * Parameters : struct handle_client_args -> int dSC_sender (file descriptor of the sender client)
 * Returns : Nothing
 */ 
void* handle_client(void* args) {
  struct handle_client_args* t_args = (struct handle_client_args*)args;
  int dSC_sender = t_args->dSC_sender;

  while (1) {
    puts ("Ready to receive");
    size_t inputLength;

    // Receive the size of the message
    if (recv(dSC_sender, &inputLength, sizeof(size_t), 0) <= 0) {
      // Erase client from list and stops the thread if it disconnects
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
      break;// Go out of the loop if the receive dont work
    }
    char lengthString[20]; // Create a char for create a String of the size
    snprintf(lengthString, 20, "%zu", inputLength); // Convert it
    puts ("Size received:");
    puts (lengthString); //Print it

    char * msg = malloc(inputLength);
    if(receive_message(dSC_sender, msg, inputLength) <= 0) {
      free(msg);
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
    //test msg to know if it's a message or a command
    if(startsWithAt(msg) == 1){
      if(isExactlyClient(msg) == 1){
        char * str;
        sprintf(str, "%d", nbr_of_clients);
        sendMsg(dSC_sender, str, sizeof(int));
      }
      if(isExactlySize(msg) == 1){
        char * str;
        sprintf(str, "%d", MAX_CLIENT);
        sendMsg(dSC_sender, str, sizeof(int));
      }
      if(isExactlyRandom(msg) == 1){
        char * str;
        int random = randomInt();
        sprintf(str, "%d", random);
        sendMsg(dSC_sender, str, sizeof(int));
      }
      if(isExactlyPileOuFace(msg) == 1){
        char *pileface = PileOuFace();
        sendMsg(dSC_sender, pileface, strlen(pileface));
      }
    }
    else{
      broadcast_size(dSC_sender, inputLength);
      broadcast_message(dSC_sender, msg, inputLength);
    }
    free(msg);
  }
}

int main(int argc, char *argv[]) {
  if(argc != 2){
    printf("Usage : %s <port>\n", argv[0]);
    printf("Can't run program\n");
    exit(EXIT_FAILURE);
  }
  signal(SIGINT, sigint_handler);
  printf("Starting program\n");

  for (int i = 0; i < MAX_CLIENT; i++) {
    clients[i] = 0;
  }

  // Server socket connection
  dS = new_server_socket(atoi(argv[1]));
  printf("Waiting for clients\n");
  while (1){
    int dSC = new_client_connection(dS);
    nbr_of_clients++;
    while(nbr_of_clients>9){
      sleep(5);
    }

    pthread_mutex_lock(&mutex);
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
}
