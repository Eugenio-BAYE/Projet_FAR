#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <signal.h>

#include "src/client_src/server_handling.h"
#include "src/client_src/client_utils.h"

#define msgLength 256

int dS;
int isRunning = 1;

struct thread_args {
  int dS;
};

/* handle_sigint : Handles the SIGINT signal (Ctrl-C interruption) by shutting down the specified socket and exiting the program.
 * Parameters: - int sig: The signal number received by the handler.
 * Returns: None, as the function terminates the program by calling exit(1).
 */
void handle_sigint(int sig) {
  printf("\nCtrl+C pressed. Exiting...\n");
  shutdown(dS, 2);
  exit(1);
}


/** sendMsg : Send messages from the user to a server over a socket.
 * Parameters: void *args: Pointer to a 'thread_args' structure which the socket descriptor 'dS'.
 * Errors: If sending the size or message fails or the socket disconnect, it will print an error and exit the thread.
 * Returns: NULL on exit, either after an error or after a termination command ("fin").
 */
void* sendMsg(void * args){
  struct thread_args * t_args = (struct thread_args *) args;
  int dS = t_args -> dS;
  char * buffer = malloc(msgLength);

  while(isRunning == 1){
    // Read the keyboard enter
    puts("Please enter a string of characters :");
    if (fgets(buffer, msgLength, stdin) != NULL) {
      puts ("You have entered:") ;
      puts (buffer);
      size_t inputLength = strlen(buffer)+1; // +1 to include the newline character ('\n')
      int sendSize = send(dS, &inputLength, sizeof(size_t), 0);
      if (sendSize == -1) {
        perror("Error sending size");
        pthread_exit(0);
      }
      if (sendSize == 0) {
        puts("Error, disconnected when sending size");
        pthread_exit(0);
      }

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

      // Check if the loop is finished with the word "fin"
      if (compareFin(buffer) == 1){
        isRunning = 0;
        puts ("End of program");
        free(buffer);
        pthread_exit(0);
      }
    } 
    else {
      puts ("Error reading or end of file detected");
    }
  }
  free(buffer);
  return NULL;
}

/** receiveMsg : Receive messages from the user to a server over a socket.
 * Parameters: void *args: Pointer to a 'thread_args' structure which the socket descriptor 'dS'.
 * Errors: If receiving the size or message fails or the socket disconnect, it will print an error and exit the thread.
 * Returns: NULL on exit, either after an error or after a termination command ("fin").
 */
void* receiveMsg(void* args) {
  struct thread_args * t_args = (struct thread_args *) args;
  int dS = t_args -> dS;
  char * buffer = malloc(msgLength);

  while(isRunning == 1) {
    puts ("Ready to receive");
    size_t inputLength;

    // Receive the size of the message
    int receiveSize = recv(dS, &inputLength, sizeof(size_t), 0);
    if (receiveSize == -1) {
      perror("Error receiving size");
      pthread_exit(0);
    }
    if (receiveSize == 0) {
      puts("Server disconnected");
      pthread_exit(0);
    }

    // Receive the message
    int receiveMessage = recv(dS, buffer, inputLength, 0);
    if (receiveMessage == -1) {
      perror("Error receiving message");
      pthread_exit(0);
    }
    if(receiveMessage == 0){
      puts("Server disconnected");
      pthread_exit(0);
    }

    puts ("Message received:");
    puts (buffer);

    // Check if the loop is finished with the word "fin"
    if (compareFin(buffer) == 1){
      isRunning = 0;
      puts ("End of program");
      pthread_exit(0);
    }
  }
  free(buffer);
  return NULL;
}



int main(int argc, char *argv[]) {
  // Check the number of arguments
  if (argc != 3) {
    puts ("Invalid number of arguments, usage:");
    printf("%s IpServer Port\n", argv[0]);
    exit(0);
  }

  // Connect the socket
  signal(SIGINT, handle_sigint);
  dS = connect_socket(argv[1], atoi(argv[2]) );

  pthread_t thread1, thread2;
  struct thread_args args1 = {dS};

  // Create first thread
  if (pthread_create(&thread1, NULL, sendMsg, (void*)&args1) != 0) {
    perror("pthreadSend_create");
    return 1;
  }

  // Create second thread
  if (pthread_create(&thread2, NULL, receiveMsg, (void*)&args1) != 0) {
    perror("pthreadReceive_create");
    return 1;
  }

  // Wait the end of threads
  while (isRunning){
    sleep(1);
  }
  shutdown(dS, 2); // Close the connection
  return 1;
}
