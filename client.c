#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#define msgLength 256

struct thread_args {
    int dS;
};

int connectSocket(char * arg1, int arg2 ){
    // Create the socket
    int dS = socket(PF_INET, SOCK_STREAM, 0);
    if (dS == -1){
        perror("Error creating socket");
        exit(0);
    }
    // Create the adress
    struct sockaddr_in aS;
    aS.sin_family = AF_INET;
    inet_pton(AF_INET,arg1,&(aS.sin_addr)) ;
    aS.sin_port = htons(arg2) ;
    socklen_t lgA = sizeof(struct sockaddr_in) ;
    // Connect the socket
    if (connect(dS, (struct sockaddr *) &aS, lgA) == -1){
        perror("Error connecting socket");
        exit(0);
    }
    puts ("Socket connected");
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

void* sendMsg(void * args){
    struct thread_args * t_args = (struct thread_args *) args;
    int dS = t_args -> dS;
    int isRunning = 1;
    char * buffer = malloc(msgLength);
  
    while(isRunning == 1){
        // Read the keyboard enter
        puts("Please enter a string of characters :");
        if (fgets(buffer, msgLength, stdin) != NULL) {
            puts ("You have entered:") ;
            puts (buffer);
            size_t inputLength = strlen(buffer)+1; // +1 to include the newline character ('\n')
            char lengthString[20]; // Create a char for create a String of the size
            snprintf(lengthString, 20, "%zu", inputLength-1); // Convert it
            puts ("Number of characters entered:");
            puts (lengthString); //Print it

            int receiveMessage = send(dS, &inputLength, sizeof(size_t), 0);
            if (receiveMessage == -1) {
                perror("Error sending size");
                pthread_exit(0);
            }

            puts ("Input length sent");


            if (send(dS, buffer, inputLength, 0) == -1) {
                perror("Error sending message");
                break; // Go out of the loop if the send dont work
            }
            puts ("Message sent");
            
            // Check if the loop is finished with the word "fin"
            if (compareFin(buffer) == 1){
                isRunning = 0;
                puts ("End of program");
            }
        } 
        else {
            puts ("Error reading or end of file detected");
        }
    }
    free(buffer);
    shutdown(dS, 2); // Close the connection
    return NULL;
}

void* receiveMsg(void* args) {
    struct thread_args * t_args = (struct thread_args *) args;
    int dS = t_args -> dS;
    int isRunning = 1;
    char * buffer = malloc(msgLength);
  
    while(isRunning == 1) {
        puts ("Ready to receive");
        size_t inputLength;

        int receiveSize = recv(dS, &inputLength, sizeof(size_t), 0);
        // Receive the size of the message
        if(receiveSize == -1) {
            perror("Error receiving size");
            pthread_exit(0);
        }
        if(receiveSize == 0){
            puts("Error, disconnected when receiving size");
            pthread_exit(0);
        }

        char * buffer = malloc(inputLength);
        char lengthString[20]; // Create a char for create a String of the size
        snprintf(lengthString, 20, "%zu", inputLength); // Convert it
        puts ("Size received:");
        puts (lengthString); //Print it

        int receiveMessage = recv(dS, buffer, inputLength, 0);
        // Receive the message
        if (receiveMessage == -1) {
            perror("Error receiving message");
            pthread_exit(0);
        }
        if(receiveMessage == 0){
            puts("Error, disconnected when receiving message");
            pthread_exit(0);
        }

        puts ("Message received:");
        puts (buffer);

        // Check if the loop is finished with the word "fin"
        if (compareFin(buffer) == 1){
            isRunning = 0;
            puts ("End of program");
        }
    }
    free(buffer);
    shutdown(dS, 2); // Close the connection
    return NULL;
}


int main(int argc, char *argv[]) {
    // Check the number of arguments
    if (argc != 3) {
        puts ("Invalid number of arguments, usage:");
        printf("%s IpServer Port\n", argv[0]);
        exit(0);
    }

    // Connect the socket
    int dS = connectSocket(argv[1], atoi(argv[2]) );
    pthread_t thread1, thread2;
    struct thread_args args1 = {dS};
    
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
