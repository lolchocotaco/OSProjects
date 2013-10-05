#include<stdio.h>
#include<errno.h>

int main( int argc, char *argv[]){
    if(argc < 2){
        printf("Please input a file name!\n");
    }
    else if( argv[1] == "-b"){
        printf("There is a buffer size");
    }
    else if( argv[1] == "-o"){
        printf("There is an output file");
    }
    else{
        FILE *file = fopen(argv[1], "r");
        if (file == 0){
            printf("Couldn't open file!\n");
            printf("__%s__\n", argv[1]);
        }
        else{
           int x;
          while( (x=fgetc(file)) != EOF){
             printf( "%c",x);
          }
         fclose(file); 
        }
    }


}
