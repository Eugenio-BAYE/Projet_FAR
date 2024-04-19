#ifndef CLIENT_HANDLING_H
#define CLIENT_HANDLING_H

#endif // !CLIENT_HANDLING_H

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
int receive_from_client(int dSC, char msg[], int msgLenght);
