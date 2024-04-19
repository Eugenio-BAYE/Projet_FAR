#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#define msgLenght 256

struct thread_args {
    int dS;
};

int connectSocket(char * arg1, int arg2 ){
    //connect the socket
    int dS = socket(PF_INET, SOCK_STREAM, 0);

    struct sockaddr_in aS;
    aS.sin_family = AF_INET;
    inet_pton(AF_INET,arg1,&(aS.sin_addr)) ;
    aS.sin_port = htons(arg2) ;
    socklen_t lgA = sizeof(struct sockaddr_in) ;
    connect(dS, (struct sockaddr *) &aS, lgA);
    printf("Socket connected\n");
    return dS;
}

void* sendMsg(void* args){
    int isRunning = 1;
    char buffer [msgLenght];
    struct thread_args * t_args = (struct thread_args *) args;
    int dS = t_args->dS;
    while(isRunning == 1){
        //read the keyboard enter
        printf("Please enter a string of characters : \n");
        if (fgets(buffer, msgLenght, stdin) != NULL) {
          printf("You have entered : %s\n", buffer);
        } else {
          printf("Error reading or end of file detected.\n");
        }

        //send the message
        send(dS, buffer, strlen(buffer) , 0);
        printf("Message sent \n");

        //check if the loop is finished with the word "fin"
        char compare [256];
        memset(compare, '\0', sizeof(compare));
        compare[0] = 'f';
        compare[1] = 'i';
        compare[2] = 'n';
        compare[3] = '\n';
        if(strcmp(buffer,compare) == 0){
            isRunning = 0;
            printf("End of program\n");
        }
    }
    shutdown(dS, 2);
    return NULL;
}

void* receiveMsg(void* args){
    int isRunning = 1;
    char buffer [msgLenght];
    struct thread_args * t_args = (struct thread_args *) args;
    int dS = t_args->dS;
    while(isRunning == 1){
        //receive the message
        printf("Ready to receive\n");
        if(recv(dS, buffer, msgLenght, 0) <= 0){
      break;
    }
        printf("Message received : %s\n", buffer);

        //check if the loop is finished with the word "fin"
        char compare [256];
        memset(compare, '\0', sizeof(compare));
        compare[0] = 'f';
        compare[1] = 'i';
        compare[2] = 'n';
        compare[3] = '\n';
        if(strcmp(buffer,compare) == 0){
            isRunning = 0;
            printf("End of program\n");
        }
    }
    shutdown(dS,2) ;
    return NULL;
}



int main(int argc, char *argv[]) {

    //connect the socket
    int dS = connectSocket(argv[1], atoi(argv[2]) );
    pthread_t thread1, thread2;
    struct thread_args args1 = {dS};
    
    //create first thread
    if (pthread_create(&thread1, NULL, *sendMsg, (void*)&args1) != 0) {
        perror("pthread_create");
        return 1;
    }
    
    //create second thread
    if (pthread_create(&thread2, NULL, receiveMsg, (void*)&args1) != 0) {
        perror("pthread_create");
        return 1;
    }
    
    //wait the end of threads
    if (pthread_join(thread1, NULL) != 0) {
        perror("pthread_join");
        return 1;
    }
    if (pthread_join(thread2, NULL) != 0) {
        perror("pthread_join");
        return 1;
    }
    
return 0;
}
