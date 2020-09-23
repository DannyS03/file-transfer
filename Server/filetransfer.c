#include "filetransfer.h"


void receive_file(int fromsockfd, char* filename)
{
	PDU pdu;
	int bytesRead;
	FILE *fout = fopen(filename,"w");
	char err[50];

	while((bytesRead = read(fromsockfd, &pdu, sizeof(PDU))) > 0)
	{
		if(pdu.type == 'E')
		{
			remove(filename);
			strncpy(err, pdu.data, pdu.length);
			printf("%s\n\n", err);
			break;
		}
		else if(pdu.type == 'F')
		{
			if(pdu.length > 0){
				fwrite(pdu.data, sizeof(char), pdu.length, fout);
				printf("received %d bytes of data of file %s\n", pdu.length,filename);
			}
			else break;
		}
	}
	fclose(fout);
}

void send_file(int tosockfd, char* filename)
{
	PDU pdu;
	int bytesRead;
	FILE *fin= fopen(filename,"r");
	char err[50];

	if(fin == NULL)  /* file could not be opened for reading */
	{
		bzero(&pdu, sizeof(pdu));
		pdu.type = 'E';
		sprintf(err,"file %s does not exist", filename);
		strcpy(pdu.data, err);
		pdu.length = strlen(err);
		write(tosockfd, &pdu, sizeof(pdu));
		return;
	}
	bzero(&pdu, sizeof(pdu));
	/* read chunks of file data and send it to socket */
	while((bytesRead = fread(pdu.data, sizeof(char), FILE_CHUNK_SIZE, fin) > 0))
	{
		pdu.length = FILE_CHUNK_SIZE;
		pdu.type = 'F';
		write(tosockfd, &pdu, sizeof(pdu));
		printf("Sent %d bytes of data of file %s\n", pdu.length, filename);
		bzero(&pdu, sizeof(pdu));
	}
	pdu.type = 'F';
	pdu.length = 0;
	printf("Sent %d bytes of data of file %s\n\n", pdu.length, filename);
	write(tosockfd, &pdu, sizeof(pdu));
	fclose(fin);
}

