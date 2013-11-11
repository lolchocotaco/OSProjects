///////////////////////////
// Sameer Chauhan        //
// ECE357                //
// TCP Send   //
///////////////////////////



#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/stat.h> 
#include <sys/types.h>
#include <fcntl.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>


int main (int argc, char **argv){
	int isFile = 0;

	if(argc <3){
		fprintf(stderr,"Please input the correct number of arguments\n");
		exit(-1);
	} else if(argc == 4){
		isFile = 1;
	}

	struct sockaddr_in server;
	char* hostName;
	int client;
	int port;
	int connection;
	int sendStat;
	char * text = "This is testing text\n";


	if((client = socket(AF_INET,SOCK_STREAM,0)) < 0){
		perror("Could not create TCP socket: ");
		exit(-1);
	}
	hostName = (char *)malloc(strlen(argv[1]));
	strcpy(hostName,argv[1]);   // Save hostName
	port = atoi(argv[2]); 		//Save port number

	printf("Connecting to %s on port %d...\n",hostName,port);

	server.sin_family = AF_INET;
	server.sin_port = htons(port);

	if( (connection = connect(client, (struct sockaddr *)&server, sizeof(server) )) <0){
		perror("Could not connect to server: ");
		exit(-1);
	} else{
		printf("Connection succeeeded\n");
	}

	if ((sendStat = send(client,text,strlen(text),0)) <0){
		perror("Sending failed");
	} else{
		printf("Sent %d bytes\n",sendStat);
	}
	close(client);

	return 0;
}