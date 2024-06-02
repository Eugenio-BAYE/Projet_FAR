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

/* cmd_msg : Send a message to the client
 * Parameters : int dSC (file descriptor of the client), const char *command (message to send)
 * Returns : Nothing
 */
void cmd_msg(int dSC, const char *command);

/* cmd_quit : Disconnect the client
 * Parameters : int dSC (file descriptor of the client)
 * Returns : Nothing
*/
void cmd_quit(int dSC);

/* cmd_shutdown : Shutdown the server
 * Parameters : int dSC (file descriptor of the client)
 * Returns : Nothing
*/
void cmd_shutdown(int dSC);

/* cmd_man : Send the list of commands to the client
 * Parameters : int dSC (file descriptor of the client)
 * Returns : Nothing
*/
void cmd_man(int dSC);

/* cmd_send_file : Send a file to the client
 * Parameters : int dSC (file descriptor of the client)
 * Returns : Nothing
*/
void cmd_send_file(int dSC);

/* cmd_receive_file : Receive a file from the client
 * Parameters : int dSC (file descriptor of the client)
 * Returns : Nothing

*/
void cmd_receive_file(int dSC);

/* cmd_kick : Kick a client
 * Parameters : int dSC (file descriptor of the client), const char *command (client to kick)
 * Returns : Nothing
*/
void cmd_kick(int dSC, const char* command);

/* cmd_create_channel : Create a channel
 * Parameters : int dSC (file descriptor of the client), const char *command (channel to create and description)
 * Returns : Nothing
*/
void cmd_create_channel(int dSC, char* command);

/* cmd_join_channel : Join a channel
 * Parameters : int dSC (file descriptor of the client), const char *command (channel to join)
 * Returns : Nothing
*/
void cmd_join_channel(int dSC, char* command);

/* cmd_list_channels : List all the channels
 * Parameters : int dSC (file descriptor of the client)
 * Returns : Nothing
*/
void cmd_list_channels(int dSC);

/* cmd_leave_channel : Leave a channel
 * Parameters : int dSC (file descriptor of the client)
 * Returns : Nothing
*/
void cmd_leave_channel(int dSC);

#endif
