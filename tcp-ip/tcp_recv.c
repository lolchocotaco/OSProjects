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


int main (int argc, char **argv){
	int port = 8000;
	int rqst;
	socklen_t alen;
	struct sockaddr_in svcAddr;    
	struct sockaddr_in client_addr;  
	int sockoptval = 1;
	int svc;
	char buffer[1024];
	int readBytes;

	memset(buffer,0,sizeof(buffer));

	char hostname[1024];
	gethostname(hostname,1024);

	if((svc = socket(AF_INET, SOCK_STREAM,0)) <0){
		perror("Cannot create socket: ");
		exit(-1);
	}

	setsockopt(svc,SOL_SOCKET,SO_REUSEADDR, &sockoptval, sizeof(int));

	memset((char*)&svcAddr, 0, sizeof(svcAddr));  
	svcAddr.sin_family = AF_INET;
	svcAddr.sin_port = htons(port);
	svcAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	if (bind(svc, (struct sockaddr *)&svcAddr, sizeof(svcAddr)) < 0) {
		perror("Bind failed: ");
		exit(-1);
	}

	if (listen(svc, 128) < 0) {
		perror("Listen failed: ");
		exit(-1);
	}

	printf("Server started on %s, listening on port %d\n", hostname, port);

	alen = sizeof(client_addr);
	while(1){
		while ((rqst = accept(svc, (struct sockaddr *)&client_addr, &alen)) < 0) {
            if ((errno != ECHILD) && (errno != ERESTART) && (errno != EINTR)) {
                perror("Accept failed: ");
                exit(-1);
            } 
        }        
		printf("received a connection from: %s port %d\n", inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));
        while((readBytes = read(rqst,buffer,1024) ) >0){
        	printf("%s",buffer);
        	memset(buffer,0,sizeof(buffer));
        } 
        if(readBytes == 0){
        	printf("Connection closed\n");
        } else{
        	fprintf(stderr,"Error reading");
        	exit(-1);
        }
		

	}

	close(rqst);
    return 0;
}

