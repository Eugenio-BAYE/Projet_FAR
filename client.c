#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <signal.h>

#include "src/client_src/server_handling.h"
#include "src/client_src/threads.h"
#include "src/client_src/file_sending.h"
#include "src/client_src/file_receiving.h"

int main(int argc, char *argv[]) {
  // Check the number of arguments
  if (argc != 3) {
    puts ("Invalid number of arguments, usage:");
    printf("%s IpServer Port\n", argv[0]);
    exit(0);
  }

  // Connect the socket
  signal(SIGINT, handle_sigint);
  set_dS(connect_socket(argv[1], atoi(argv[2])));
  set_addr(argv[1]);
  set_port(atoi(argv[2]));

  if (create_threads() != 0) {
      return 1;
  }

  // Wait the end of threads
  while (get_is_running()){
    sleep(1);
  }

  // Close the connection
  shutdown(get_dS(), 2);
  return 1;
}