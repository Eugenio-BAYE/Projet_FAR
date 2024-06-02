#ifndef FILE_SENDING_H
#define FILE_SENDING_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>
#include "server_handling.h"

#define DIRECTORY_PATH "./stocked_files"
#define BUFFER_SIZE 1024

typedef struct {
    int dS_sender;
    char *selected_file;
} FileSendArgs;

/**
 * @brief Sends the list of available files to the client.
 */
void send_file_list(void);

/**
 * @brief Receives the client's file selection.
 * 
 * @param selected_file Pointer to store the selected file name.
 * @return Returns 0 if the client cancels, 1 if the selection is successful, -1 on failure.
 */
int receive_client_selection(char *selected_file);

/**
 * @brief Generates a unique file path.
 * 
 * @param file_path Pointer to store the generated file path.
 * @param dir Directory where the file is stored.
 * @param file_name Name of the file.
 */
void generate_unique_file_path(char *file_path, const char *dir, const char *file_name);

/**
 * @brief Sends the selected file to the client.
 * 
 * @param dSC Socket descriptor of the client connection.
 * @param file_name Name of the file to send.
 */
void send_file_to_client(int dSC, const char *file_name);

/**
 * @brief Creates a directory if it does not exist.
 * 
 * @param path Path of the directory to create.
 */
void create_directory_if_not_exists(const char *path);

/**
 * @brief Receives a file name from a socket.
 * 
 * @param socket Socket descriptor from which to receive the file name.
 * @return Returns the received file name.
 */
char* receive_file_name(int socket);

/**
 * @brief Receives and writes the file contents.
 * 
 * @param socket Socket descriptor from which to receive the file.
 * @param file Pointer to the file to write the contents.
 */
void receive_and_write_file(int socket, FILE *file);

/**
 * @brief Main thread function to handle file sending.
 * 
 * @param args Arguments for the file sending thread.
 */
void* file_sending_thread(void* args);

#endif