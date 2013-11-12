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
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <time.h>


#define PORT 8000

int main (int argc, char **argv){
	int isFile = 0;
	int inFile = -1;
	char* fileName;

	if(argc <3){
		fprintf(stderr,"Please input the correct number of arguments\n");
		exit(-1);
	} 


	struct sockaddr_in server;
	struct hostent *host;
	char* hostName;
	unsigned char **addrList;
	int client;
	int port;
	int connection;
	int sendStat;
	int byteSent;
	clock_t time1;
	int i;
	char line[1024];


	// Create socket
	if((client = socket(AF_INET,SOCK_STREAM,0)) < 0){
		perror("Could not create TCP socket: ");
		exit(-1);
	}
	hostName = (char *)malloc(strlen(argv[1]));
	strcpy(hostName,argv[1]);   // Save hostName
	port = atoi(argv[2]); 		//Save port number


	// Setting up connection
	printf("Connecting to %s on port %d...\n",hostName,port);
	memset((char*)&server, 0, sizeof(server));
	server.sin_family = AF_INET;
	server.sin_port = htons(port);


	// get proper host address
	if(isdigit(hostName[0])){
		inet_aton(hostName,&server.sin_addr);
	} else{
		if ( (host = gethostbyname(hostName)) == NULL) {
			fprintf(stderr, "Could not lookup '%s': %s\n",hostName, strerror(h_errno));
			exit(-1);
		}
		memcpy(&server.sin_addr, host->h_addr, sizeof(server.sin_addr));
	}


	//Connect to host
	if( (connection = connect(client, (struct sockaddr *)&server, sizeof(server) )) <0){
		perror("Could not connect to server: ");
		exit(-1);
	} else{
		printf("Connection succeeeded\n");
	}

	byteSent = 0;
	// Read from stdin and send to server
	time1 = clock();
	while (1) {
		if (!fgets(line, 1024, stdin)) {
			printf("Closing Connection..\n");
			break;
		}
		if ((sendStat = send(client, line, strlen(line),0)) <0){
			perror("Sending failed\n");
			break;
		}
		byteSent += sendStat;

	}
	time1 = clock()-time1;
	fprintf(stderr,"Total bytes sent: %d \n",byteSent);
	fprintf(stderr,"Throughput: %f Mb/sec\n",byteSent/(1000000*((double)time1)/CLOCKS_PER_SEC));
	close(client);
	return 0;
}