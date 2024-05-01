#include "server_utils.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "client_handling.h"

void cmd_random(int dSC){
  int random = rand() %1000;
  int num_digits = snprintf(NULL, 0, "%d", random);
  
  // Convert the random integer to a string
  char random_str[num_digits + 1];
  sprintf(random_str, "%d", random);

  send_msg(dSC, random_str);
}

void cmd_pile_ou_face(int dSC){
  int random = rand() % 2;
  if(random == 1){
    char* pile = "pile\0";
    send_msg(dSC, pile);
  }
  if(random == 0){
    char* face = "face\0";
    send_msg(dSC, face);
  }
}

void cmd_client(int dSC){
  int nbr_of_clients = get_nbr_of_clients();
  int num_digits = snprintf(NULL, 0, "%d", nbr_of_clients);
  
  // Convert the nbr_of_clients integer to a string
  char nbr_of_clients_str[num_digits + 1];
  sprintf(nbr_of_clients_str, "%d", nbr_of_clients);

  send_msg(dSC, nbr_of_clients_str);
}

void cmd_size(int dSC){
  int max_int = get_max_client();
  int num_digits = snprintf(NULL, 0, "%d", max_int);

  char max_str[num_digits + 1];
  sprintf(max_str, "%d", max_int);
  send_msg(dSC, max_str);
}

void cmd_dascalu(int dSC){
  char* message = "DASCALUUU\0";
  
  int message_size = strlen(message) + 1; 
  broadcast_size(dSC, message_size);
  broadcast_message(dSC, message, message_size);
}
