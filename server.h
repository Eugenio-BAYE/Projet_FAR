#ifndef SERVER_H
#define SERVER_H

#include <unistd.h>

/* shutdown_server : Shutdown the server
 * Parameters : None
 * Returns : Nothing
*/
void shutdown_server();

/* get_dS : Get the server socket
 * Parameters : None
 * Returns : int dS (file descriptor, non-negative)
*/
int get_dS();

/* get_port : Get the port
 * Parameters : None
 * Returns : int port (port number)
*/
int get_port();


#endif
