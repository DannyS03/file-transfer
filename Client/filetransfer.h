#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <strings.h>

#define MAX_DATA_LENGTH 1024
#define FILE_CHUNK_SIZE 256
#define MAX_ARGS 3

/* struct for the PDU data */
typedef struct pdu {
	char type;
	int length;
	char data[MAX_DATA_LENGTH];
} PDU;


/* this method receives data of a file with given name from a given socket */
void receive_file(int fromsockfd, char* filename);

/* this method sends data of a file with given name to a given socket */
void send_file(int tosockfd, char* filename);


