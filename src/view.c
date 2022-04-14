// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

/* Standard library */
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <stdlib.h>
#include <string.h>

/* Local headers */
#include "./include/posShmADT.h"
#include "./include/lib.h"

int main(int argc, char * argv[]){

    int error=0, filesQty=0;
    if(argc > 1){
        error = sscanf(argv[1], "%d", &filesQty);    // https://stackoverflow.com/questions/35397273/how-to-use-sscanf-with-argv
    } else {
        error = scanf("%d", &filesQty);
    }

    if(error < 0){
        perrorExit("Error obtaining files quantity");
    }

    int size = SLAVES_MAX_OUTPUT * filesQty;

    posShmADT shm = newPosShmADT(SHM_NAME, SEM_NAME, O_RDWR, 0, size, PROT_READ); 

    char buffer[SLAVES_MAX_OUTPUT];

    for(int i=0; i < filesQty; i++){
        shmRead(shm, buffer);
        printf("%s", buffer);
    }
    
    shmClose(shm);

    return 0;
}