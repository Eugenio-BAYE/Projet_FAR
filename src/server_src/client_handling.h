#ifndef CLIENT_HANDLING_H
#define CLIENT_HANDLING_H

#include <stdlib.h>
#include <semaphore.h>

#endif // !CLIENT_HANDLING_H


/* list_channels : List all the channels
 * Parameters : int dSC (file descriptor of the client)
 * Returns : Nothing
 */
void list_channels(int dSC); 

/* join_channel : Join a channel
 * Parameters : int dSC (file descriptor of the client), char *channel_name (name of the channel)
 * Returns : int (1 if the client joined the channel, 0 otherwise)
 */
int join_channel(int dSC,char *channel_name);

/* create_channel : Create a new channel
 * Parameters : const char *channel_name (name of the channel), const char *description (description of the channel)
 * Returns : int (1 if the channel was created, 0 otherwise)
 */
int create_channel(const char *channel_name, const char *description); 

/* find_client_username : Find the username of a client in the global list of clients
 * Parameters : int dSC (file descriptor of the client), char username[] (username of the client)
 * Postconditions : The username of the client is returned in username
 */
void find_client_username(int dSC, char username[]);

/* find_client_by_username : Find a client in the global list of clients by its username
 * Parameters : const char *username (username of the client)
 * Returns : int (file descriptor of the client)
 */
int find_client_by_username(const char *username);

/* get_nbr_of_clients : Get the number of clients connected to the server
 * Parameters : None
 * Returns : int (number of clients)
 */
int get_nbr_of_clients();

/* get_max_client : Get the maximum number of clients that can be connected to the server
 * Parameters : None
 * Returns : int (maximum number of clients)
 */
int get_max_client();

/* free_client_list : Free the global list of clients
 * Parameters : None
 * Postconditions : The global list of clients is freed
 */
void free_client_list();

/* add_new_client : Add a new client to the global list of clients
 * Parameters : int dSC (file descriptor of the client)
 * Postconditions : The client is added to the global list of clients
 */
void add_new_client(int dSC);

/* can_accept_new_client : Check if the server can accept a new client
 * Parameters : sem_t *semaphore (semaphore)
 * Returns : int (1 if the server can accept a new client, 0 otherwise)
 */
int can_accept_new_client(sem_t *semaphore);

/* new_semaphore : Create a new semaphore
 * Parameters : None
 * Returns : sem_t (the semaphore)
 */
sem_t new_semaphore();


/* create_client_handling_socket : Create a new TCP server socket for client handling
 * Parameters : int port (socket port)
 * Returns : int dS (file descriptor, non-negative)
 * Postconditions : Non-blocking, can throw errors
 */
void remove_client_from_current_channel(int dSC);

/* remove_client : Remove a client from the global list of clients
 * Parameters : int dSC (file descriptor of the client)
 * Postconditions : The client is removed from the global list of clients
 */
void remove_client(int dSC);

/* is_username_valid : Checks if username can be used
 * Parameters : char username[] 
 * Returns :  1 if it can be taken (SUCESS)
 *            2 if it is too long (FAILURE)
 *            3 if it is aldready taken (FAILURE)
 */
int is_username_valid(char username[]);

/* ask_username : Ask the client for a username
 * Parameters : int dSC (file descriptor of the client)
 * Returns : int (SUCESS)
 */
int ask_username(int dSC);

/* update_username : Update the username of a client
 * Parameters : int dSC (file descriptor of the client), char username[] (new username)
 * Postconditions : The username of the client is updated
 */
void update_username(int dSC, char username[]);

/* formated_msg_size : Format the size of a message to be sent to a client
 * Precondition : The message must be formated in the following way : "message"
 * Parameters : int dSC (file descriptor of the client), int msg_size (size of the message)
 * Returns : int (size of the formated message)
 */
int formated_msg_size(int dSC, int msg_size);

/* format_msg : Format a message to be sent to a client
 * Precondition : The message must be formated in the following way : "message"
 * Parameters : char msg[] (message to format), int dSC (file descriptor of the client), int size (size of the message), char formated_msg[] (formated message)
 * Postconditions : formated_msg is the message formated to be sent to the client, <username> : message
 */
void format_msg(char msg[],int dSC,int size, char formated_msg[]);

/* broadcast_message : Send a message to every client except the one in parameter
  * Precondition : Sender must be in the global clients listening, mutex must be free in order to access the global client list 
  * Parameters : int sender(file descriptor of the sender), char* message(message to broadcast to the list), int message_size (size of the message)
  */
void broadcast_message(int sender, char *message, size_t message_size);
/* broadcast_size: Send the size to every client except the one in parameter
  * Precondition : Sender must be in the global clients listening, mutex must be free in order to access the global client list 
  * Parameters : int sender(file descriptor of the sender), size_t inputLength (size to broadcast to the list) 
  */

int new_client_connection(int server_socket);

/* receive_from_client : Get a message from a client in msg
 * Preconditions : Ensure messages will fit in var "msg"
 * Parameters : int dSC (descriptor of socket client), char *msg[] (char list in wich the message is returned)
 */
int receive_message(int dSC, char msg[], int msgLenght);

/* receive_from_client : Get a message from a client in msg
 * Preconditions : Ensure messages will fit in var "msg"
 * Parameters : int dSC (descriptor of socket client), char *msg[] (char list in wich the message is returned)
 */
int receive_msg_size(int dSC, size_t *size);
