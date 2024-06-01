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

typedef struct {
    int dS_sender;
    char *selected_file;
} FileSendArgs;

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
    char *newline = strchr(buffer, '\n');  // Trouve le premier '\n'
    if (newline) {
        *newline = '\0';  // Remplace par '\0' si '\n' est trouvé
    }

    printf("Input received: '%s'\n", buffer); // Affiche l'entrée après suppression de la nouvelle ligne

    // Check if the client canceled
    if (strncmp(buffer, "cancel", 6) == 0) {
        printf("Client canceled the operation.\n");
        return 0;
    }

    // Convert the selection to a number
    int file_index = atoi(buffer);
    printf("File index selected: %d\n", file_index); // Affiche l'indice du fichier sélectionné

    if (file_index <= 0) {
        printf("Invalid input or cancel was selected.\n");
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

    printf("Looking for the selected file...\n");

    while ((entry = readdir(dir)) != NULL) {
        if (entry->d_type == DT_REG) { // Only consider regular files
            printf("Current file: %s (index %d)\n", entry->d_name, current_index); // Affiche chaque fichier trouvé

            if (current_index == file_index) {
                strncpy(selected_file, entry->d_name, BUFFER_SIZE - 1);
                selected_file[BUFFER_SIZE - 1] = '\0'; // Ensure null termination
                closedir(dir);
                printf("File selected: %s\n", selected_file); // Affiche le fichier sélectionné
                return 1; // File found
            }
        }
        current_index++;
    }

    printf("File with index %d not found.\n", file_index); // Affiche si l'indice du fichier n'est pas trouvé
    closedir(dir);
    return -1; // File index not found
}

// Send the selected file to the client
void send_file_to_client(int dSC, const char *file_name) {
    char file_path[BUFFER_SIZE];
    snprintf(file_path, BUFFER_SIZE, "%s/%s", DIRECTORY_PATH, file_name);
    printf("Sending file: %s\n", file_path); // Debugging statement

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
    printf("File sent successfully or with errors.\n"); // Debugging statement
}

// Main thread function to handle file sending
void* file_sending_thread(void* arg) {
    FileSendArgs* args = (FileSendArgs*) arg;
    send_file_to_client(args->dS_sender, args->selected_file);  // Perform the file sending
    free(args);  // Clean up the allocated memory
    pthread_exit(NULL);
}