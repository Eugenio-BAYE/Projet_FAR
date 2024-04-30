#include <stdio.h>
#include <sys/socket.h>
#include <string.h>
#include <stdlib.h>

#include "commands.h"

void send_msg(int dSC, char *msg) {
  // Calculate the size of the message
  size_t msg_size = strlen(msg) + 1;
  printf("server_utils:send_msg Message lenght %ld\n", msg_size);

  char *new_msg = malloc(msg_size + 1); // Add 1 for null terminator
  memcpy(new_msg, msg, msg_size);
  new_msg[msg_size - 1] = '\0';
  // Send the size of te message to the client
  ssize_t bytes_sent = send(dSC, &msg_size, sizeof(size_t), 0);
  if (bytes_sent == -1) {
    perror("Error sending message size to client");
    return; // Return early if an error occurs
  }

  // Send the message to the client
  bytes_sent = send(dSC, new_msg, msg_size, 0);
  if (bytes_sent == -1) {
    perror("Error sending message to client");
  }
  free(new_msg);
}

int is_a_command(const char *msg) {
  if (msg != NULL){
    if (msg[0] == '@') {
      return 1;
    }
  }
  return 0;
}

void execute_command(const char *command, int dSC) {
  printf("server_utils : execute_command\n");
  if (command == NULL){
    perror("Can't execute_command() on empty command");
  }

  if (strcmp(command, "@size\n") == 0) {
    cmd_size(dSC);
  }
  if (strcmp(command, "@client\n") == 0) {
    cmd_client(dSC);
  }
  if (strcmp(command, "@random\n") == 0) {
    printf("server_utils : is random\n");
    cmd_random(dSC);
  }
  if (strcmp(command, "@pileouface\n") == 0) {
    cmd_pile_ou_face(dSC);
  }
  if (strcmp(command, "@dascalu\n") == 0) {
    cmd_dascalu(dSC);
  } else {
    printf("Unknown command: %s\nCheck man for more info\n", command);
  }
}

