#include <stdio.h>
#include <sys/socket.h>
#include <string.h>
#include <stdlib.h>
#include <semaphore.h>

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

void execute_command(const char *command, int dSC, sem_t semaphore) {
  if (command == NULL){
    perror("Can't execute_command() on empty command");
  }

  if (strcmp(command, "@size\0") == 0) {
    cmd_size(dSC);
    return;
  }
  if (strcmp(command, "@client\0") == 0) {
    cmd_client(dSC);
    return;
  }
  if (strcmp(command, "@random\0") == 0) {
    cmd_random(dSC);
    return;
  }
  if (strcmp(command, "@pileouface\0") == 0) {
    cmd_pile_ou_face(dSC);
    return;
  }
  if (strcmp(command, "@dascalu\0") == 0) {
    cmd_dascalu(dSC);
    return;
  }
  if (strncmp(command, "@msg ", 5) == 0) {
    cmd_msg(dSC, command);
    return;
  }
  if (strcmp(command, "@quit\0")==0){
    cmd_quit(dSC);
    return;
  }
  if (strcmp(command, "@shutdown\0")==0){
    cmd_shutdown(dSC);
    return;
  }
  if (strcmp(command, "@man\0")==0){
    cmd_man(dSC);
    return;
  }
  if (strcmp(command, "@send_file\0")==0){
    //*********|WARNING|************/
    // Here it's the "@send_file" command handling so the
    // server is actually RECEIVING the file to the client
    cmd_send_file(dSC);
    return;
  }
  if (strcmp(command, "@receive_file\0")==0){
    //*********|WARNING|************/
    // Here it's the "@receive_file" command handling so the
    // server is actually SENDING the file to the client
    cmd_receive_file();
    return;
  }
  send_msg(dSC,"Unknown command \nCheck man for more info\0");
}

