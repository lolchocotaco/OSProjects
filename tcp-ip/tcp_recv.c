#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/stat.h> 
#include <sys/types.h>
#include <fcntl.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>

#define PORT 8000

int main (int argc, char **argv){

	if(argc <2){
		fprintf(stderr,"Pass valid number of arguments\n");
		exit(-1);
	}

	int port;
	int rqst;
	socklen_t alen;
	struct sockaddr_in server_addr;    
	struct sockaddr_in client_addr;  
	int sockoptval = 1;
	int server;
	char buffer[1024];
	int readBytes;
	int totalBytes= 0;

	memset(buffer,0,sizeof(buffer));

	char hostname[1024];
	gethostname(hostname,1024);
	port = atoi(argv[1]);
	
	// Create socket
	if((server = socket(AF_INET, SOCK_STREAM,0)) <0){
		perror("Cannot create socket: ");
		exit(-1);
	}



	// Set special options on socket
	setsockopt(server,SOL_SOCKET,SO_REUSEADDR, &sockoptval, sizeof(int));

	// Set up server and bind to port. 
	memset((char*)&server_addr, 0, sizeof(server_addr));  
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(port);
	server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	if (bind(server, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
		perror("Bind failed: ");
		exit(-1);
	}

	//Set the server to listen for connections
	if (listen(server, 128) < 0) {
		perror("Listen failed: ");
		exit(-1);
	}

	// Wait for connections and take actions when done. 
	printf("Server started on %s, listening on port %d\n", hostname, port);
	alen = sizeof(client_addr);
	// while(1){
		if ((rqst = accept(server, (struct sockaddr *)&client_addr, &alen)) < 0){
			perror("Accept failed: ");
			exit(-1);
		}
		printf("Received a connection from: %s port %d\n", inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));
        while((readBytes = read(rqst,buffer,1024) ) >0){
        	printf("%s",buffer);
        	memset(buffer,0,sizeof(buffer));
        	totalBytes += readBytes;
        } 
        if(readBytes == 0){
        	printf("\nConnection closed from: %s\n", inet_ntoa(client_addr.sin_addr));
        } else {
        	fprintf(stderr,"Error reading from socket");
        	exit(-1);
        }
        fprintf(stderr,"Total bytes received: %d\n",totalBytes);
		close(rqst); // close after done and wait for new connection.. 
	// }


    return 0;
}

