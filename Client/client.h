#include <stdlib.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdio.h>
#include <strings.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "filetransfer.h"


/* print menu for all commands in client console */
void print_menu();

/* user interaction of client with connected server */
void run(int sockfd);
