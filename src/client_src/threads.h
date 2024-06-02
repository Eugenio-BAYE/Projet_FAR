#ifndef THREADS_H
#define THREADS_H

#include <pthread.h>

struct thread_args {
    int dS;
};

/* create_threads : Creates and manages the threads for sending and receiving messages.
 * Parameters: None.
 * Returns: 0 on success, 1 on error.
 * Description: This function creates two threads: one for sending messages and one for receiving messages. It waits for both threads to finish before returning.
 */
int create_threads();

#endif // THREADS_H