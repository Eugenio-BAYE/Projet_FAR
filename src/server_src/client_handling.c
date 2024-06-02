#include "client_handling.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>
#include <semaphore.h>

#include "semaphore.h"
#include "server_utils.h"

#define MAX_CLIENT 10
#define MAX_CHANNELS 10
#define CHANNEL_DESCRIPTION_LENGTH 128
#define CHANNEL_NAME_LENGTH 20
#define BUFFER_SIZE 1024

typedef struct {
    char name[CHANNEL_NAME_LENGTH];
    char description[CHANNEL_DESCRIPTION_LENGTH];
    int client_count;
} Channel;

typedef struct {
    int dSC;
    char username[21];
    int username_lenght;
    char channel[CHANNEL_NAME_LENGTH];
} Client;

static Client clients[MAX_CLIENT];
static Channel channels[MAX_CHANNELS];
static int nbr_of_clients = 0;
int channel_count = 0;
static pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
// We will keep the semaphore herer as a global variable

void list_channels(int dSC) {
  pthread_mutex_lock(&mutex);
  char buffer[BUFFER_SIZE];
  for (int i = 0; i < channel_count; i++) {
    snprintf(buffer, BUFFER_SIZE, "Channel Name: %s\nDescription: %s\nNumber of Users: %d\n\n",
             channels[i].name, channels[i].description, channels[i].client_count);
    send_msg(dSC, buffer);
  }
  pthread_mutex_unlock(&mutex);
}

int join_channel(int dSC, char *channel_name) {
  pthread_mutex_lock(&mutex);
  send_msg(dSC, "Joining channel");
  for (int i = 0; i < channel_count; i++) {
    if (strcmp(channels[i].name, channel_name) == 0) {
      // Find the client and add them to the channel
      for (int j = 0; j < MAX_CLIENT; j++) {
        if (clients[j].dSC == dSC) {
          strncpy(clients[j].channel, channel_name, CHANNEL_NAME_LENGTH - 1);
          clients[j].channel[CHANNEL_NAME_LENGTH - 1] = '\0';
          channels[i].client_count++;
          pthread_mutex_unlock(&mutex);
          send_msg(dSC, "Channel joined successfully");
          return 0; // Joined successfully
        }
      }
    }
  }
  pthread_mutex_unlock(&mutex);
  return -1; // Channel not found
}

int create_channel(const char *channel_name, const char *description) {
    pthread_mutex_lock(&mutex);
    if (channel_count >= MAX_CHANNELS) {
        pthread_mutex_unlock(&mutex);
        return -1; // Maximum number of channels reached
    }
    strncpy(channels[channel_count].name, channel_name, CHANNEL_NAME_LENGTH - 1);
    channels[channel_count].name[CHANNEL_NAME_LENGTH - 1] = '\0';
    strncpy(channels[channel_count].description, description, CHANNEL_DESCRIPTION_LENGTH - 1);
    channels[channel_count].description[CHANNEL_DESCRIPTION_LENGTH - 1] = '\0';
    channels[channel_count].client_count = 0;
    channel_count++;
    pthread_mutex_unlock(&mutex);
    puts(channel_name);
    puts(description);
    return 0; // Channel created successfully
}


void find_client_username(int dSC, char username[]){
  for (int i = 0; i < MAX_CLIENT; i++) {
    if (clients[i].dSC==dSC) {
      snprintf(username, 21, "%s", clients[i].username);
    }
  }
}

int find_client_by_username(const char *username) {
  for (int i = 0; i < MAX_CLIENT; i++) {
    if (strcmp(clients[i].username, username) == 0) {
      return clients[i].dSC;
    }
  }
  return -1; // Return -1 if the client was not found
}

int formated_msg_size(int dSC, int msg_size){
  int size = 0;
  for(int i=0; i<MAX_CLIENT; i++){
    if (clients[i].dSC==dSC){
      size=clients[i].username_lenght+3+msg_size;
    }
  }
  return size;
}

void format_msg(char msg[], int dSC, int size, char formated_msg[]){
  memset(formated_msg, '\0', size);
  for(int i=0; i<MAX_CLIENT; i++){
    if(clients[i].dSC==dSC){
      snprintf(formated_msg, size, "<%s> %s", clients[i].username, msg);
    }
  }
}

int is_username_valid(char username[]){
  if(strlen(username)>20){
    return 2;
  }
  for(int i=0; i<MAX_CLIENT; i++){
    if (strcmp(clients[i].username, username) == 0){
      return 3; 
    }
  }
  return 1;
}

