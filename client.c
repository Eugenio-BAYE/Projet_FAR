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

  while(isRunning == 1){
    // Allocate memory for the buffer
    char * buffer = malloc(msgLength);
    if (buffer == NULL) {
      perror("Error allocating memory for buffer");
      pthread_exit(0);
    }

    // Read the keyboard input
    if (fgets(buffer, msgLength, stdin) == NULL) {
      puts ("Error reading or end of file detected");
      free(buffer);
      pthread_exit(0);
    }

    size_t inputLength = strlen(buffer); // +1 to include the newline character ('\n')
    buffer[inputLength-1]='\0';

    // Send the size of the message
    int sendSize = send(dS, &inputLength, sizeof(size_t), 0);
    if (sendSize <= 0) {
      if (sendSize == 0) {
        puts("Server disconnected when sending size");
      } else {
        perror("Error sending size");
      }
      free(buffer);
      pthread_exit(0);
    }

    // Send the message
    int sendMessage = send(dS, buffer, inputLength, 0);
    if (sendMessage <= 0) {
      if(sendMessage == 0){
        puts("Server disconnected when sending message");
      } else {
        perror("Error sending message");
      }
      free(buffer);
      pthread_exit(0);
    }

    // Check if the loop is finished with the word "fin"
    if (compareFin(buffer) == 1){
      isRunning = 0;
      puts ("End of program");
    }

    free(buffer);
  }
  return NULL;
}

int receive_message(int dSC, char msg[], int msgLenght){
  memset(msg, '\0', msgLenght);
  int received_size = recv(dSC, msg, msgLenght, 0);
  return received_size;
}

/** receiveMsg : Receive messages from the user to a server over a socket.
 * Parameters: void *args: Pointer to a 'thread_args' structure which the socket descriptor 'dS'.
 * Errors: If receiving the size or message fails or the socket disconnect, it will print an error and exit the thread.
 * Returns: NULL on exit, either after an error or after a termination command ("fin").
 */
void* receiveMsg(void* args) {
  struct thread_args * t_args = (struct thread_args *) args;
  int dS = t_args -> dS;

  while(isRunning == 1) {

    size_t inputLength;

    // Receive the size of the message
    int receiveSize = recv(dS, &inputLength, sizeof(size_t), 0);
    if (receiveSize <= 0) {
      if (receiveSize == 0) {
        puts("Server disconnected");
      } else {
        perror("Error receiving size");
      }
      pthread_exit(0);
    }
    // Check that the message size is not too large
    if (inputLength > 1024) {
      fprintf(stderr, "Error: Message size %zu is too large\n", inputLength);
      pthread_exit(0);
    }

    char * msg = malloc(inputLength);
    if (msg == NULL) {
      perror("Error allocating memory for msg");
      pthread_exit(0);
    }

    // Receive the message
    int receiveMessage = receive_message(dS, msg, inputLength);
    if (receiveMessage <= 0) {
      if(receiveMessage == 0){
        puts("Server disconnected");
      } else {
        perror("Error receiving message");
      }
      free(msg);
      pthread_exit(0);
    }
    
    puts(msg);      

        // Check if the loop is finished with the word "fin"
    if (compareFin(msg) == 1){
      isRunning = 0;
      puts ("End of program");
      free(msg);
      pthread_exit(0);
    }
    free(msg);
  }
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
