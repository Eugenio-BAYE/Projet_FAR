#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <semaphore.h>
#include <pthread.h>

#include "file_receiving.h"
#include "file_sending.h"
#include "server_handling.h"

void cmd_send_file() {
  printf("cmd_file_send\n");
  pthread_t file_thread;

  if (pthread_create(&file_thread, NULL, file_sending_thread, NULL) != 0) {

    perror("Failed to create file handling thread");
    return;
  }

  if (pthread_detach(file_thread) != 0) {
    perror("Failed to detach file handling thread");
    return;
  }
}

void cmd_receive_file(){
  printf("cmd_file_receive\n");
  pthread_t file_thread;

  if (pthread_create(&file_thread, NULL, file_receiving_thread, NULL) != 0) {
    perror("Failed to create file sending thread");
    return;
  }

  if (pthread_detach(file_thread) != 0) {
    perror("Failed to detach file sending thread");
    return;
  }

}

void execute_command(const char *command, int dSC) {
  if (strcmp(command, "@send_file\0")==0){
    //*********|WARNING|************/
    // Here it's the "@send_file" command handling so the
    // server is actually RECEIVING the file to the client
    cmd_send_file();
    return;
  }
  if (strcmp(command, "@receive_file\0")==0){
    //*********|WARNING|************/
    // Here it's the "@receive_file" command handling so the
    // server is actually SENDING the file to the client
    cmd_receive_file();
    return;
  }
}