#include "server.h"

int servsockfd;               /* the server socket descriptor */

int main(int argc, char *argv[])
{

	int connectfd;                /* connected client socket descriptor */
	struct sockaddr_in servaddr;  /* server socket address */
	struct sockaddr_in clientaddr; /* connected client address */
	int len;
	int port;                     /* server port */
	pid_t pid;                    

	port = DEFAULT_PORT;

	signal(SIGINT, handle_signal);

	/* save the port if passed as command line argument */
	if(argc > 1)
	{
		port = atoi(argv[1]);
	}
	/* create the server socket */
	servsockfd = socket(AF_INET, SOCK_STREAM, 0);
	if(servsockfd < 0)
	{
		printf("socket() error!\n"); 
        exit(0); 
	}
	/* initialise the server socket address */
	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET; 
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY); 
    servaddr.sin_port = htons(port);

    /* bind the server port to address */ 
    if ((bind(servsockfd, (struct sockaddr *)&servaddr, sizeof(servaddr))) != 0) { 
        printf("bind() failed!\n"); 
        exit(0); 
    }

    /* start listening for client connections */
    if ((listen(servsockfd, 5)) != 0) { 
        printf("listen() failed!\n"); 
        exit(0); 
    } 
    printf("Server started at port %d...\n", port);

    /* start the concurrent server to accept multiple client connections */
    while(1)
    {
    	/* receive the next client connection */
    	connectfd = accept(servsockfd,NULL, 0);

    	/* fork a child process to handle the client */
    	if((pid = fork()) < 0)
    	{
    		printf("fork() failed!\n"); 
        	exit(0); 
    	}
    	if(pid == 0)
    	{
    		handle_client(connectfd);
    		exit(0);
    	}
    }
	return 0;
}


void handle_client(int connectfd)
{
	PDU pdu;
	char err[50];
	int bytesRead;
	char filename[40];

	/* loop to read client messages */
	bzero(&pdu,sizeof(pdu));
	while((bytesRead = read(connectfd, &pdu, sizeof(pdu))) > 0)
	{
		if(pdu.type == 'D')  /* download file request */
		{
			strncpy(filename, pdu.data, pdu.length);
			send_file(connectfd, filename);
		}
		else if(pdu.type == 'U') /* upload file request */
		{
			strncpy(filename, pdu.data, pdu.length);
			filename[pdu.length] = '\0';
			bzero(&pdu, sizeof(pdu));
			pdu.type = 'R';
			write(connectfd, &pdu, sizeof(pdu));
			receive_file(connectfd, filename);
		}
		else if(pdu.type == 'L')  /* list directory request */
		{
			strncpy(filename, pdu.data, pdu.length);
			filename[pdu.length] = '\0';
			char* list = get_dir_list(filename);
			bzero(&pdu, sizeof(pdu));
			if(list != NULL)
			{
				pdu.type = 'l';
				strcpy(pdu.data, list);
				pdu.length = strlen(list);
			}
			else
			{
				pdu.type = 'E';
				sprintf(err,"directory path %s does not exist!", filename);
				strcpy(pdu.data, err);
				pdu.length = strlen(err);
			}
			
			write(connectfd, &pdu, sizeof(pdu));
		}
		else if(pdu.type == 'P')  /* change directory request */
		{
			strncpy(filename, pdu.data, pdu.length);
			filename[pdu.length] = '\0';
			bzero(&pdu, sizeof(pdu));			
			if(chdir(filename) != 0)
			{
				sprintf(err, "directory %s does not exist!", filename);
				strncpy(pdu.data, err, strlen(err));
				pdu.length = strlen(err);
				write(connectfd, &pdu, sizeof(pdu));
			}
			else
			{
				pdu.type = 'R';
				char *currdir = getcwd(NULL, 0);
				sprintf(pdu.data, "Current directory is %s\n", currdir);
				write(connectfd, &pdu, sizeof(pdu));
			}
		}
		bzero(&pdu,sizeof(pdu));
	}
	/* close connection with client */
	close(connectfd);
}

char* get_dir_list(char *path)
{
	char *list = (char*)malloc(sizeof(char) * MAX_DATA_LENGTH);
	DIR *dir;
	struct dirent *direntry;
	char entry[100];

	if(strcmp(path,".") == 0)
	{
		strcat(path,"/");
	}
	dir = opendir(path);
	if(dir == NULL)
	{
		return NULL;
	}
	while((direntry = readdir(dir)) != NULL)
	{
		if(direntry->d_type == DT_DIR){
			sprintf(entry,"%s [ DIR ]",direntry->d_name);
			strcat(list, entry);
		}
		else{
			strcat(list, direntry->d_name);
		}
		strcat(list,"\n");
	}
	return list;
}

void handle_signal(int signo)
{
	if(signo == SIGINT)
	{
		/* close server socket and terminate */
		printf("Server shutting down...\n");
		close(servsockfd);
		exit(0);
	}
}