void update_username(int dSC, char username[]){
  if(is_username_valid(username) != 1){
    perror("Can't update_username() on invalid username");
    return;
  }

  for(int i=0; i<MAX_CLIENT; i++){
    if (clients[i].dSC == dSC){
      printf("New client name : %s\n", username);
      strncpy(clients[i].username, username, sizeof(clients[i].username) - 1);
      clients[i].username[sizeof(clients[i].username) - 1] = '\0'; // ensure null termination
      clients[i].username_lenght = strlen(clients[i].username);
    }
  }
}

int ask_username(int dSC) {
  int is_valid = 0;
  char username[21];
  memset(username, '\0', 21);
  char* message = "Please enter your username: \n";
  while(!is_valid){
    send_msg(dSC, message);
    size_t input_length;
    if (recv(dSC, &input_length, sizeof(input_length), 0)==0){
      remove_client(dSC);
      return 0;
    }
    char *input = malloc(input_length);
    int length = receive_message(dSC, input, input_length);
    if (length > 0) {
      input[length-1] = '\0'; // ensure null termination
      printf("Received username : %s\n", input);
      int valid_code = is_username_valid(input);
      if(valid_code == 1){
        is_valid = 1;
        strncpy(username, input, sizeof(username) - 1);
        username[sizeof(username) - 1] = '\0'; // ensure null termination
        update_username(dSC, username);
        return 1;
      }
      else if(valid_code == 2){
        send_msg(dSC, "Username is too long\0");
      }
      else if(valid_code == 3){
        send_msg(dSC, "Username is already taken\0");
      }
    }
    else {
      perror("Error receiving username");
    }
    free(input);
  }
}

int get_nbr_of_clients(){
  return nbr_of_clients;
}

int get_max_client(){
  return MAX_CLIENT;
}

void free_client_list(){
  for (int i = 0; i < MAX_CLIENT; i++){
    clients[i].dSC=0;
    strcpy(clients[i].username, "Anonymous");
  }
}

int can_accept_new_client(sem_t *semaphore) {
    if (sem_wait(semaphore) != 0) {
        perror("Failed to wait on semaphore");
        return -1;  // Return -1 on fail
    }
    return 0;  // Return 0 on success
}


void add_new_client(int dSC){
  nbr_of_clients++;
  pthread_mutex_lock(&mutex);
  for (int i = 0; i < MAX_CLIENT; i++){
    if(clients[i].dSC==0){
      clients[i].dSC=dSC;
      break;
    }
  }
  pthread_mutex_unlock(&mutex);
}

void broadcast_message(int sender, char *message, size_t message_size) {
  printf("Message broadcasting : %s\n", message);
  pthread_mutex_lock(&mutex); // Lock mutex for thread safety
  message_size++;
  for (int i = 0; i < MAX_CLIENT; i++) {
    if (clients[i].dSC != 0 && clients[i].dSC != sender) {
      send(clients[i].dSC, &message_size, sizeof(size_t), 0);
      send(clients[i].dSC, message, message_size, 0);
    }
  }
  pthread_mutex_unlock(&mutex);
}

void remove_client(int dSC){
  pthread_mutex_lock(&mutex);
  for (int i = 0; i < MAX_CLIENT; i++) {
    if (clients[i].dSC == dSC) {
      clients[i].dSC = 0;
      strcpy(clients[i].username, "Anonymous");
      break;
    }
  }
  pthread_mutex_unlock(&mutex);
  sem_post(get_semaphore());
  nbr_of_clients--;
  close(dSC);
  printf("Client disconnected\n");
}

int new_client_connection(int server_socket) {
  struct sockaddr_in client_addr;
  socklen_t client_addr_len = sizeof(client_addr);
  int client_socket = accept(server_socket, (struct sockaddr*) &client_addr, &client_addr_len);
  printf("New client connected\n");
  return client_socket;
}

int receive_message(int dSC, char msg[], int msgLenght){
  memset(msg, '\0', msgLenght);
  int received_size = recv(dSC, msg, msgLenght, 0);
  return received_size;
}

int receive_msg_size(int dSC, size_t *size) {
  size_t message_size;
  printf("Create message size\n");
  int received_size = recv(dSC, &message_size, sizeof(message_size), 0);
  if (received_size > 0) {
    printf("Message size received: %d\n", (int)message_size);
    *size = message_size;
  } else if (received_size == 0) {
    printf("Client disconnected.\n");
  } else {
    perror("recv");
  }
  return received_size;
}

