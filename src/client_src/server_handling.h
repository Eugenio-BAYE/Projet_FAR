#ifndef SERVER_HANDLING_H
#define SERVER_HANDLING_H

#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <signal.h>

#define MSG_LENGTH 256

typedef struct {
    int dS;
} thread_args;

extern int dS;
extern int is_running;
extern int port;
extern char *addr;

/**
 * @brief Suspends user input.
 */
void break_input();

/**
 * @brief Resumes user input.
 */
void resume_input();

/**
 * @brief Gets the port number.
 * 
 * @return The port number.
 */
int get_port();

/**
 * @brief Sets the port number.
 * 
 * @param port The port number to set.
 */
void set_port(int port);

/**
 * @brief Gets the server address.
 * 
 * @return The server address.
 */
const char* get_addr();

/**
 * @brief Sets the server address.
 * 
 * @param addr The server address to set.
 */
void set_addr(const char *addr);

/**
 * @brief Gets the socket descriptor.
 * 
 * @return The socket descriptor.
 */
int get_dS();

/**
 * @brief Sets the socket descriptor.
 * 
 * @param value The socket descriptor to set.
 */
void set_dS(int value);

/**
 * @brief Gets the program status.
 * 
 * @return The program status.
 */
int get_is_running();

/**
 * @brief Sets the program status.
 * 
 * @param value The program status to set.
 */
void set_is_running(int value);

/**
 * @brief Handles the SIGINT signal.
 * 
 * @param sig The signal number.
 */
void handle_sigint(int sig);

/**
 * @brief Handles the SIGINT signal locally.
 * 
 * @param sig The signal number.
 */
void handle_local_sigint(int sig);

/**
 * @brief Connects to a server.
 * 
 * @param ip The IP address of the server.
 * @param port The port number.
 * 
 * @return The socket descriptor.
 */
int connect_socket(const char *ip, int port);

/**
 * @brief Receives data and initializes the buffer with null characters.
 * 
 * @param dSC The socket descriptor.
 * @param msg The buffer to store received data.
 * @param msgLength The length of the buffer.
 * 
 * @return The number of bytes received.
 */
int receive_memset(int dSC, char msg[], int msgLength);

/**
 * @brief Sends a message to the server.
 * 
 * @param dS The socket descriptor.
 * @param buffer The message to send.
 * @param input_length The length of the message.
 * 
 * @return 0 on success, -1 on failure.
 */
int send_msg(int dS, char* buffer, size_t input_length);

/**
 * @brief Receives a message from the server.
 * 
 * @param dS The socket descriptor.
 * 
 * @return 0 on success, -1 on failure.
 */
int receive_msg(int dS);

/**
 * @brief Sends messages to the server in a loop.
 * 
 * @param args The thread arguments.
 * 
 * @return NULL.
 */
void* loop_send_msg(void* args);

/**
 * @brief Receives messages from the server in a loop.
 * 
 * @param args The thread arguments.
 * 
 * @return NULL.
 */
void* loop_receive_msg(void* args);

#endif // SERVER_HANDLING_H
