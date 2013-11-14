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
#include <time.h>

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
	struct hostent *client;
	char* clientName;
	int server;
	char * buffer;
	int readBytes;
	int totalBytes= 0;
	clock_t time1;


	char* hostname;
	hostname = (char*)malloc(1024);
	buffer = (char*)malloc(1024);
	
	if( buffer == NULL || hostname == NULL){
		fprintf(stderr,"Malloc failed!");
		exit(-1);
	}

	gethostname(hostname,1024);
	port = atoi(argv[1]);


	// Create socket
	if((server = socket(AF_INET, SOCK_STREAM,0)) <0){
		perror("Cannot create socket: ");
		exit(-1);
	}




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
	fprintf(stderr,"Server started on %s, listening on port %d\n", hostname, port);
	alen = sizeof(client_addr);


	time1 = clock();
	if ((rqst = accept(server, (struct sockaddr *)&client_addr, &alen)) < 0){
		perror("Accept failed: ");
		exit(-1);
	}
	fprintf(stderr,"Received a connection from: %s port %d\n", inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));
    while((readBytes = read(rqst,buffer,1024) ) >0){
    	printf("%s",buffer);
    	memset(buffer,0,sizeof(buffer));
    	totalBytes += readBytes;
    } 
    if(readBytes == 0){
    	fprintf(stderr,"Client closed connection\n");
    } else {
    	fprintf(stderr,"Error reading from socket");
    	exit(-1);
    }

    time1 = clock() - time1;

    //DNS Lookup
    if ( (client = gethostbyaddr(&client_addr.sin_addr,sizeof(client_addr.sin_addr),AF_INET)) == NULL) {
    	clientName = inet_ntoa(client_addr.sin_addr);
	} else {
		clientName = client->h_name;
	}

    fprintf(stderr,"Total bytes received: %d from %s\n",totalBytes,clientName);
    fprintf(stderr,"Throughput: %f Mb/sec\n",totalBytes/(1000000.0*((double)time1)/CLOCKS_PER_SEC));
	close(rqst);
    return 0;
}

