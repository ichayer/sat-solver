/* Standard library */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>

/* Local headers */
#include "slaveManagerADT.h"
#include "posShmADT.h"

/* Constants */
#define VIEW_PROCES_WAITTIME 2
#define SHM_NAME "shm_name"
#define SEM_NAME "sem_name"


// TO DO: remove this constant
#define SIZE 81920

int
main(int argc, char *argv[]){

    setvbuf(stdout, NULL, _IONBF, 0);

    if(1 == argc){
        perror("You must input at least one file\n");
        exit(1);
    }

    sleep(VIEW_PROCES_WAITTIME);

    char * output[] = {"test/prueba", "test/prueba", "test/prueba", "test/prueba", "test/prueba", "test/prueba", "test/prueba", "test/prueba", "test/prueba", "test/prueba", "test/prueba", "test/prueba","test/prueba", "test/prueba", "test/prueba", "test/prueba", "test/prueba", "test/prueba","test/prueba", "test/prueba", "test/prueba", "test/prueba", "test/prueba", "test/prueba"};
    slaveManagerADT sm = newSlaveManager(output, 20);

    initializeSlaves(sm);

    // TO DO: remove SIZE constant
    posShmADT shm = newPosShmADT(SHM_NAME, SEM_NAME, O_CREAT | O_RDWR, S_IRUSR | S_IWUSR, SIZE, PROT_WRITE);

    puts(SHM_NAME);
    puts(SEM_NAME);
    fflush(stdout);
    
    // TO DO: remove testing variables
    char buffer[MAX_BUFFER];

    while(hasNextData(sm)){
        retriveData(sm, buffer, MAX_BUFFER);
        shmWrite(shm, buffer);
    }   

    freeSlaveManager(sm);   

    shmClose(shm);   

    return 0;
}