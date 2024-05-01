#ifndef SERVER_UTILS_H
#define SERVER_UTILS_H


/* is_a_command : Checks if the message is a command
 * Parameters : const char *msg - A pointer to a string which will be checked.
 * Return : Returns 1 if the string starts with '@', otherwise returns 0.
 */
int is_a_command(char* msg);

/* execute_command : Handle the execution of the command in parameter
 * Parameters : const char *command (command to execute), int dSC (file descriptor of the client)
 * Returns : Nothing
 */
void execute_command(const char *command, int dSC);

/* send_msg : Send a message to a client
 * Parameters : int dSC (file descriptor of the client), const char *msg(message to send)
 * Returns : Nothing
 */
void send_msg(int dSC, const char *msg);

#endif
