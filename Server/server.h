#include <stdlib.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdio.h>
#include <netinet/in.h>
#include <strings.h>
#include <dirent.h> 
#include <signal.h>

#include "filetransfer.h"

#define DEFAULT_PORT 9000


/* this method communicates with a single client with given socket connection */
void handle_client(int connectfd);

/* get the list of files/subfolders in given directory */
char* get_dir_list(char *path);

/* handle CTRL+C signal to gracefully exit the server */
void handle_signal(int signo);

