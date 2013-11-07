///////////////////////////
// Sameer Chauhan        //
// ECE357                //
// Memory Mapped files   //
///////////////////////////



#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/stat.h> 
#include <sys/types.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <signal.h>
#include <string.h>

char* fileName = "a.txt";
char* testTxt = "This is a sample test text block\n";



/*
	Part A: Segmentation fault occurs when writing to a read only map
	Part B: The update is visible immediately
	Part C: The update is not visible in the file
	Part D: The file size through the traditional interface does not change nor is the
			the change reflected in the file. 
	Part E: The file size is changed, however the print command stops once it encounters holes. 
	Part F: There is a bus error when accessing the memory in the second page. 
			mmap can only work with a page at a time, and so the second region of of the mapping doesn't really exist



 */
void abc(int id, int prot, int mapType ){

	int fd;
	struct stat fileStat;
	char* readBuff = (char*)malloc(1024);
	if ( (fd = open(fileName, O_RDWR, S_IREAD | S_IWRITE ) )== -1){
		fprintf(stderr,"Error occured while opening %s: %s\n", fileName, strerror(errno));
		exit(-1);
	}

	fstat(fd,&fileStat);
	char *m = mmap(0, fileStat.st_size, prot, mapType, fd, 0);
	if(id =='a'){
		printf("Writing to the protected space will cause it to seg fault\n");
	}

	int w = snprintf(m, 1024, "%s",testTxt);
	if( (id == 'b'|| id == 'c') ){
		if (read(fd, readBuff, 1024) <0 ){
			fprintf(stderr,"Error occured while reading %s, %s\n", fileName, strerror(errno));
			exit(-1);
		}
		if (strcmp(readBuff,testTxt) ==0){
		  	printf("Immediately visible\n");
		} else{
			printf("Not Visible\n");
		}
	}
	
}
void de(int id){
	int fd;
	struct stat fileStat;
	char* readBuff = (char*)malloc(1024);
	if ( (fd = open(fileName, O_RDWR, S_IREAD | S_IWRITE ) )== -1){
		fprintf(stderr,"Error occured while opening %s: %s\n", fileName, strerror(errno));
		exit(-1);
	}

	fstat(fd,&fileStat);
	char *m = mmap(0, fileStat.st_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
	printf("Size before: %d\n", (int)fileStat.st_size);
	if(id == 'd'){
		sprintf(m+strlen(m),"Super end\n");

		printf("Contents of file: %s",m);

	} else {
		if( lseek(fd, 100, SEEK_END)<0){
			perror("Could not seek");
			exit(-1);
		}
		if( write(fd,"hello",strlen("hello"))!= strlen("hello")){
			perror("Write error in e");
			exit(-1);
		}
		if( lseek(fd, 0, SEEK_SET)<0){
			perror("Could not seek");
			exit(-1);
		}
		while(read(fd,readBuff, 1024)>0){
			printf("%s",readBuff);
		}
	}
	fstat(fd,&fileStat);
	printf("Size after: %d\n", (int)fileStat.st_size);
}

void f(){
	int fd;
	struct stat fileStat;
	char* readBuff = (char*)malloc(1024);
	if ( (fd = open(fileName, O_RDWR, S_IREAD | S_IWRITE ) )== -1){
		fprintf(stderr,"Error occured while opening %s: %s\n", fileName, strerror(errno));
		exit(-1);
	}

	fstat(fd,&fileStat);
	char *m = mmap(0, 8192, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
	printf("This is on the first page: %c\n",m[3141]);
	printf("This is on the second page but won't work! SIGBUS galore!");
	printf("%c\n",m[6666]);
}

	
int main (int argc, char **argv){
	if(argc <2){
		fprintf(stderr, "Please enter more inputs\n");
		exit(-1);
	}
	int type = *argv[1];

	// struct stat fileStat;
	switch(type){
		case 'a':
			 abc(type, PROT_READ, MAP_SHARED );
			break;
		case 'b':
			abc(type, PROT_READ | PROT_WRITE, MAP_SHARED);
			break;
		case 'c':
			abc(type, PROT_READ  |PROT_WRITE, MAP_PRIVATE);
			break;
		case 'd':
			de(type);
			break;
		case 'e':
			de(type);
			break;
		case 'f':
			f();
			break;
		default:
			printf("Please enter a valid argument\n");
			break;
	}
	return 0;
}