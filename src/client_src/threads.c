#include "threads.h"
#include "server_handling.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int create_threads() {
    pthread_t thread1, thread2;
    struct thread_args args1 = { get_dS() };

    // Create first thread
    if (pthread_create(&thread1, NULL, loop_send_msg, (void*)&args1) != 0) {
        perror("pthreadSend_create");
        return 1;
    }

    // Create second thread
    if (pthread_create(&thread2, NULL, loop_receive_msg, (void*)&args1) != 0) {
        perror("pthreadReceive_create");
        return 1;
    }

    return 0;
}