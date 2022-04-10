/* Defines */
#define _GNU_SOURCE

/* Standard library */
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h> 
#include <sys/stat.h>  
#include <sys/mman.h>
#include <stdlib.h>
#include <semaphore.h>

/* Local headers */
#include "posShmADT.h"

typedef struct posShmCDT {
    char sem_name[MAX_NAME_LENGTH];     // semaphore name
    char shm_name[MAX_NAME_LENGTH];     // shared memory name
    sem_t * sem;                        // semaphore
    int shmSize;                        // shared memory size
    char * shm_address;                 // shared memory pointer (must be preserved to munmap)
    char * shm_current_address;         // dynamic pointer
    bool creator;                       
} posShmCDT;


static void freeShm(posShmADT shm){

    free(shm);
}

static void unlinkSem(posShmADT shm){

    if(-1 == sem_unlink(shm->sem_name)){
        perror("Error unlinking semaphore\n");
        exit(1);
    }
}

static void unlinkShm(posShmADT shm){

    if(-1 == shm_unlink(shm->shm_name)){
        perror("Error unlinking shared memory\n");
        exit(1);
    }
}

posShmADT newPosShmADT(const char * shm_name, const char * sem_name, int oflags, int mode, int shmSize, int prot) {

    posShmADT shm = calloc(1, sizeof(posShmCDT)); 
    shm->shmSize = shmSize;
    
    if(NULL == shm) {
        perror("Error initializing shared memory\n");
        exit(1);
    }
    
    strcpy(shm->sem_name, sem_name);
    strcpy(shm->shm_name, shm_name);

    int shm_fd = shm_open(shm_name, oflags, mode);
    if(-1 == shm_fd){
        freeShm(shm);
        perror("Error opening shared memory\n");
        exit(1);
    }

    if(oflags & O_CREAT){ 
        if(-1 == ftruncate(shm_fd, shm->shmSize)){
            unlinkShm(shm);
            freeShm(shm);
            perror("Error in ftruncate()\n");
            exit(1);
        }
    }


    shm->shm_address = mmap(NULL, shm->shmSize, prot, MAP_SHARED, shm_fd, 0);
    if(MAP_FAILED == shm->shm_address){
        unlinkShm(shm);
        freeShm(shm);
        perror("Error in mmap()\n");
        exit(1);
    }

    shm->shm_current_address = shm->shm_address;

    if(close(shm_fd)==-1){
        unlinkShm(shm);
        freeShm(shm);
        perror("Error closing fd\n");
        exit(1);
    }

    if(oflags & O_CREAT){ 
        shm->sem = sem_open(sem_name, oflags, mode, 0);
        if(SEM_FAILED == shm->sem){
            unlinkShm(shm);
            freeShm(shm);
            perror("Error creating semaphore\n");
            exit(1);
        }
        shm->creator = true;

    } else { 
        shm->sem = sem_open(sem_name, 0);
        if(SEM_FAILED == shm->sem){
            unlinkShm(shm);
            freeShm(shm);
            perror("Error opening semaphore\n");
            exit(1);
        }
        shm->creator = false;
    }   

    return shm;
} 

void shmRead(posShmADT shm, char * buff){
 
    if(-1 == sem_wait(shm->sem)){
        perror("Error reading shared memory\n");
        unlinkSem(shm);
        unlinkShm(shm);
        shmClose(shm);
        exit(1);
    }

    shm->shm_current_address += sprintf(buff, "%s", shm->shm_current_address) + 1;
}

void shmWrite(posShmADT shm, char * buff){    

    shm->shm_current_address += sprintf(shm->shm_current_address, "%s", buff) + 1;

    if(-1 == sem_post(shm->sem)){
        perror("Error writing shared memory\n");
        shmClose(shm);
        exit(1);
    }
}

void shmClose(posShmADT shm){

    if(-1 == munmap(shm->shm_address, shm->shmSize)){
        perror("Error unmapping address\n");
        unlinkSem(shm);
        unlinkShm(shm);
        freeShm(shm);
        exit(1);
    }
    
    if(-1 == sem_close(shm->sem)){
        unlinkSem(shm);
        unlinkShm(shm);
        freeShm(shm);
        perror("Error closing sem\n");
        exit(1);
    }

    if(shm->creator){
        unlinkSem(shm);
        unlinkShm(shm);
    }

    freeShm(shm);
}


