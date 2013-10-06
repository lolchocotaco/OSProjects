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
#include <pwd.h>
#include <time.h>
#include <grp.h>
extern int errno;

int uflag = 0;
int mflag = 0;
int uid = 0;
int mTime = 0;
time_t currentTime;
int powOne = 1;


void listFiles(char* dirName){
	DIR * dp; //Directory Pointer
	struct dirent *sp; //Directory Structure pointer
	char * d_name;
	struct stat fileStat; // Struture to hold file stats

	dp = opendir(dirName);
	if(dp != NULL){
		while(sp = readdir(dp)){
			d_name = sp->d_name;
			if(strcmp(sp->d_name, "..") != 0 && strcmp(sp->d_name,".") != 0){
				int path_length;
				char path[PATH_MAX];
				path_length = snprintf(path, PATH_MAX,"%s/%s",dirName,sp->d_name);
				// Displaying stats
				if(stat(path,&fileStat)>=0){
					if(uflag && fileStat.st_uid != uid)
						continue;
					if(mflag && powOne*(currentTime - fileStat.st_mtime) < mTime)
						continue;

					struct passwd * ownerInfo;
					struct group * groupInfo;
					char mTime[20];

					// Device and iNode Number
					printf("%ld/%lu\t",fileStat.st_dev,fileStat.st_ino);
					// Permissions (bitwise checks)
					printf( (S_ISDIR(fileStat.st_mode)) ? "d" : "-");
				    printf( (fileStat.st_mode & S_IRUSR) ? "r" : "-");
				    printf( (fileStat.st_mode & S_IWUSR) ? "w" : "-");
				    printf( (fileStat.st_mode & S_IXUSR) ? "x" : "-");
				    printf( (fileStat.st_mode & S_IRGRP) ? "r" : "-");
				    printf( (fileStat.st_mode & S_IWGRP) ? "w" : "-");
				    printf( (fileStat.st_mode & S_IXGRP) ? "x" : "-");
				    printf( (fileStat.st_mode & S_IROTH) ? "r" : "-");
				    printf( (fileStat.st_mode & S_IWOTH) ? "w" : "-");
				    printf( (fileStat.st_mode & S_IXOTH) ? "x" : "-");
				    //Number of links
				   	printf("\t%ld",fileStat.st_nlink);
				   	// Owner of file
				   	if( (ownerInfo = getpwuid(fileStat.st_uid) )!= NULL)
				   		printf("\t%s",ownerInfo->pw_name);
				   	else
				   		printf("\t%d",fileStat.st_uid);
				   	// Group of file
				   	if( (groupInfo = getgrgid(fileStat.st_gid) ) !=NULL)
				   		printf("\t%s",groupInfo->gr_name);
				   	else
				   		printf("\t%d",fileStat.st_gid);
				   	// Size of file
				   	if(sp->d_type == DT_BLK || sp->d_type == DT_CHR)
				   		printf("\t0x%x",(unsigned int)fileStat.st_size);
				   	else
				   		printf("\t%ld", fileStat.st_size);
				   	// Last Modified
				   	strftime(mTime,20,"%b %d %Y %H:%M",localtime(&fileStat.st_mtime));
				   	printf("\t%s",mTime);
				   	//File Name
				   	printf("\t%s\n",path);
				} else{ // Throw error and continue listing others
					fprintf(stderr,"Error with %s: %s",path,strerror(errno));
				}

				if(sp->d_type == DT_DIR){

					if(path_length >= PATH_MAX){
						fprintf(stderr,"Path Length is too long. \n");
						exit(1);
					}
					listFiles(path);
				}
			}			
		}
		if(closedir(dp)){
			fprintf(stderr,"Error closing '%s' : %s\n",dirName,strerror(errno));
			exit(1);
		}
	}else{
		fprintf(stderr,"Could not open directory '%s': %s\n", dirName, strerror(errno));
		exit(1);
	}

}



int main (int argc, char **argv){
	int index;
	int optVal;

	opterr = 0;
	char * dirName;
	struct passwd *userPtr;

	// Find the operators
	while ((optVal = getopt (argc, argv, "u:m:")) != -1) {
		switch (optVal) {
			case 'u':
			  uflag = 1;
			  if(isalpha(optarg[0])){
			  	if( (userPtr = getpwnam(optarg)) != NULL) {
			  		uid = userPtr->pw_uid;
			  	} else {
			  		fprintf(stderr,"Cannot find user: '%s'\n",optarg);
			  		exit(1);
			  	}
			  }
			  else if(isdigit(optarg[0])){
			  	uid = atoi(optarg);
			  } else{
			  	fprintf(stderr, "Input a valid UID/Username\n");
			  	exit(1);
			  }
			  break;
			case 'm':
			  mflag = 1;
			  if(isdigit(optarg[0]) || optarg[0] == '-' ){
			  	mTime = atoi(optarg);
			  	if(mTime <0)
			  		powOne = -1;
			  	else
			  		powOne = 1;
			  	time(&currentTime);
			  } else{
			  	fprintf(stderr,"Option 'm' requres numeric input\n");
			  	exit(1);
			  }
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
