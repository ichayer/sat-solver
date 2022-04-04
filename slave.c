#define _GNU_SOURCE
#include <stdio.h>
#include <sys/types.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <time.h>

int
main(int argc, char *argv[]){

    char * line = NULL;
    size_t linecap = 0;
    ssize_t linelen;
   
    while ((linelen = getline(&line, &linecap, stdin)) > 0)
    {
        clock_t t = clock();


        //printf("%s:", line);
        int fd = open(line,O_RDWR);
        char leer[50];
        read(fd,leer, 4);
        leer[4] = 0;
        //printf("%s \n", leer);


        t = clock() - t;
        double time_taken = ((double)t)/CLOCKS_PER_SEC;
        printf("Nombre del archivo: %s, ID del esclavo: %d. Tiempo de procesamiento: %f \n", line, getpid(), time_taken);        
    }
    
    free(line);
    return 0;
}