#ifndef FILE_SENDING_H
#define FILE_SENDING_H

#include <stdio.h> // Pour FILE, printf, fopen, etc.
#include <stdlib.h> // Pour malloc et autres fonctions de gestion de la mémoire
#include <string.h> // Pour strncpy et autres fonctions de manipulation des chaînes
#include <sys/socket.h> // Pour les fonctions de socket
#include <sys/types.h> // Types de données utilisés par les fonctions de système
#include <sys/stat.h> // Pour mkdir
#include <unistd.h> // Pour close
#include <dirent.h> // Pour les opérations sur les répertoires
#include "server_handling.h" // Pour les prototypes de connect_socket, etc.

#define DIRECTORY_PATH "./stocked_files" // Chemin du répertoire de stockage des fichiers
#define BUFFER_SIZE 1024 // Taille du buffer utilisé pour la lecture/écriture des fichiers

typedef struct {
    int dS_sender;
    char *selected_file;
} FileSendArgs;

// Prototype de la fonction pour envoyer la liste des fichiers disponibles au client
void send_file_list(void);

// Prototype de la fonction pour recevoir la sélection du client
int receive_client_selection(char *selected_file);

// Prototype de la fonction pour générer un chemin de fichier unique
void generate_unique_file_path(char *file_path, const char *dir, const char *file_name);

// Prototype de la fonction pour envoyer le fichier sélectionné au client
void send_file_to_client(int dSC, const char *file_name);

// Prototype de la fonction pour créer un répertoire s'il n'existe pas
void create_directory_if_not_exists(const char *path);

// Prototype de la fonction pour recevoir un nom de fichier d'un socket
char* receive_file_name(int socket);

// Prototype de la fonction pour recevoir et écrire le fichier
void receive_and_write_file(int socket, FILE *file);

// Prototype de la fonction thread principale pour gérer l'envoi de fichiers
void* file_sending_thread(void* args);

#endif // FILE_SENDING_H
