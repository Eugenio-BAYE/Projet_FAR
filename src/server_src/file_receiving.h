#ifndef FILE_RECEIVING_H
#define FILE_RECEIVING_H

//*********|WARNING|************/
// Here it's the "@receive_file" command handling so the
// server is actually SENDING the file to the client

/* create_file_receiving_socket : Create a new TCP server socket for file receiving
 * Parameters : int port (socket port)
 * Returns : Nothing
 * Postconditions : Non-blocking, can throw errors
 */
void create_file_receiving_socket(int port);

/* file_receiving_thread : Thread for receiving files
 * Parameters : None
 * Returns : void* (NULL)
 * Postconditions : Non-blocking, can throw errors
 */
void* file_receiving_thread();

#endif
