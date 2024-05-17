#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <signal.h>
#include "threads.h"


#define msgLength 256
int dS;
int is_running = 1;

int get_dS() {
    return dS;
}

void set_dS(int value) {
    dS = value;
}

int get_is_running() {
    return is_running;
}

void set_is_running(int value) {
    is_running = value;
}

void handle_sigint(int sig) {
  printf("\nCtrl+C pressed. Exiting...\n");
  shutdown(get_dS, 2);
  exit(1);
}

/* handle_local_sigint : Handles the SIGINT signal (Ctrl-C interruption) by shutting down the specified socket and exiting the program.
 * Parameters: - int sig: The signal number received by the handler.
 * Returns: None, as the function terminates the program by calling exit(1).
 */
void handle_local_sigint(int sig) {
    printf("\nCtrl+C pressed. Exiting...\n");
    exit(1);
}

int connect_socket(char * arg1, int arg2 ){

    signal(SIGINT, handle_local_sigint);
    // Create the socket
    int dS = socket(PF_INET, SOCK_STREAM, 0);
    printf("Attempting connection with the server...\n");
    if (dS == -1){
        perror("Error creating socket");
        exit(0);
    }
    // Create the adress
    struct sockaddr_in aS;
    aS.sin_family = AF_INET;
    inet_pton(AF_INET,arg1,&(aS.sin_addr)) ;
    aS.sin_port = htons(arg2) ;
    socklen_t lgA = sizeof(struct sockaddr_in) ;
    // Connect the socket
    if (connect(dS, (struct sockaddr *) &aS, lgA) == -1){
        perror("Error connecting socket");
        exit(0);
    }
    puts ("Socket connected");
    return dS;
}

void* loop_send_msg(void * args){
  struct thread_args * t_args = (struct thread_args *) args;
  int dS = t_args -> dS;

  while(is_running == 1){
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

    size_t input_length = strlen(buffer); // +1 to include the newline character ('\n')
    buffer[input_length-1]='\0';

    // Send the size of the message
    int send_size = send(dS, &input_length, sizeof(size_t), 0);
    if (send_size <= 0) {
      if (send_size == 0) {
        puts("Server disconnected when sending size");
      } else {
        perror("Error sending size");
      }
      free(buffer);
      pthread_exit(0);
    }

    // Send the message
    int send_message = send(dS, buffer, input_length, 0);
    if (send_message <= 0) {
      if(send_message == 0){
        puts("Server disconnected when sending message");
      } else {
        perror("Error sending message");
      }
      free(buffer);
      pthread_exit(0);
    }

    free(buffer);
  }
  return NULL;
}

/* receive_memset : Receives a message from a specified socket and ensures the message buffer is properly zero-initialized before receiving data.
 * Parameters: - int dSC: The descriptor of the socket from which the message is to be received.
 *             - char msg[]: The buffer where the received message will be stored.
 *             - int msgLenght: The length of the message buffer.
 * Returns: The size of the received message, or -1 if an error occurred.
 */
int receive_memset(int dSC, char msg[], int msgLenght){
  memset(msg, '\0', msgLenght);
  int received_size = recv(dSC, msg, msgLenght, 0);
  return received_size;
}

void* loop_receive_msg(void* args) {
  struct thread_args * t_args = (struct thread_args *) args;
  int dS = t_args -> dS;

  while(is_running == 1) {
    size_t input_length;

    // Receive the size of the message
    int receive_size = recv(dS, &input_length, sizeof(size_t), 0);
    if (receive_size <= 0) {
      if (receive_size == 0) {
        puts("Server disconnected");
      } else {
        perror("Error receiving size");
      }
      pthread_exit(0);
    }
    // Check that the message size is not too large
    if (input_length > 1024) {
      fprintf(stderr, "Error: Message size %zu is too large\n", input_length);
      pthread_exit(0);
    }

    char * msg = malloc(input_length);
    if (msg == NULL) {
      perror("Error allocating memory for msg");
      pthread_exit(0);
    }

    // Receive the message
    int receive_message = receive_memset(dS, msg, input_length);
    if (receive_message <= 0) {
      if(receive_message == 0){
        puts("Server disconnected");
      } else {
        perror("Error receiving message");
      }
      free(msg);
      pthread_exit(0);
    }
    
    puts(msg);      
    free(msg);
  }
  return NULL;
}