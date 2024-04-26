#ifndef SERVER_HANDLING_H
#define SERVER_HANDLING_H

/* connect_socket : connect the socket to the server put in parameter 
 * Parameters: - char *arg1: A pointer to a string representing the server's IP address in IPv4 format.
 *             - int arg2: An integer representing the server's port number.
 * Errors: If the socket cannot be created and if the connection fails, exits with 0.
 * Returns: The socket descriptor (int) on successful connection.
 */
int connect_socket(char * arg1, int arg2);


#endif
