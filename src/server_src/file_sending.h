#ifndef FILE_SENDING_H
#define FILE_SENDING_H   

/* create_file_sending_socket : Create a new TCP server socket for file sending
 * Parameters : int port (socket port)
 * Returns : Nothing
 * Postconditions : Non-blocking, can throw errors
*/
void create_file_sending_socket(int port);

/* file_sending_thread : Thread for sending files
 * Parameters : None
 * Returns : void* (NULL)
 * Postconditions : Non-blocking, can throw errors
*/
void* file_sending_thread();

#endif
