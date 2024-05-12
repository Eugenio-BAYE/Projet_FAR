#include "server_utils.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <semaphore.h>

#include "client_handling.h"
#include "../../server.h"

void cmd_random(int dSC) {
  int random = rand() %1000;
  int num_digits = snprintf(NULL, 0, "%d", random);
  
  // Convert the random integer to a string
  char random_str[num_digits + 1];
  sprintf(random_str, "%d", random);

  send_msg(dSC, random_str);
}

void cmd_pile_ou_face(int dSC){
  int random = rand() % 2;
  if(random == 1){
    char* pile = "pile\0";
    send_msg(dSC, pile);
  }
  if(random == 0){
    char* face = "face\0";
    send_msg(dSC, face);
  }
}

void cmd_client(int dSC){
  int nbr_of_clients = get_nbr_of_clients();
  int num_digits = snprintf(NULL, 0, "%d", nbr_of_clients);
  
  // Convert the nbr_of_clients integer to a string
  char nbr_of_clients_str[num_digits + 1];
  sprintf(nbr_of_clients_str, "%d", nbr_of_clients);

  send_msg(dSC, nbr_of_clients_str);
}

void cmd_size(int dSC){
  int max_int = get_max_client();
  int num_digits = snprintf(NULL, 0, "%d", max_int);

  char max_str[num_digits + 1];
  sprintf(max_str, "%d", max_int);
  send_msg(dSC, max_str);
}

void cmd_dascalu(int dSC){
  char* message = "DASCALUUU\0";
  int message_size = strlen(message) + 1; 
  broadcast_message(dSC, message, message_size);
}

void cmd_msg(int dSC, const char* command) {
  printf("cmd_msg\n");
  char *command_copy = strdup(command);
  printf("cmd_msg : strdup\n");
  if (command_copy == NULL) {
    fprintf(stderr, "Memory allocation failed\n");
    return;
  }

  char *cmd = strtok(command_copy, " ");
  char *username = strtok(NULL, " ");
  char *message = strtok(NULL, "");

  if (username == NULL || message == NULL) {
    send_msg(dSC, "Error: Invalid command syntax. Use @msg <username> <message>");
  } else {
    int recipient = find_client_by_username(username);
    if (recipient == -1) {
      send_msg(dSC, "Error: User not found");
    } else {
      int size = strlen(message)+1+5;
      char* msg = malloc(size);
      snprintf(msg, size, "->Me %s", message);
      printf("%d %s\n",size, msg);
      int formated_size = formated_msg_size(dSC, size);
      char* formated_msg = malloc(formated_size);
      format_msg(msg, dSC, formated_size, formated_msg);
      printf("%d %s\n",formated_size, formated_msg);
      send_msg(recipient, formated_msg);
      free(formated_msg);
    }
  }

  free(command_copy);
}

void cmd_shutdown(int dSC){
  char username[21];
  find_client_username(dSC, username);
  char msg[50];
  sprintf(msg, "%s closed the server\n", username);
  broadcast_message(dSC, msg, strlen(msg)+1);
  shutdown_server();
}

void cmd_quit(int dSC, sem_t semaphore){
  send_msg(dSC, "Disconnecting from server...\n");
  remove_client(dSC, semaphore);
}
