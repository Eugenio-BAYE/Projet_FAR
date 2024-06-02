#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>
#include <semaphore.h>
#include <sys/stat.h>
#include "server_handling.h"

int dS_receiver;

void create_directory_if_not_exists(const char *path) {
  struct stat st = {0};
  if (stat(path, &st) == -1) {
    if (mkdir(path, 0700) == -1) {
      perror("Failed to create directory"); // Indicate if directory creation fails
      pthread_exit(NULL); // Exit the thread in case of failure
    }
  }
}

char* receive_file_name(int socket) {
  size_t input_length;

  if (recv(socket, &input_length, sizeof(size_t), 0) <= 0) {
    perror("Failed to receive the size of the file name"); // Indicate if receiving size fails
    close(socket);
    pthread_exit(NULL);
  }

  char *file_name = malloc(input_length + 1);
  if (!file_name) {
    perror("Failed to allocate memory for the file name"); // Indicate if memory allocation fails
    close(socket);
    pthread_exit(NULL);
  }
  if (recv(socket, file_name, input_length, 0) <= 0) {
    perror("Failed to receive the file name"); // Indicate if receiving file name fails
    free(file_name);
    close(socket);
    pthread_exit(NULL);
  }
  file_name[input_length] = '\0';

  return file_name;
}

void generate_unique_file_path(char *file_path, const char *dir, const char *file_name) {
  snprintf(file_path, 512, "%s/%s", dir, file_name);

  int counter = 1;
  // Create a file in stocked_files named [file_name][number?] if the 
  // file name already exists
  while (access(file_path, F_OK) == 0) {
    snprintf(file_path, 512, "%s/%d_%s", dir, counter, file_name);
    counter++;
  }
}

void receive_and_write_file(int socket, FILE *file) {
  char buffer[1024];
  int bytes_read;

  while ((bytes_read = recv(socket, buffer, sizeof(buffer), 0)) > 0) {
    if (fwrite(buffer, 1, bytes_read, file) != bytes_read) {
      perror("Failed to write to file"); // Indicate if writing to file fails
      fclose(file);
      close(socket);
      pthread_exit(NULL);
    }
  }

  if (bytes_read < 0) {
    perror("Failed to receive file contents"); // Indicate if receiving file contents fails
  }

  fclose(file);
}

void* file_receiving_thread(void *arg) {
  dS_receiver = connect_socket(get_addr(), get_port()-1);

  char * input_buffer= malloc(64);
  while (receive_msg(dS_receiver)!=1){
  }
  if (fgets(input_buffer, 64, stdin) == NULL) {
    perror("Failed to read input"); // Indicate if reading input fails
    close(dS_receiver);
    pthread_exit(NULL);
  }
  puts(input_buffer);
  resume_input();

  // Send the integer as a string to the server
  if (send_msg(dS_receiver, input_buffer, strlen(input_buffer) + 3) == -1) { 
    //close(dS_receiver);
    pthread_exit(NULL);
  }

  // Following the original task
  char *file_name = receive_file_name(dS_receiver);
  create_directory_if_not_exists("./stocked_files");

  char file_path[512];
  generate_unique_file_path(file_path, "./stocked_files", file_name);
  free(file_name);

  FILE *file = fopen(file_path, "wb");
  if (!file) {
    perror("Failed to open file for writing"); // Indicate if opening file for writing fails
    close(dS_receiver);
    pthread_exit(NULL);
  }

  receive_and_write_file(dS_receiver, file);

  printf("Filed received, you can now chat on the server : \n"); // Indicate that the file has been received successfully
  close(dS_receiver);
  pthread_exit(NULL);
}