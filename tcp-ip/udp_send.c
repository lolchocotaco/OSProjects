///////////////////////////
// Sameer Chauhan        //
// ECE357                //
// UDP Receive             //
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



int main(int argc, char**argv)
{
   int sockfd,n;
   struct sockaddr_in servaddr;
   char*sendLine;
   char*recvLine;
   char *hostName;
   int port; 

   if (argc != 4){
      fprintf(stderr,"Please input the valid arguments\n");
      exit(-1);
   }

   sendLine = malloc(strlen(argv[3]));
   recvLine = malloc(1024);
   hostName = malloc(strlen(argv[1]));
   if(sendLine == NULL || recvLine == NULL || hostName == NULL){
      fprintf(stderr, "Malloc Failed\n");
      exit(-1);
   }


   sendLine = argv[3];
   port = atoi(argv[2]);
   hostName = argv[1];

   if ((sockfd=socket(AF_INET,SOCK_DGRAM,0)) <0 ){
      fprintf(stderr,"Error making socket\n");
      exit(-1);
   }

   bzero(&servaddr,sizeof(servaddr));
   servaddr.sin_family = AF_INET;
   // servaddr.sin_addr.s_addr=inet_addr(hostName);
   servaddr.sin_port=htons(port);

   if (inet_aton(hostName, &servaddr.sin_addr)==0) {
      fprintf(stderr, "Could not convert IP\n");
      exit(-1);
   }

   printf("Attempting to send '%s' to %s on %d\n", sendLine, hostName, port);

   if( sendto(sockfd,sendLine,strlen(sendLine),0, (struct sockaddr *)&servaddr,sizeof(servaddr)) <0){
      fprintf(stderr,"Error sending!\n");
      exit(-1);
   }
   n=recvfrom(sockfd,recvLine,1024,0,NULL,NULL);
   if(n<0){
      fprintf(stderr,"Error recvfrom\n");
      exit(-1);
   }
   recvLine[n] = 0;
   printf("Server responded: %s\n",recvLine);


   close(sockfd);
   return 0;
}