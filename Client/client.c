#include "client.h"


int main(int argc, char *argv[])
{
	int port;                   /* server port */
	int sockfd;                 /* connected socket */
	struct sockaddr_in servaddr; /* server address */

	/* check correct parameters are passed to the program */
	if(argc != 3)
	{
		printf("\nUsage: %s SERVER PORT\n", argv[0]);
		printf("where arguments are as below:\n");
		printf("SERVER   hostname or ip address of server\n");
		printf("PORT     port number of server\n");
		exit(0);
	}
	/* save the port from command line arguments */
	port = atoi(argv[2]);

	/* create the TCP socket */
	sockfd = socket(AF_INET, SOCK_STREAM, 0); 
    if (sockfd == -1) { 
        printf("socket() error!\n"); 
        exit(0); 
    }
    /* prepare the server address */
    bzero(&servaddr, sizeof(servaddr)); 
    servaddr.sin_family = AF_INET; 
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY); 
    servaddr.sin_port = htons(port);

    /* connect with the server */
    if (connect(sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr)) != 0) { 
        printf("Server connection failed!\n"); 
        exit(0); 
    } 
    
    /* run the user interaction menu */
    run(sockfd);
	return 0;
}


void print_menu()
{
	printf("MENU:\n");
	printf("1. Download a file\n");
	printf("2. Upload a file\n");
	printf("3. List files\n");
	printf("4. Change directory\n");
	printf("0. Exit\n");
}

void run(int sockfd)
{
	int choice;
	int done = 0;
	char filedirname[50];
	PDU clientpdu;
	PDU servpdu;

	/* display the greeting message  */
	printf("\nWelcome to File Transfer Program.");

	while(!done)
	{
		print_menu();
		printf("Enter your choice [ 0 - 4]: ");
		scanf("%d", &choice);
		switch(choice)
		{
			case 1:      /* Download file */
				printf("Enter the file name to download: ");
				scanf("%s", filedirname);
				bzero(&clientpdu, sizeof(clientpdu));
				clientpdu.type = 'D';
				strcpy(clientpdu.data, filedirname);
				clientpdu.length = strlen(filedirname);
				write(sockfd, &clientpdu, sizeof(clientpdu));
				receive_file(sockfd, filedirname);
				break;
			case 2:     /* Upload file */
				printf("Enter the file name to upload: ");
				scanf("%s", filedirname);
				bzero(&clientpdu, sizeof(clientpdu));
				clientpdu.type = 'U';
				strcpy(clientpdu.data, filedirname);
				clientpdu.length = strlen(filedirname);
				write(sockfd, &clientpdu, sizeof(clientpdu));
				bzero(&servpdu, sizeof(servpdu));
				read(sockfd, &servpdu, sizeof(servpdu));
				if(servpdu.type == 'R')
				{
					send_file(sockfd, filedirname);
				}				
				break;
			case 3:     /* List files */
				printf("Enter the directory path: ");
				scanf("%s", filedirname);
				bzero(&clientpdu, sizeof(clientpdu));
				clientpdu.type = 'L';
				strcpy(clientpdu.data, filedirname);
				clientpdu.length = strlen(filedirname);
				write(sockfd, &clientpdu, sizeof(clientpdu));
				bzero(&servpdu, sizeof(servpdu));
				read(sockfd, &servpdu, sizeof(servpdu));
				printf("%s\n", servpdu.data);
				break;
			case 4:     /* change directory */
				printf("Enter the new directory path: ");
				scanf("%s", filedirname);
				bzero(&clientpdu, sizeof(clientpdu));
				clientpdu.type = 'P';
				strcpy(clientpdu.data, filedirname);
				clientpdu.length = strlen(filedirname);
				write(sockfd, &clientpdu, sizeof(clientpdu));
				bzero(&servpdu, sizeof(servpdu));
				read(sockfd, &servpdu, sizeof(servpdu));
				printf("%s\n", servpdu.data);
				break;
			case 0:     /* Exit */
				done = 1;
				break;
			default:
				printf("Invalid choice! Must be between 0 - 4.\n");
				break;
		}
	}

	/* close the connection with server */
	close(sockfd);
}


