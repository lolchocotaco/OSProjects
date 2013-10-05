////////////////////
//Sameer Chauhan  //
//ECE357          //
//Find Program //
////////////////////  

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>         /* defines options flags */
#include <sys/types.h>     /* defines types used by sys/stat.h */
#include <sys/stat.h>      /* defines S_IREAD & S_IWRITE  */
#include <dirent.h>
#include <limits.h> 		/*defints PATHMAX */
extern int errno;


void listFiles(char* dirName){
	DIR * dp; //Directory Pointer
	struct dirent *sp; //Directory Structure pointer
	char * d_name;

	dp = opendir(dirName);
	if(dp != NULL){
		while(sp = readdir(dp)){
			d_name = sp->d_name;
			printf("%s/%s\n",dirName,sp->d_name);
			if(sp->d_type == DT_DIR && strcmp(sp->d_name, "..") != 0 && strcmp(sp->d_name,".") != 0){
				int path_length;
				char path[PATH_MAX];
				path_length = snprintf(path, PATH_MAX,"%s/%s",dirName,d_name);
				if(path_length >= PATH_MAX){
					fprintf(stderr,"Path Legnth is too long. \n");
					exit(1);
				}
				listFiles(path);
			}
		}
		(void)closedir(dp);
	}else{
		fprintf(stderr,"Could not open directory '%s': %s\n", dirName, strerror(errno));
		exit(1);
	}

}


int main (int argc, char **argv){
	int index;
	int optVal;
	int uflag = 0;
	int mflag = 0;
	opterr = 0;
	char * dirName;
	

	// Find the operators
	while ((optVal = getopt (argc, argv, "u:m:")) != -1) {
	switch (optVal)
	 {
	 case 'u':
	   uflag = 1;
	   // outName = optarg;
	   break;
	 case 'm':
	   mflag = 1;
	   // bufferSize = atoi(optarg); // Save the buffer size
	   break;
	 case '?':
	   if (optopt == 'u' || optopt =='m')
	     fprintf (stderr, "Option -%c requires a additional arguments.\n", optopt);
	   else
	     fprintf (stderr,  "Unknown option `-%c'.\n", optopt);
	   return -1;
	 default:
	   abort ();
	 }
	}



	if(optind == argc){
		dirName = ".";
	} else{
		dirName = argv[optind];
	}

	listFiles(dirName);

	return 0;
}
