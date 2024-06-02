#ifndef COMMAND_H
#define COMMAND_H

#include <stdio.h>
#include <pthread.h> // Include for thread management

/**
 * Handles the "@send_file" command by initializing a file sending operation.
 */
void cmd_send_file(int dSC);

/**
 * Handles the "@receive_file" command by initializing a file receiving operation.
 */
void cmd_receive_file();

/**
 * Executes a command based on the provided string.
 * @param command The command string to execute.
 * @param dSC The socket descriptor of the client connection.
 */
void execute_command(const char *command, int dSC);

#endif // COMMAND_H
