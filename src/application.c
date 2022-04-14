// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

/* Standard library */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>

/* Local headers */
#include "./include/slaveManagerADT.h"
#include "./include/posShmADT.h"
#include "./include/lib.h"

/* Constants */
#define VIEW_PROCES_WAITTIME 2

int main(int argc, char *argv[]){

    setvbuf(stdout, NULL, _IONBF, 0);

    if(1 == argc){
        perrorExit("You must input at least one file");
    }

    int filesQty = argc - 1;
    int size = SLAVES_MAX_OUTPUT * filesQty;
    char ** files = &argv[1];

    posShmADT shm = newPosShmADT(SHM_NAME, SEM_NAME, O_CREAT | O_RDWR, S_IRUSR | S_IWUSR, size, PROT_WRITE);

    printf("%d\n", filesQty);

    slaveManagerADT sm = newSlaveManager(files, filesQty);

    initializeSlaves(sm);

    sleep(VIEW_PROCES_WAITTIME);
    
    char buffer[SLAVES_MAX_OUTPUT];

    int fdOutputFile = open(OUTPUT_FILE_NAME, O_CREAT|O_WRONLY|O_TRUNC, WR_ALL);
    if(fdOutputFile == -1)
        perrorExit("Error opening output file");

    int readData;

    while(hasNextData(sm)){
        readData = retriveData(sm, buffer, SLAVES_MAX_OUTPUT);
        write(fdOutputFile, buffer, readData);
        shmWrite(shm, buffer);
    } 

    freeSlaveManager(sm);   
    shmClose(shm);   
    close(fdOutputFile);
    
    return 0;
}