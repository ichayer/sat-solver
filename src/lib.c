// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

/* Standard library */
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>

/* Local headers */
#include "./include/lib.h"

void perrorExit(char * message){
    perror(message);
    exit(EXIT_FAILURE);
}

void remapfd(int * fd, int idxFrom, int fdTo){
    if (fd[idxFrom] != fdTo){
        
        if (dup2(fd[idxFrom], fdTo) == -1)
            perrorExit("Error in dup2 function");

        if (close(fd[idxFrom]) == -1)
            perrorExit("Error closing file descriptor");
    
    }
}

int maxValue(int * vec, int qty){    
    int max = vec[0];
    for(int i=1 ; i<qty ; ++i){
        if(max < vec[i])
            max = vec[i];
    }
    return max;
}

void setFds(int * vec, int qty, fd_set * rfds){
    for(int i=0 ; i<qty ; ++i){
        FD_SET(vec[i], rfds);
    }
}