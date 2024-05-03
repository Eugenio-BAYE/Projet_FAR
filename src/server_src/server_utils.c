#include <stdio.h>
#include <sys/socket.h>
#include <string.h>
#include <stdlib.h>

#include "commands.h"

void send_msg(int dSC, char *msg) {
  // Calculate the size of the message
  size_t msg_size = strlen(msg) + 1;

  char *new_msg = malloc(msg_size); // Add 1 for null terminator
  memcpy(new_msg, msg, msg_size);
  new_msg[msg_size - 1] = '\0';
  // Send the size of te message to the client
  ssize_t bytes_sent = send(dSC, &msg_size, sizeof(size_t), 0);
  if (bytes_sent == -1) {
    perror("Error sending message size to client");
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
  if (command == NULL){
    perror("Can't execute_command() on empty command");
  }

  if (strcmp(command, "@size\n") == 0) {
    cmd_size(dSC);
    return;
  }
  if (strcmp(command, "@client\n") == 0) {
    cmd_client(dSC);
    return;
  }
  if (strcmp(command, "@random\n") == 0) {
    cmd_random(dSC);
    return;
  }
  if (strcmp(command, "@pileouface\n") == 0) {
    cmd_pile_ou_face(dSC);
    return;
  }
  if (strcmp(command, "@dascalu\0") == 0) {
    cmd_dascalu(dSC);
    return;
  }
  send_msg(dSC,"Unknown command \nCheck man for more info\0");
}

