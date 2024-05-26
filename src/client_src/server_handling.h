#ifndef SERVER_HANDLING_H
#define SERVER_HANDLING_H

/* get_dS : Retrieves the current value of the global variable 'dS'.
 * Parameters: None.
 * Returns: The current value of 'dS'.
 * Description: This function provides access to the value of the global variable 'dS', allowing other parts of the program to read its value without directly accessing the variable.
 */
int get_dS();

/* set_dS : Sets a new value for the global variable 'dS'.
 * Parameters: - int value: The new value to be assigned to 'dS'.
 * Returns: None.
 * Description: This function assigns a new value to the global variable 'dS', allowing other parts of the program to update its value in a controlled manner.
 */
void set_dS(int value);

/* get_isRunning : Retrieves the current value of the global variable 'isRunning'.
 * Parameters: None.
 * Returns: The current value of 'isRunning'.
 * Description: This function provides access to the value of the global variable 'isRunning', allowing other parts of the program to read its value without directly accessing the variable.
 */
int get_is_running();

/* set_isRunning : Sets a new value for the global variable 'isRunning'.
 * Parameters: - int value: The new value to be assigned to 'isRunning'.
 * Returns: None.
 * Description: This function assigns a new value to the global variable 'isRunning', allowing other parts of the program to update its value in a controlled manner.
 */
void set_is_running(int value);

/* handle_sigint : Handles the SIGINT signal (Ctrl-C interruption) by shutting down the specified socket and exiting the program.
 * Parameters: - int sig: The signal number received by the handler.
 * Returns: None, as the function terminates the program by calling exit(1).
 */
void handle_sigint(int sig);

/* connect_socket : connect the socket to the server put in parameter 
 * Parameters: - char *arg1: A pointer to a string representing the server's IP address in IPv4 format.
 *             - int arg2: An integer representing the server's port number.
 * Errors: If the socket cannot be created and if the connection fails, exits with 0.
 * Returns: The socket descriptor (int) on successful connection.
 */
int connect_socket(char * arg1, int arg2);

/* send_msg : Sends a message through the specified socket.
 * Parameters: - int dS: The socket descriptor.
 *             - char* buffer: The message buffer.
 *             - size_t input_length: The length of the message.
 * Returns: 0 on success, -1 on error.
 */
int send_msg(int dS, char* buffer, size_t input_length);

/** send_msg : Send messages from the user to a server over a socket.
 * Parameters: void *args: Pointer to a 'thread_args' structure which the socket descriptor 'dS'.
 * Errors: If sending the size or message fails or the socket disconnect, it will print an error and exit the thread.
 * Returns: NULL on exit, either after an error or after a termination command ("fin").
 */
void* loop_send_msg(void * args);

/* receive_message : Receives a message through the specified socket.
 * Parameters: - int dS: The socket descriptor.
 * Returns: 0 on success, -1 on error.
 */
int receive_msg(int dS);

/** receive_msg : Receive messages from the user to a server over a socket.
 * Parameters: void *args: Pointer to a 'thread_args' structure which the socket descriptor 'dS'.
 * Errors: If receiving the size or message fails or the socket disconnect, it will print an error and exit the thread.
 * Returns: NULL on exit, either after an error or after a termination command ("fin").
 */
void* loop_receive_msg(void* args);


/**
 * Create a new TCP server socket and configure it for use.
 * @param port The port number on which the server socket will listen.
 * @return The file descriptor for the newly created socket. Exits on failure.
 */
int new_server_socket(int port);

#endif