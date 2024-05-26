#include <semaphore.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include "server_handling.h"
#include "server_utils.h"

#define MAX_CLIENT 10
sem_t semaphore;


void create_semaphore(){
  int semaphore_is_successful = sem_init(&semaphore, 0, MAX_CLIENT);
  if (semaphore_is_successful==-1){
    perror("Error initializing semaphore");
    exit(EXIT_FAILURE);
  }
}

sem_t* get_semaphore(){
  return &semaphore;
}

void put_in_queue(int dSC){
  while(sem_trywait(&semaphore)!=0){
    int nbr = 0;
    sem_getvalue(&semaphore,&nbr);
    printf("%d\n",nbr);
    send_msg(dSC, "You're in queue...");
    sleep(5);
  }
}

int bool_can_accept_new_client(){
  if(sem_wait(&semaphore)!=0){
    perror("Failed to wait on semaphore");
    return -1;
  }
}
