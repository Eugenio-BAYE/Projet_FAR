#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <signal.h>
#define msgLength 256

int dS;

struct thread_args {
    int dS;
};

/* handle_sigint : Handles the SIGINT signal (Ctrl-C interruption) by shutting down the specified socket and exiting the program.
 * Parameters: - int sig: The signal number received by the handler.
 * Returns: None, as the function terminates the program by calling exit(1).
 */
void handle_sigint(int sig) {
    printf("Caught signal %d, shutting down socket\n", sig);
    shutdown(dS, 2);
    exit(1);
}

/* connectSocket : connect the socket to the server put in parameter 
 * Parameters: - char *arg1: A pointer to a string representing the server's IP address in IPv4 format.
 *             - int arg2: An integer representing the server's port number.
 * Errors: If the socket cannot be created and if the connection fails, exits with 0.
 * Returns: The socket descriptor (int) on successful connection.
 */
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


/* compareFin : Checks if the provided buffer matches the specific word "fin" followed by \n. 
 * Preconditions: the buffer should not be a NULL pointer
 * Parameters: char *buffer: A pointer to a character array
 * Returns: - 1 if the buffer contains exactly "fin\n".
 *          - 0 (int): If the buffer contains any other string.
 */
int compareFin(char * buffer){
    // Check if the char * contains only the word "fin", return 1 if it contains only "fin" or return 0 otherwise
    if(strcmp(buffer, "fin\n") == 0){
        return 1;
    }
    else{
        return 0;
    }
}

/** sendMsg : Send messages from the user to a server over a socket.
 * Parameters: void *args: Pointer to a 'thread_args' structure which the socket descriptor 'dS'.
 * Errors: If sending the size or message fails or the socket disconnect, it will print an error and exit the thread.
 * Returns: NULL on exit, either after an error or after a termination command ("fin").
 */
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

            int sendSize = send(dS, &inputLength, sizeof(size_t), 0);
            if (sendSize == -1) {
                perror("Error sending size");
                pthread_exit(0);
            }
            if (sendSize == 0) {
            puts("Error, disconnected when sending size");
                pthread_exit(0);
            }
            puts ("Input length sent");


            int sendMessage = send(dS, buffer, inputLength, 0);
            if (sendMessage == -1) {
                perror("Error sending message");
                pthread_exit(0);
            }
            if (sendMessage == 0) {
            puts("Error, disconnected when sending message");
                pthread_exit(0);
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
    return NULL;
}

/** receiveMsg : Receive messages from the user to a server over a socket.
 * Parameters: void *args: Pointer to a 'thread_args' structure which the socket descriptor 'dS'.
 * Errors: If receiving the size or message fails or the socket disconnect, it will print an error and exit the thread.
 * Returns: NULL on exit, either after an error or after a termination command ("fin").
 */
void* receiveMsg(void* args) {
    struct thread_args * t_args = (struct thread_args *) args;
    int dS = t_args -> dS;
    int isRunning = 1;
    char * buffer = malloc(msgLength);
  
    while(isRunning == 1) {
        puts ("Ready to receive");
        size_t inputLength;

        // Receive the size of the message
        int receiveSize = recv(dS, &inputLength, sizeof(size_t), 0);
        if (receiveSize == -1) {
            perror("Error receiving size");
            pthread_exit(0);
        }
        if (receiveSize == 0) {
            puts("Error, disconnected when receiving size");
            pthread_exit(0);
        }

        char * buffer = malloc(inputLength);
        char lengthString[20]; // Create a char for create a String of the size
        snprintf(lengthString, 20, "%zu", inputLength); // Convert it
        puts ("Size received:");
        puts (lengthString); //Print it

        // Receive the message
        int receiveMessage = recv(dS, buffer, inputLength, 0);
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

    dS = connectSocket(argv[1], atoi(argv[2]) );
    signal(SIGINT, handle_sigint);

    pthread_t thread1, thread2;
    struct thread_args args1 = {dS};

    // Create first thread
    if (pthread_create(&thread1, NULL, sendMsg, (void*)&args1) != 0) {
        perror("pthreadSend_create");
        return 1;

    
    // Create second thread
    if (pthread_create(&thread2, NULL, receiveMsg, (void*)&args1) != 0) {
        perror("pthreadReceive_create");
        return 1;
    }
    
    // Wait the end of threads
    if ((pthread_join(thread1, NULL) != 0) || (pthread_join(thread2, NULL) != 0)) {
        shutdown(dS, 2); // Close the connection
        perror("pthread_join");
        return 1;
    }
    
    return 0;
}