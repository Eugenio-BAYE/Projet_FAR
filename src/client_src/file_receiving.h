#ifndef FILE_RECEIVING_H
#define FILE_RECEIVING_H

#include <sys/types.h>  // for size_t
#include <sys/stat.h>   // for mkdir, stat
#include <unistd.h>     // for close, access
#include <stdio.h>

// External variable to hold the descriptor of the file receiving socket
extern int dS_receiver;

/**
 * Initializes a socket dedicated to receiving files and stores its descriptor in `dS_receiver`.
 * @param port The port number on which the server should listen for incoming file transfer requests.
 */
void create_file_receiving_socket(int port);

/**
 * Ensures that a directory exists at the specified path. If the directory does not exist, it attempts to create it.
 * @param path The file system path to the directory.
 */
void create_directory_if_not_exists(const char *path);

/**
 * Receives a file name from a connected socket. This function assumes that the file name's length is sent before the actual name.
 * @param socket The socket descriptor from which the file name is received.
 * @return A pointer to a dynamically allocated string containing the file name. The caller is responsible for freeing this memory.
 */
char* receive_file_name(int socket);

/**
 * Generates a unique file path for saving a file, avoiding overwriting existing files by appending a counter to the file name.
 * @param file_path Buffer where the unique file path is stored.
 * @param dir Directory in which the file is to be saved.
 * @param file_name The base name of the file.
 */
void generate_unique_file_path(char *file_path, const char *dir, const char *file_name);

/**
 * Receives file data from the socket and writes it to the provided file stream.
 * @param socket The socket descriptor from which the file data is received.
 * @param file The file pointer where data should be written.
 */
void receive_and_write_file(int socket, FILE *file);

/**
 * Thread function that handles the reception of a file from a client. It sets up the file path, receives the file name,
 * creates necessary directories, and manages the file writing process.
 * @return Always returns NULL, ending the thread on completion or error.
 */
void* file_receiving_thread(void *arg);

#endif // FILE_RECEIVING_H
