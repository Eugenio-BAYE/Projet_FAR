// Include packages
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <signal.h>
#include <time.h>
#include <string.h>
#include <semaphore.h>

// Include files
#include "src/server_src/client_handling.h" 
#include "src/server_src/server_handling.h"
#include "src/server_src/server_utils.h"

// ------------------------------------------------------
static int dS ;
struct handle_client_args{
  int dSC_sender;
  sem_t semaphore;
};
// ------------------------------------------------------

/* sigint_handler : handle ctrl + c of the program 
 * Parameters : int sig_num (signal number)
 */
void sigint_handler(int sig_num){
  printf("\nCtrl+C pressed. Exiting...\n");
  shutdown(dS,2);
  close(dS);
  exit(0);
}

void shutdown_server(){
  printf("Server shutting down\n");
  shutdown(dS,2);
  close(dS);
  exit(0);
}

/* handle_client : Thread-dedicated function to handle each client
 * Precondition : No more than <MAX_CLIENT> clients to handle
 * Parameters : struct handle_client_args -> int dSC_sender (file descriptor of the sender client)
 */ 
void* handle_client(void* args){
  struct handle_client_args* t_args=(struct handle_client_args*)args;
  int dSC_sender=t_args->dSC_sender;
  sem_t semaphore=t_args->semaphore;
  if(ask_username(dSC_sender, semaphore)==0){
    pthread_exit(NULL);
  }
  char username[21];
  find_client_username(dSC_sender, username);
  char msg[50];
  sprintf(msg, "Welcome %s to the server\n", username);
  broadcast_message(dSC_sender, msg, strlen(msg)+1);
  while(1){
    puts ("Ready to receive");
    size_t inputLength;

    // Receive the size of the message
    if (recv(dSC_sender, &inputLength, sizeof(size_t), 0) <= 0) {
      remove_client(dSC_sender, semaphore);
      printf("Client disconnect\n");
      pthread_exit(NULL);
      break;
    }
    // Receive message
    char * msg = malloc(inputLength);
    int size_of_received_message = receive_message(dSC_sender, msg, inputLength);
    if(size_of_received_message <= 0) {
      free(msg);
      remove_client(dSC_sender, semaphore);
      printf("Client disconnected\n");
      pthread_exit(NULL);
      break;
    }
    // Check message to know if it's a message or a command
    if(is_a_command(msg) == 1){
      execute_command(msg, dSC_sender, semaphore);
    }
    else{
      int size = formated_msg_size(dSC_sender, inputLength);
      char* formated_msg = malloc(size);
      format_msg(msg, dSC_sender, size, formated_msg);
      broadcast_message(dSC_sender, formated_msg, size);
      free(formated_msg);
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

  free_client_list();

  // Server socket connection
  dS = new_server_socket(atoi(argv[1]));
  sem_t semaphore = new_semaphore();
  while (1){
    
    if (can_accept_new_client(&semaphore) < 0) {
      perror("Failed to acquire semaphore. Cannot accept new client.\n");
      return 1;
    }
    int dSC = new_client_connection(dS);
    if (dSC < 0) {
      perror("Failed to accept new client connection.\n");
      return 1;
    }

    pthread_t thread;
    add_new_client(dSC);

    struct handle_client_args arg = {dSC, semaphore};
    if (pthread_create(&thread, NULL, handle_client, (void*)&arg) != 0) {
      perror("pthread_create");
      return 1;
    }
  }
}
