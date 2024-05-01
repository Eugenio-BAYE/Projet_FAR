#ifndef SERVER_H
#define SERVER_H

#include <unistd.h>

/* get_clients : Function for sharing the client table across mutliple files
 * Precondition : Use new_server_socket() before
 * Parameters : None
 * Returns : int* clients
 */
int* get_clients();

/* get_server_socket : Function for sharing the dS across mutliple files
 * Precondition : Use new_server_socket() before
 * Parameters : None
 * Returns : int dS (file descriptor, non-negative)
 */ // Pas sur de si c'est utile
int get_server_socket();

/* get_nbr_of_clients : Function for sharing the number of clients across mutliple files
 * Precondition : Use new_server_socket() before
 * Parameters : None
 * Returns : int (non-negative)
 */ 
int get_nbr_of_clients();

/* get_max_client : Function for sharing the number of clients across mutliple files
 * Precondition : Use new_server_socket() before
 * Parameters : None
 * Returns : int (non-negative)
 */ 
int get_max_client();

void broadcast_size(int sender, size_t inputLength);

void broadcast_message(int sender, char *message, int message_size);

#endif
