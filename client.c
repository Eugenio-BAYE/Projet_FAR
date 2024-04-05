#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int main(int argc, char *argv[]) {

  printf("Début programme\n");
  int dS = socket(PF_INET, SOCK_STREAM, 0);
  printf("Socket Créé\n");
  struct sockaddr_in aS;
  aS.sin_family = AF_INET;
  inet_pton(AF_INET,argv[1],&(aS.sin_addr)) ;
  aS.sin_port = htons(atoi(argv[2])) ;
  int nbr = atoi(argv[3]);
  socklen_t lgA = sizeof(struct sockaddr_in) ;
  connect(dS, (struct sockaddr *) &aS, lgA);
  printf("Socket Connecté\n");




  char buffer[100];
  printf("Veuillez entrer une chaîne de caractères : ");
  if (fgets(buffer, 100, stdin) != NULL) {
    printf("Vous avez saisi : %s\n", buffer);
  } else {
    printf("Erreur de lecture ou fin de fichier détectée.\n");
  }

  send(dS, buffer, strlen(buffer) , 0);
  printf("Message Envoyé \n");

  int r;
  recv(dS, &r, sizeof(int), 0) ;
  printf("Réponse reçue : %d\n", r) ;

  shutdown(dS,2) ;
  printf("Fin du programme");
}