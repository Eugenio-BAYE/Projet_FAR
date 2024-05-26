// src/server_src/file_receiving.c
#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/stat.h>

#include "server_handling.h"
#include "client_handling.h"
#include "server_utils.h"
#include "../../server.h"

int dS_receiver;

void create_file_receiving_socket(int port){
  int file_receiving_port = port + 1;
  int file_receiving_socket = new_server_socket(file_receiving_port);
  dS_receiver= file_receiving_socket;
  printf("%d\n", dS_receiver);
  printf("Created file receiving socket\n");
} 

// Crée un répertoire s'il n'existe pas
void create_directory_if_not_exists(const char *path) {
  struct stat st = {0};
  if (stat(path, &st) == -1) {
    if (mkdir(path, 0700) == -1) {
      perror("Failed to create directory");
      pthread_exit(NULL);
    }
  }
}

char* receive_file_name(int socket) {
  size_t input_length;

  if (recv(socket, &input_length, sizeof(size_t), 0) <= 0) {
    perror("Failed to receive the size of the file name");
    close(socket);
    pthread_exit(NULL);
  }

  char *file_name = malloc(input_length + 1);
  if (!file_name) {
    perror("Failed to allocate memory for the file name");
    close(socket);
    pthread_exit(NULL);
  }
  if (recv(socket, file_name, input_length, 0) <= 0) {
    perror("Failed to receive the file name");
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
  // file name aldready exists
  while (access(file_path, F_OK) == 0) {
    snprintf(file_path, 512, "%s/%s_%d", dir, file_name, counter);
    counter++;
  }
}

void receive_and_write_file(int socket, FILE *file) {
  char buffer[1024];
  int bytes_read;

  while ((bytes_read = recv(socket, buffer, sizeof(buffer), 0)) > 0) {
    if (fwrite(buffer, 1, bytes_read, file) != bytes_read) {
      perror("Failed to write to file");
      fclose(file);
      close(socket);
      pthread_exit(NULL);
    }
  }

  if (bytes_read < 0) {
    perror("Failed to receive file contents");
  }

  fclose(file);
}

void* file_receiving_thread() {
  int dSC = new_client_connection(dS_receiver);
  if (dSC < 0) {
    perror("Failed to accept new client connection.\n");
    pthread_exit(NULL);
  }

  send_msg(dSC, "Server ready to receive file");

  char *file_name = receive_file_name(dSC);
  create_directory_if_not_exists("./stocked_files");

  char file_path[512];
  generate_unique_file_path(file_path, "./stocked_files", file_name);
  free(file_name);

  FILE *file = fopen(file_path, "wb");
  if (!file) {
    perror("Failed to open file for writing");
    close(dSC);
    pthread_exit(NULL);
  }

  receive_and_write_file(dSC, file);

  close(dSC);
  pthread_exit(NULL);
}
