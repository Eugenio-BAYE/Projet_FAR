#ifndef SERVER_HANDLING_H
#define SERVER_HANDLING_H

#endif // !SERVER_HANDLING_H
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <arpa/inet.h>

/* new_server_socket : Create a new TCP server socket
 * Parameters : int port (socket port)
 * Returns : int dS (file descriptor, non-negative)
 * Postconditions : Non-blocking, can throw errors
 */
int new_server_socket(int port);
