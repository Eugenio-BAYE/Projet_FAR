#ifndef FILE_SENDING_H
#define FILE_SENDING_H

#include <stdio.h>
#include <sys/socket.h>
#include <dirent.h>      // For directory handling
#include <pthread.h>     // For threading

#define DIRECTORY_PATH "./stocked_files"
#define BUFFER_SIZE 1024

// External variable to store the socket descriptor for sending files
extern int dS_sender;

/**
 * Creates and initializes a socket for sending files to clients.
 * @param port The base port number to which the sending port will be offset.
 * @return The socket descriptor for the created socket.
 */
int create_file_sending_socket(int port);

/**
 * Sends a list of available files in the predefined directory to the client.
 * @param dSC The socket descriptor of the client connection.
 */
void send_file_list(int dSC);

/**
 * Receives the client's file selection directly from the terminal.
 * @param selected_file A buffer to store the name of the selected file if successful.
 * @return 1 if a valid file is selected, 0 if canceled, -1 if an invalid input or error occurs.
 */
int receive_client_selection(char *selected_file);

/**
 * Sends the selected file to the client.
 * @param dSC The socket descriptor of the client connection.
 * @param file_name The name of the file to be sent.
 */
void send_file_to_client(int dSC, const char *file_name);

/**
 * Main thread function to handle file sending operations.
 * @param args The arguments passed to the thread, typically including the client socket descriptor.
 * @return Always returns NULL, ending the thread on completion or error.
 */
void* file_sending_thread(void* args);

#endif // FILE_SENDING_H
