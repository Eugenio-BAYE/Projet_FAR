
#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/stat.h>
#include <dirent.h>
#include <string.h>

#include "server_handling.h"
#include "client_handling.h"
#include "server_utils.h"

#define DIRECTORY_PATH "./stocked_files"
#define BUFFER_SIZE 1024

int dS_sender;

//*********|WARNING|************/
// Here it's the "@receive_file" command handling so the
// server is actually SENDING the file to the client


int create_file_sending_socket(int port) {
    int file_sending_port = port - 1;
    int file_sending_socket = new_server_socket(file_sending_port);
    dS_sender = file_sending_socket;
    printf("Created file sending socket\n");
    return file_sending_socket;
}

// Send the list of available files to the client
void send_file_list(int dSC) {

    send_msg(dSC, "Sending messages on normal port");

    DIR *dir;
    struct dirent *entry;
    char buffer[BUFFER_SIZE];
    int file_count = 0;

    dir = opendir(DIRECTORY_PATH);
    if (dir == NULL) {
        perror("Failed to open directory");
        return;
    }
    

    // Send the header of the file list
    snprintf(buffer, BUFFER_SIZE, "Available files:\n");
    send_msg(dSC, buffer);

    // List the files and send to the client
    while ((entry = readdir(dir)) != NULL) {
        if (entry->d_type == DT_REG) { // Regular file
            file_count++;
            snprintf(buffer, BUFFER_SIZE, "%d. %s\n", file_count, entry->d_name);
            send_msg(dSC, buffer);
        }
    }
    closedir(dir);

    // Send the command to cancel
    snprintf(buffer, BUFFER_SIZE, "Enter the number of the file to download or 'cancel' to abort:\n");
    send_msg(dSC, buffer);
}

// Receive the client's selection
int receive_client_selection(int dSC, char *selected_file) {
    size_t msg_size;
    if (recv(dSC, &msg_size, sizeof(size_t), 0) <= 0) {
        perror("Failed to receive message size");
        return -1;
    }

    char *buffer = malloc(msg_size);
    int bytes_received = recv(dSC, buffer, msg_size, 0);
    if (bytes_received <= 0) {
        perror("Failed to receive client selection");
        free(buffer);
        return -1;
    }
    buffer[bytes_received] = '\0';

    // Check if the client canceled
    if (strncmp(buffer, "cancel", 6) == 0) {
        free(buffer);
        return 0;
    }

    // Convert the selection to a number
    int file_index = atoi(buffer);
    free(buffer);
    if (file_index <= 0) {
        return -1;
    }

    // Find the corresponding file
    DIR *dir;
    struct dirent *entry;
    int current_index = 0;

    dir = opendir(DIRECTORY_PATH);
    if (dir == NULL) {
        perror("Failed to open directory");
        return -1;
    }

    while ((entry = readdir(dir)) != NULL) {
        if (entry->d_type == DT_REG) {
            current_index++;
            if (current_index == file_index) {
                strncpy(selected_file, entry->d_name, BUFFER_SIZE);
                closedir(dir);
                return 1;
            }
        }
    }

    closedir(dir);
    return -1;
}

// Send the selected file to the client
void send_file_to_client(int dSC, const char *file_name) {

    char file_path[BUFFER_SIZE];
    snprintf(file_path, BUFFER_SIZE, "%s/%s", DIRECTORY_PATH, file_name);

    FILE *file = fopen(file_path, "rb");
    if (file == NULL) {
        perror("Failed to open file");
        return;
    }

    char buffer[BUFFER_SIZE];
    size_t bytes_read;

    // Send the file in chunks
    while ((bytes_read = fread(buffer, 1, BUFFER_SIZE, file)) > 0) {
        if (send(dSC, buffer, bytes_read, 0) != bytes_read) {
            perror("Failed to send file");
            break;
        }
    }

    fclose(file);
}

struct handle_client_args{
  int normal_dSC;
};

// Main thread function to handle file sending
void* file_sending_thread(void* args) {
  struct handle_client_args* t_args=(struct handle_client_args*)args;
  int normal_dSC=t_args->normal_dSC;
    int dSC = new_client_connection(dS_sender);
    if (dSC < 0) {
        perror("Failed to accept new client connection.\n");
        pthread_exit(NULL);
    }

  printf("%d normal %d specific\n", normal_dSC, dSC);

    send_file_list(normal_dSC);

    char selected_file[BUFFER_SIZE];
    int selection_status = receive_client_selection(normal_dSC, selected_file);

    if (selection_status == 1) {
        send_file_to_client(dSC, selected_file);
    } else if (selection_status == 0) {
        printf("Client canceled the file transfer.\n");
    } else {
        printf("Invalid selection from client.\n");
    }

    close(dSC);
    pthread_exit(NULL);
}

