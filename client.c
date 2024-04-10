#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#define msgLenght 256

int main(int argc, char *argv[]) {
  //connect the socket
  int dS = socket(PF_INET, SOCK_STREAM, 0);
  printf("Socket created\n");
  struct sockaddr_in aS;
  aS.sin_family = AF_INET;
  inet_pton(AF_INET,argv[1],&(aS.sin_addr)) ;
  aS.sin_port = htons(atoi(argv[2])) ;
  socklen_t lgA = sizeof(struct sockaddr_in) ;
  connect(dS, (struct sockaddr *) &aS, lgA);
  printf("Socket connected\n");

  //create a variable to know who should receive and send
  //create a varible to know when the loop should stop
  int isRunning = 1;
  int client = atoi(argv[3]);
  char buffer [msgLenght];
  while(isRunning == 1){
    if(client == 1){
      printf("Please enter a string of characters : \n");
      
      //read the keyboard enter
      if (fgets(buffer, msgLenght, stdin) != NULL) {
        printf("You have entered : %s\n", buffer);
      } else {
        printf("Error reading or end of file detected.\n");
      }

      //send the message
      send(dS, buffer, strlen(buffer) , 0);
      printf("Message sent \n");
      client = 2;
    }
    else{
      //receive the message
      printf("Ready to receive\n");
      recv(dS, buffer, msgLenght, 0) ;
      printf("Message received : %s\n", buffer);
      client = 1;
    }

    //check if the loop is finished with the word "fin"
    char compare [256];
    memset(compare, '\0', sizeof(compare));
    compare[0] = 'f';
    compare[1] = 'i';
    compare[2] = 'n';
    compare[3] = '\n';
    if(strcmp(buffer,compare) == 0){
      shutdown(dS,2) ;
      isRunning = 0;
      printf("End of program\n");
      shutdown(dS, 2) ; 
    }
  }
}