#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#define msgLength 256

int connectSocket(char * arg1, int arg2 ){
    // Connect the socket and the server
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

int compareFin(char * buffer){
    // Check if the char * contains only the word "fin", return 1 if it contains only "fin" or return 0 otherwise
    if(strcmp(buffer, "fin\n") == 0){
        return 1;
    }
    else{
        return 0;
    }
}

struct thread_args {
  int client;
  int dS;
};

void* sendMsg(void * args){
  struct thread_args * t_args = (struct thread_args *) args;
  int client = t_args -> client;
  int dS = t_args -> dS;
    int isRunning = 1;
    char * buffer = malloc(msgLength);
  
    while(isRunning == 1){
        // Read the keyboard enter
        printf("Please enter a string of characters : \n");
        if (fgets(buffer, msgLength, stdin) != NULL) {
            printf("You have entered : %s\n", buffer);
            size_t inputLength = strlen(buffer); // -1 to exclude the newline character ('\n')
            printf("Number of characters entered: %zu\n", inputLength);

            if (send(dS, &inputLength, sizeof(size_t), 0) == -1) {
                perror("Error sending size");
                break; // Go out of the loop if the send dont work
            }
            printf("Input length sent \n");

            if (send(dS, buffer, inputLength+1, 0) == -1) {
                perror("Error sending message");
                break; // Go out of the loop if the send dont work
            }
            printf("Message sent \n");
            
            // Check if the loop is finished with the word "fin"
            if (compareFin(buffer) == 1){
                isRunning = 0;
                printf("End of program\n");
            }
        } 
        else {
            printf("Error reading or end of file detected.\n");
        }
    }
    free(buffer);
    shutdown(dS, 2); // Close the connection
    return NULL;
}

void* receiveMsg(void* args) {
  struct thread_args * t_args = (struct thread_args *) args;
  int client = t_args -> client;
  int dS = t_args -> dS;

    int isRunning = 1;
    char *buffer = malloc(msgLength);
  
    while(isRunning == 1) {
        printf("Ready to receive\n");
        size_t inputLength;

        // Receive the size of the message
        if (recv(dS, &inputLength, sizeof(size_t), 0) == -1) {
            perror("Error receiving size");
            break; // Go out of the loop if the receive dont work
        }

        printf("Size received: %zu",inputLength);

        // Receive the message
        if (recv(dS, buffer, inputLength, 0) == -1) {
            perror("Error receiving message");
            break; // Go out of the loop if the receive dont work
        }

        printf("Message received : %s\n", buffer);

        // Check if the loop is finished with the word "fin"
        if (compareFin(buffer) == 1){
            isRunning = 0;
            printf("End of program\n");
        }
    }
    free(buffer);
    shutdown(dS, 2); // Close the connection
    return NULL;
}


int main(int argc, char *argv[]) {
    // Check the number of arguments
    if (argc != 3) {
        printf("Invalid number of arguments, usage:\n");
        printf("%s IpServer Port\n", argv[0]);
        exit(0);
    }

    // Connect the socket
    int dS = connectSocket(argv[1], atoi(argv[2]) );
    int client = atoi(argv[3]);
    pthread_t thread1, thread2;
  struct thread_args args1 = {client, dS};
    
    // Create first thread
    if (pthread_create(&thread1, NULL, sendMsg, (void*)&args1) != 0) {
        perror("pthread_create");
        return 1;
    }
    
    // Create second thread
    if (pthread_create(&thread2, NULL, receiveMsg, (void*)&args1) != 0) {
        perror("pthread_create");
        return 1;
    }
    
    // Wait the end of threads
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
