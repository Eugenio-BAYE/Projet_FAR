#ifndef FILE_RECEIVING_H
#define FILE_RECEIVING_H

#include <stdio.h> // Pour les opérations de fichier
#include <sys/types.h> // Pour les types comme size_t
#include <sys/socket.h> // Pour les sockets
#include <netinet/in.h> // Pour les structures sockaddr
#include <arpa/inet.h> // Pour les fonctions de manipulation d'adresses IP

// Fonction pour créer une socket de réception de fichier et se connecter
void create_file_receiving_socket(int port);

// Fonction pour créer un répertoire s'il n'existe pas
void create_directory_if_not_exists(const char *path);

// Fonction pour recevoir le nom d'un fichier d'un socket
char* receive_file_name(int socket);

// Fonction pour générer un chemin de fichier unique dans un répertoire donné
void generate_unique_file_path(char *file_path, const char *dir, const char *file_name);

// Fonction pour recevoir un fichier et l'écrire dans le fichier système
void receive_and_write_file(int socket, FILE *file);

// Thread principal pour gérer la réception de fichiers
void* file_receiving_thread(void *arg);

#endif // FILE_RECEIVING_H
