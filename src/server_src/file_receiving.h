#ifndef FILE_RECEIVING_H
#define FILE_RECEIVING_H

//*********|WARNING|************/
// Here it's the "@receive_file" command handling so the
// server is actually SENDING the file to the client

void create_file_receiving_socket(int port);

void* file_receiving_thread();

#endif
