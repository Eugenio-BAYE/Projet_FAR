#ifndef COMMAND_H
#define COMMAND_H

#include <stdio.h>
#include <pthread.h>

/**
 * @brief Initializes a file sending operation for the "@send_file" command.
 * 
 * @param dSC The socket descriptor of the client connection.
 */
void cmd_send_file(int dSC);

/**
 * @brief Initializes a file receiving operation for the "@receive_file" command.
 */
void cmd_receive_file();

/**
 * @brief Executes a command based on the provided string.
 * 
 * @param command The command string to execute.
 * @param dSC The socket descriptor of the client connection.
 */
void execute_command(const char *command, int dSC);

#endif