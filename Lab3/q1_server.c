#include <string.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <fcntl.h>
#include <sys/stat.h>

int  main() {
	char buf[1024], str[1024];
	struct sockaddr_in  serv, client;
	socklen_t size;

	printf("Enter Port Number : ");
	int p ;
	scanf("%d" , &p);
	printf("\n");

	int s;
	s = socket(AF_INET , SOCK_STREAM , 0);
	if( s == -1) {
		printf("Connection error\n");
		exit(0);
	}
	printf("Socket creation OK\n");

	serv.sin_family = AF_INET;
	serv.sin_port = htons(p);
	serv.sin_addr.s_addr = htonl(INADDR_ANY);

	int k;

	k = bind(s, (struct sockaddr *)&serv , sizeof(serv));

	if( k == -1) {
		printf("Connection error\n");
		exit(0);
	}


	printf("Binding OK!\n");

	if(listen(s , 10) == 0)
		printf("Listening ...\n");
	else {
		printf("Connection error\n");
		close(s);
		exit(0);
	}

	int ns;

	size = sizeof(client);
	ns = accept(s , (struct sockaddr *)&client , &size);

	if(ns == -1) {
		printf("Connection error\n");
		close(s);
		exit(0);
	}
	printf("Connection OK\n");

	pid_t pid;
	pid = fork();

	if( pid == 0 ) {
		printf("===Child Process===\n");
		printf("Process ID : %d\n", getpid());
		printf("Parent ID  : %d\n", getppid());
		printf("===================\n");
	}
	else if( pid > 0 ) {
		printf("===Parent Process===\n");
		printf("Process ID : %d\n", getpid());
		printf("Parent ID  : %d\n", getppid());
		printf("====================\n");
	}

	while(1) {
		if( pid == 0) {
			//printf("Enter message to send to client : ");
			scanf("%s" , buf);
			printf("\n");
			int sd = send(ns , buf , sizeof(buf) , 0);
			
			if((strcmp(buf , "Stop") == 0 ) || (strcmp(buf , "stop") == 0)) {
				printf("Terminating server.\n");
				break;
			}
			else {	
				if( sd < 0) {
					printf("Message not sent.\n");
				}
				else {
					printf("Message sent.\n");
				}
			}
		}
		else if ( pid > 0) {
			int rv = recv(ns , str , sizeof(str) , 0);
			if( rv < 0 ) {
				printf("Message reception error.\n");
			}
			else {
				printf("Client : %s\n" , str);
				if((strcmp(str , "Stop") == 0) || (strcmp(str , "stop") == 0)) {
					printf("Terminating server");
					break;
				}
			}
		}
	}

	close(ns);
	close(s);
}