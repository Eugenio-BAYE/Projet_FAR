#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <semaphore.h>
#include <pthread.h>

#include "file_receiving.h"
#include "file_sending.h"
#include "server_handling.h"

#define BUFFER_SIZE 1024

void cmd_send_file() {
    send_file_list();
    FileSendArgs *args = malloc(sizeof(FileSendArgs));
    if (args == NULL) {
        perror("Failed to allocate memory for thread arguments");
        return;
    }
    char selected_file[BUFFER_SIZE];
    int selection_status = receive_client_selection(selected_file);
    args->selected_file=selected_file;
    if (selection_status == 1) {
        args->dS_sender = connect_socket(get_addr(), get_port() + 1);
        if (args->dS_sender < 0) {
            printf("Failed to connect socket.\n");
            free(args);
            return;
        }

        pthread_t send_thread;
        if (pthread_create(&send_thread, NULL, file_sending_thread, args) != 0) {
            perror("Failed to create file send thread");
            close(args->dS_sender);
            free(args);
            return;
        }
        pthread_detach(send_thread);
        return;
    } else if (selection_status == 0) {
        printf("Client canceled the file transfer.\n");
        free(args);
    } else {
        printf("Invalid selection from client.\n");
        free(args);
    }
}

void cmd_receive_file(){
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
  //check if the command @send_file is call
  if (strcmp(command, "@send_file\n")==0){
    cmd_send_file();
    return;
  }

  //check if the command @receive_file is call
  if (strcmp(command, "@receive_file\n")==0){
    break_input();
    cmd_receive_file();
    return;
  }
}
