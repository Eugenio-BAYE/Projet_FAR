#ifndef COMMANDS_H
#define COMMANDS_H

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

#endif
