#ifndef CLIENT_HANDLING_H
#define CLIENT_HANDLING_H

#include <stdlib.h>

#endif // !CLIENT_HANDLING_H

int get_nbr_of_clients();
int get_max_client();
void free_client_list();
void add_new_client(int dSC);
int can_accept_new_client();
void broadcast_message(int sender, char *message, int message_size);
void broadcast_size(int sender, size_t inputLength);
void remove_client(int dSC);
/* new_client_connection : Accepts a new client 
 * Parameters : int server_socket (file descriptor of the socket)
 * Returns : int client_socket (file descriptor for the client)
 * Postconditions : Blocking function, can't continue without a new user connecting
 */
int new_client_connection(int server_socket);

/* receive_from_client : Get a message from a client in msg
 * Preconditions : Ensure messages will fit in var "msg"
 * Parameters : int dSC (descriptor of socket client), char *msg[] (char list in wich the message is returned)
 */
int receive_message(int dSC, char msg[], int msgLenght);


int receive_msg_size(int dSC, size_t *size);
