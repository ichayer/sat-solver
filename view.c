/* Standard library */
#define _GNU_SOURCE
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <stdlib.h>

/* Local headers */
#include "posShmADT.h"

// TO DO: remove this constant
#define SIZE 81920


int
main(int argc, char * argv[]){

    posShmADT shm;

    //setvbuf(stdin, NULL, _IONBF, 0);

    if(3 != argc){

        size_t lineCap = 0;
        char *shm_name = NULL;
        char *sem_name = NULL;

        if(getline(&shm_name, &lineCap, stdin) <= 0){
            perror("Getline function");
            exit(1);
        }

        if(getline(&sem_name, &lineCap, stdin) <= 0){
            free(shm_name);
            perror("Getline function");
            exit(1);
        }

        // TO DO: Check how to obtain size.
        shm = newPosShmADT(shm_name, sem_name, O_RDWR, 0, SIZE, PROT_READ); 

        free(shm_name);
        free(sem_name);

    } else {

        // TO DO: Check how to obtain size.
        shm = newPosShmADT(argv[1], argv[2], O_RDWR, 0, SIZE, PROT_READ);
    }

    char buff[MAX_BUFFER];

    // TO DO: How do we obtain the total quantity of files we are going to process? 
    // Is there any way to avoid using stdin?

    // TO DO: remove i<20, its just for testing
    for(int i=0; i < 20; i++){
        shmRead(shm, buff);
        printf("%s\n", buff);
    }
    
    shmClose(shm);

    return 0;
}