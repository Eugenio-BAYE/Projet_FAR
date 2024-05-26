#ifndef SEMAPHORE_H
#define SEMAPHORE_H

#include <semaphore.h>

void create_semaphore();

sem_t* get_semaphore();

void put_in_queue(int dSC);

#endif
