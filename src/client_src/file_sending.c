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

#define DIRECTORY_PATH "./stocked_files"
#define BUFFER_SIZE 1024

int dS_sender;

// Send the list of available files to the client
void send_file_list() {
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
    puts(buffer);

    // List the files and send to the client
    while ((entry = readdir(dir)) != NULL) {
        if (entry->d_type == DT_REG) { // Regular file
            file_count++;
            snprintf(buffer, BUFFER_SIZE, "%d. %s\n", file_count, entry->d_name);
            puts(buffer);
        }
    }
    closedir(dir);

    // Send the command to cancel
    snprintf(buffer, BUFFER_SIZE, "Enter the number of the file to download or 'cancel' to abort:\n");
    puts(buffer);
}

// Receive client's selection directly from the terminal
int receive_client_selection(char *selected_file) {
    char buffer[1024]; // Reasonable size for user input

    printf("Enter the number of the file to download or 'cancel' to abort: ");

    if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
        perror("Error reading input");
        return -1;
    }

    // Remove any newline character included by fgets
    buffer[strcspn(buffer, "\n")] = '\0';

    // Check if the client canceled
    if (strncmp(buffer, "cancel", 6) == 0) {
        return 0;
    }

    // Convert the selection to a number
    int file_index = atoi(buffer);
    if (file_index <= 0) {
        return -1; // Invalid number or "cancel"
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
        if (entry->d_type == DT_REG) { // Only consider regular files
            current_index++;
            if (current_index == file_index) {
                strncpy(selected_file, entry->d_name, BUFFER_SIZE - 1);
                selected_file[BUFFER_SIZE - 1] = '\0'; // Ensure null termination
                closedir(dir);
                return 1; // File found
            }
        }
    }

    closedir(dir);
    return -1; // File index not found
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

// Main thread function to handle file sending
void* file_sending_thread(void* args) {
    dS_sender = connect_socket(get_addr(), get_port()+1);

    send_file_list();

    char selected_file[BUFFER_SIZE];
    int selection_status = receive_client_selection(selected_file);

    if (selection_status == 1) {
        send_file_to_client(dS_sender, selected_file);
    } else if (selection_status == 0) {
        printf("Client canceled the file transfer.\n");
    } else {
        printf("Invalid selection from client.\n");
    }

    close(dS_sender);
    pthread_exit(NULL);
}