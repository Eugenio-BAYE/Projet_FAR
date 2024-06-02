#ifndef COMMANDS_H
#define COMMANDS_H

#include <semaphore.h>
/* cmd_random : Send a random integer to the client
 * Parameters : int dSC (file descriptor of the client)
 * Returns : Nothing
 */
void cmd_random(int dSC);

/* cmd_pile_ou_face : Send randomly pile ou face to the user
 * Parameters : int dSC (file descriptor of the client)
 * Returns : Nothing
 */
void cmd_pile_ou_face(int dSC);

/* cmd_client : Send the number of clients to the user
 * Parameters : int dSC (file descriptor of the client)
 * Returns : Nothing
 */
void cmd_client(int dSC);

/* cmd_size : Send the maximum number of clients to the user
 * Parameters : int dSC (file descriptor of the client)
 * Returns : Nothing
 */
void cmd_size(int dSC);

/* cmd_dascalu : Send DASCALUUU to every clients
 * Parameters : int dSC (file descriptor of the client)
 * Returns : Nothing
 */
void cmd_dascalu(int dSC);

void cmd_msg(int dSC, const char *command);

void cmd_quit(int dSC);

void cmd_shutdown(int dSC);

void cmd_man(int dSC);

void cmd_send_file(int dSC);

void cmd_receive_file(int dSC);

void cmd_create_channel(int dSC, char* command);

void cmd_join_channel(int dSC, char* command);

void cmd_list_channels(int dSC);

#endif
