#ifndef SEMAPHORE_H
#define SEMAPHORE_H

#include <semaphore.h>

/* create_semaphore : Create a semaphore, stocked in the file as a global variable
 * Parameters : None
 * Returns : Nothing
*/
void create_semaphore();

/* get_semaphore : Get the semaphore
 * Parameters : None
 * Returns : sem_t* (the semaphore)
*/
sem_t* get_semaphore();

/* put_in_queue : Put the file descriptor in the queue
 * Parameters : int dSC (file descriptor of the client)
 * Returns : Nothing
*/
void put_in_queue(int dSC);

#endif
