///////////////////////////
// Sameer Chauhan        //
// ECE357                //
// UDP Send   			//
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
#include <sys/sysinfo.h>

#define PORT 8000

int main(int argc, char**argv)
{
	int sockfd,n;
	struct sockaddr_in servaddr,clientAddr;
	socklen_t len;
	char *mesg;
	char *response;
	int port;
	struct sysinfo info;
	time_t t;
	struct tm tm;


	if(argc !=2){
		fprintf(stderr, "Please put correct arguments\n");
		exit(-1);
	}

	port = atoi(argv[1]);
	if ( (sockfd=socket(AF_INET,SOCK_DGRAM,0))<0){
		fprintf(stderr,"Error making socket\n");
		exit(-1);
	}

	mesg = malloc(1024);
	response = malloc(1024);

	if (mesg == NULL || response == NULL){
		fprintf(stderr,"Malloc failed!\n");
		exit(-1);
	}


	bzero(&servaddr,sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr=htonl(INADDR_ANY);
	servaddr.sin_port=htons(port);
	if (bind(sockfd,(struct sockaddr *)&servaddr,sizeof(servaddr)) < 0){
		fprintf(stderr,"Error binding\n");
		exit(-1);	;
	}
	printf("Accepting connections on port %d\n",port);
	while (1) {
		len = sizeof(clientAddr);
		n = recvfrom(sockfd,mesg,1024,0,(struct sockaddr *)&clientAddr,&len);
		if(n<0){
        	fprintf(stderr,"Error recvfrom\n");
        	exit(-1);
		}
        mesg[n] = 0;
		printf("Recieved packet from %s:%d\nData: %s\n",inet_ntoa(clientAddr.sin_addr),ntohs(clientAddr.sin_port),mesg);
		
		if (strncmp(mesg, "UPTIME",1024) == 0){
			sysinfo(&info);
			sprintf(response,"%f minutes\n",info.uptime/60.0);
		} else if( strncmp(mesg,"DATE",1024) == 0){
			t = time(NULL);
			tm = *localtime(&t);
			sprintf(response,"Today is %d-%d-%d at %d:%d:%d\n", tm.tm_mon + 1, tm.tm_mday,tm.tm_year + 1900, tm.tm_hour % 12, tm.tm_min, tm.tm_sec);
		} else {
			sprintf(response,"'%s' is not a valid request\n",mesg);
		}

		//Send response to client
		sendto(sockfd,response,strlen(response),0,(struct sockaddr *)&clientAddr,sizeof(clientAddr));
		memset(response,0,sizeof(response));
	}


	close(sockfd);
	return 0;
}