#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
  int dS = socket(PF_INET, SOCK_STREAM, 0);
  printf("Socket Créé\n");
  struct sockaddr_in aS;
  aS.sin_family = AF_INET;
  inet_pton(AF_INET,argv[1],&(aS.sin_addr)) ;
  aS.sin_port = htons(atoi(argv[2])) ;
  socklen_t lgA = sizeof(struct sockaddr_in) ;
  connect(dS, (struct sockaddr *) &aS, lgA);
  printf("Socket Connecté\n");

  int isRunning = 1;
  int client = atoi(argv[3]);
  char buffer [256];
  while(isRunning == 1){
    if(client == 1){
      printf("Veuillez entrer une chaîne de caractères : ");
      
      if (fgets(buffer, 256, stdin) != NULL) {
        printf("Vous avez saisi : %s\n", buffer);
      } else {
        printf("Erreur de lecture ou fin de fichier détectée.\n");
      }
      send(dS, buffer, strlen(buffer) , 0);
      printf("Message Envoyé \n");
      client = 2;
    }
    else{
      char buffer [256];
      recv(dS, buffer, sizeof(buffer), 0) ;
      printf("Message reçu : %s\n", buffer);
      client = 1;
    }
    if(strcmp(buffer,"fin") == 0){
      shutdown(dS,2) ;
      isRunning = 0;
      printf("Fin du programme");
    }
  }
}