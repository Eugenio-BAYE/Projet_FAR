#ifndef FILE_RECEIVING_H
#define FILE_RECEIVING_H

#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

/**
 * @brief Creates a file receiving socket and connects to it.
 * 
 * @param port The port number to use for the file receiving socket.
 */
void create_file_receiving_socket(int port);

/**
 * @brief Creates a directory if it does not exist.
 * 
 * @param path The path of the directory to create.
 */
void create_directory_if_not_exists(const char *path);

/**
 * @brief Receives the name of a file from a socket.
 * 
 * @param socket The socket from which to receive the file name.
 * @return The received file name.
 */
char* receive_file_name(int socket);

/**
 * @brief Generates a unique file path in a given directory.
 * 
 * @param file_path The buffer to store the generated file path.
 * @param dir The directory where the file will be stored.
 * @param file_name The name of the file.
 */
void generate_unique_file_path(char *file_path, const char *dir, const char *file_name);

/**
 * @brief Receives a file and writes it to the file system.
 * 
 * @param socket The socket from which to receive the file.
 * @param file The file pointer to write the received file.
 */
void receive_and_write_file(int socket, FILE *file);

/**
 * @brief Main thread function to handle file receiving.
 * 
 * @param arg The argument for the thread function (not used).
 */
void* file_receiving_thread(void *arg);

#endif