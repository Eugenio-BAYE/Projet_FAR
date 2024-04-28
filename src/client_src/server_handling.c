#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>

int connect_socket(char * arg1, int arg2 ){
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
