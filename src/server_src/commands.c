#include "server_utils.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <semaphore.h>
#include <pthread.h>

#include "client_handling.h"
#include "../../server.h"
#include "file_receiving.h"
#include "file_sending.h"

void cmd_random(int dSC) {
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
  broadcast_message(dSC, message, message_size);
}

void cmd_msg(int dSC, const char* command) {
  printf("cmd_msg\n");
  char *command_copy = strdup(command);
  printf("cmd_msg : strdup\n");
  if (command_copy == NULL) {
    fprintf(stderr, "Memory allocation failed\n");
    return;
  }

  char *cmd = strtok(command_copy, " ");
  char *username = strtok(NULL, " ");
  char *message = strtok(NULL, "");

  if (username == NULL || message == NULL) {
    send_msg(dSC, "Error: Invalid command syntax. Use @msg <username> <message>");
  } else {
    int recipient = find_client_by_username(username);
    if (recipient == -1) {
      send_msg(dSC, "Error: User not found");
    } else {
      int size = strlen(message)+1+5;
      char* msg = malloc(size);
      snprintf(msg, size, "->Me %s", message);
      printf("%d %s\n",size, msg);
      int formated_size = formated_msg_size(dSC, size);
      char* formated_msg = malloc(formated_size);
      format_msg(msg, dSC, formated_size, formated_msg);
      printf("%d %s\n",formated_size, formated_msg);
      send_msg(recipient, formated_msg);
      free(formated_msg);
    }
  }

  free(command_copy);
}

void cmd_shutdown(int dSC){
  char username[21];
  find_client_username(dSC, username);
  char msg[50];
  sprintf(msg, "%s closed the server\n", username);
  broadcast_message(dSC, msg, strlen(msg)+1);
  shutdown_server();
}

void cmd_quit(int dSC, sem_t semaphore){
  send_msg(dSC, "Disconnecting from server...\n");
  remove_client(dSC);
}


void cmd_man(int dSC){
  FILE *file = fopen("src/server_src/man.txt", "r");
  if (file == NULL) {
    send_msg(dSC, "Error opening man.txt");
    return;
  }

  char line[256];
  while (fgets(line, sizeof(line), file)) {
    send_msg(dSC, line);
  }

  fclose(file);
}

void cmd_send_file() {
  printf("cmd_file_send\n");
  pthread_t file_thread;

  if (pthread_create(&file_thread, NULL, file_receiving_thread, NULL) != 0) {

    perror("Failed to create file handling thread");
    return;
  }

  if (pthread_detach(file_thread) != 0) {
    perror("Failed to detach file handling thread");
    return;
  }
}

void cmd_receive_file(int dSC){
  printf("cmd_file_receive\n");
  pthread_t file_thread;

  struct handle_client_args{
    int normal_dSC;
  };


    struct handle_client_args arg = {dSC};

  if (pthread_create(&file_thread, NULL, file_sending_thread, (void*)&arg) != 0) {
    perror("Failed to create file sending thread");
    return;
  }

  if (pthread_detach(file_thread) != 0) {
    perror("Failed to detach file sending thread");
    return;
  }

}


void cmd_create_channel(int dSC, char *command) {
    // Check for the starting quote of the channel name
    char *start = command + 16;
    if (*start != '"') {
        send_msg(dSC, "Wrong syntax for creating channel");
        return;
    }

    // Find the ending quote of the channel name
    char *end = strchr(start + 1, '"');
    if (!end) {
        send_msg(dSC, "Wrong syntax for creating channel");
        return;
    }

    // Extract the channel name
    *end = '\0';
    char *channel_name = start + 1;

    // Move to the space after the channel name
    start = end + 1;
    if (*start != ' ') {
        send_msg(dSC, "Wrong syntax for creating channel");
        return;
    }
    start++;

    // Check for the starting quote of the description
    if (*start != '"') {
        send_msg(dSC, "Wrong syntax for creating channel");
        return;
    }

    // Find the ending quote of the description
    end = strchr(start + 1, '"');
    if (!end) {
        send_msg(dSC, "Wrong syntax for creating channel");
        return;
    }

    // Extract the description
    *end = '\0';
    char *description = start + 1;

    // Create the channel
    if (create_channel(channel_name, description) == 0) {
        send_msg(dSC, "Channel created successfully");
        printf("Channel %s created successfully.\n", channel_name);
    } else {
        send_msg(dSC, "Failed to create channel. Too many channels");
        printf("Failed to create channel %s. Too many channels\n", channel_name);
    }
}


void cmd_join_channel(int dSC, char* command){
  char *channel_name = command + 14;
  join_channel(dSC, channel_name);
}

void cmd_list_channels(int dSC){
  printf("Entered cmd list channels\n");
    send_msg(dSC, "Availables channels : ");
    list_channels(dSC);
}
