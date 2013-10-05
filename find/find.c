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

extern int errno;


int main (int argc, char **argv){
int index;
int optVal;

int uflag = 0;
int mflag = 0;
opterr = 0;
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

 for(index = optind; index <argc; index++){
 	printf("Non-Option Parameters: %s\n", argv[index]);
 }


return 0;
}
