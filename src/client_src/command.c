#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <semaphore.h>
#include <pthread.h>

#include "file_receiving.h"
#include "file_sending.h"
#include "server_handling.h"

void cmd_send_file() {
    send_file_list();
    FileSendArgs *args = malloc(sizeof(FileSendArgs));
    if (args == NULL) {
        perror("Failed to allocate memory for thread arguments");
        return;
    }
    int selection_status = receive_client_selection(args->selected_file);
    printf("temp1\n");
    if (selection_status == 1) {
        args->dS_sender = connect_socket(get_addr(), get_port() + 1);
        if (args->dS_sender < 0) {
            printf("Failed to connect socket.\n");
            free(args);
            return;
        }

        printf("File sending thread being created...\n");
        pthread_t send_thread;
        if (pthread_create(&send_thread, NULL, file_sending_thread, args) != 0) {
            perror("Failed to create file send thread");
            close(args->dS_sender);
            free(args);
            return;
        }
        pthread_detach(send_thread);
        printf("File send thread started and detached successfully.\n");
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
  if (strcmp(command, "@send_file\n")==0){
    cmd_send_file();
    return;
  }
  if (strcmp(command, "@receive_file")==0){
    cmd_receive_file();
    return;
  }

}