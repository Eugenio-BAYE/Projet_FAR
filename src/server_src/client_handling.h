#ifndef CLIENT_HANDLING_H
#define CLIENT_HANDLING_H

#include <stdlib.h>

#endif // !CLIENT_HANDLING_H

int get_nbr_of_clients();
int get_max_client();
void free_client_list();
void add_new_client(int dSC);
int can_accept_new_client();
void remove_client(int dSC);
/* is_username_valid : Checks if username can be used
 * Parameters : char username[] 
 * Returns :  1 if it can be taken (SUCESS)
 *            2 if it is too long (FAILURE)
 *            3 if it is aldready taken (FAILURE)
 */
int is_username_valid(char username[]);
void ask_username(int dSC);
void update_username(int dSC, char username[]);

int formated_msg_size(int dSC, int msg_size);
void format_msg(char msg[],int dSC,int size, char formated_msg[]);

/* broadcast_message : Send a message to every client except the one in parameter
  * Precondition : Sender must be in the global clients listening, mutex must be free in order to access the global client list 
  * Parameters : int sender(file descriptor of the sender), char* message(message to broadcast to the list), int message_size (size of the message)
  */
void broadcast_message(int sender, char *message, int message_size);
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
