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

    printf("Enter the number of the file to download or 'cancel' to abort: \n"); // Newline char
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
      current_index++;
      printf("Current file: %s (index %d)\n", entry->d_name, current_index); // Affiche chaque fichier trouvé
      if (current_index == file_index) {
        printf("Index found\n");
        strncpy(selected_file, entry->d_name, BUFFER_SIZE);
        printf("File selected: %s\n", selected_file); // Affiche le fichier sélectionné
        closedir(dir);
        return 1; // File found
      }
    }
  }

    printf("File with index %d not found.\n", file_index); // Affiche si l'indice du fichier n'est pas trouvé
    closedir(dir);
    return -1; // File index not found
}

int send_filename(int dS, char* buffer, size_t input_length) {
    buffer[input_length - 1] = '\0';

    int send_size = send(dS, &input_length, sizeof(size_t), 0);
    if (send_size <= 0) {
        if (send_size == 0) {
            puts("Server disconnected when sending size");
        } else {
            perror("Error sending size");
        }
        free(buffer);
        return -1;
    }

    int send_message = send(dS, buffer, input_length, 0);
    if (send_message <= 0) {
        if (send_message == 0) {
            puts("Server disconnected when sending message");
        } else {
            perror("Error sending message");
        }
        free(buffer);
        return -1;
    }

    return 0;
}

// Send the selected file to the client
void send_file_to_client(int dSC, char *file_name) {
    char file_path[BUFFER_SIZE];
    snprintf(file_path, BUFFER_SIZE, "%s/%s", DIRECTORY_PATH, file_name);
    printf("Sending file: %s\n", file_path); // Debugging statement
    size_t size_of_filename = strlen(file_name)+1;
  printf("%ld\n", size_of_filename);

  send_filename(dSC, file_name, size_of_filename);

  printf("Filename sent successfully\n");



    FILE *file = fopen(file_path, "rb");
    if (file == NULL) {
        perror("Failed to open file");
        return;
    }

  char buffer[BUFFER_SIZE];
  size_t bytes_read = -1;
  // Get the size of the file
  fseek(file, 0, SEEK_END);
  size_t file_size = ftell(file);
  fseek(file, 0, SEEK_SET);

  // Send the size of the file
  if (send(dSC, &file_size, sizeof(file_size), 0) != sizeof(file_size)) {
    perror("Failed to send file size");
    fclose(file);
    return;
  }
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
    int dS_sender = args->dS_sender;
    char* selected_file = args->selected_file;
    send_file_to_client(dS_sender,selected_file);  // Perform the file sending
    free(args);  // Clean up the allocated memory
    pthread_exit(NULL);
}
