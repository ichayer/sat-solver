// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

/* Standard library */
#define _GNU_SOURCE
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h> 
#include <sys/stat.h>  
#include <sys/mman.h>
#include <stdlib.h>
#include <semaphore.h>
#include <stdbool.h>

/* Local headers */
#include "../include/lib.h"
#include "../include/posShmADT.h"

typedef struct posShmCDT {
    const char * sem_name;     
    const char * shm_name;    
    sem_t * sem;                        
    int shmSize;                        
    char * shm_address;                 
    char * shm_current_address;         
    bool creator;                       
} posShmCDT;


static void freeShm(posShmADT shm){
    free(shm);
}

static void unlinkSem(posShmADT shm){
    if(-1 == sem_unlink(shm->sem_name)){
        perrorExit("Error unlinking semaphore");
    }
}

static void unlinkShm(posShmADT shm){

    if(-1 == shm_unlink(shm->shm_name)){
        perrorExit("Error unlinking shared memory");
    }
}

posShmADT newPosShmADT(const char * shm_name, const char * sem_name, int oflags, int mode, int shmSize, int prot) {

    posShmADT shm = calloc(1, sizeof(posShmCDT)); 
    
    if(NULL == shm) {
        perrorExit("Error initializing shared memory");
    }
    
    shm->sem_name = sem_name;
    shm->shm_name = shm_name;
    shm->shmSize = shmSize;

    int shm_fd = shm_open(shm_name, oflags, mode);
    if(-1 == shm_fd){
        freeShm(shm);
        perrorExit("Error opening shared memory");
    }

    if(oflags & O_CREAT){ 
        if(-1 == ftruncate(shm_fd, shm->shmSize)){
            unlinkShm(shm);
            freeShm(shm);
            perrorExit("Error in ftruncate function");
        }
    }

    shm->shm_address = mmap(NULL, shm->shmSize, prot, MAP_SHARED, shm_fd, 0);
    if(MAP_FAILED == shm->shm_address){
        unlinkShm(shm);
        freeShm(shm);
        perrorExit("Error in mmap function");
    }

    shm->shm_current_address = shm->shm_address;

    if(close(shm_fd)==-1){
        unlinkShm(shm);
        freeShm(shm);
        perrorExit("Error closing file descriptor");
    }

    if(oflags & O_CREAT){ 
        shm->sem = sem_open(sem_name, oflags, mode, 0);
        if(SEM_FAILED == shm->sem){
            unlinkShm(shm);
            freeShm(shm);
            perrorExit("Error creating semaphore");
        }
        shm->creator = true;

    } else { 
        shm->sem = sem_open(sem_name, 0);
        if(SEM_FAILED == shm->sem){
            unlinkShm(shm);
            freeShm(shm);
            perrorExit("Error opening semaphore");
        }
        shm->creator = false;
    }   

    return shm;
} 

void shmRead(posShmADT shm, char * buffer){
 
    if(-1 == sem_wait(shm->sem)){
        unlinkSem(shm);
        unlinkShm(shm);
        shmClose(shm);
        perrorExit("Error reading shared memory");
    }

    shm->shm_current_address += sprintf(buffer, "%s", shm->shm_current_address) + 1;
}

void shmWrite(posShmADT shm, char * buffer){    

    shm->shm_current_address += sprintf(shm->shm_current_address, "%s", buffer) + 1;

    if(-1 == sem_post(shm->sem)){
        shmClose(shm);
        perrorExit("Error writing shared memory");
    }
}

void shmClose(posShmADT shm){

    if(-1 == munmap(shm->shm_address, shm->shmSize)){
        unlinkSem(shm);
        unlinkShm(shm);
        freeShm(shm);
        perrorExit("Error unmapping address");
    }
    
    if(-1 == sem_close(shm->sem)){
        unlinkSem(shm);
        unlinkShm(shm);
        freeShm(shm);
        perrorExit("Error closing sem");
    }

    if(shm->creator){
        unlinkSem(shm);
        unlinkShm(shm);
    }

    freeShm(shm);
}


