#include <stdlib.h>
#include <stdio.h>
#include "slaveManagerADT.h"

typedef struct slaveManagerCDT {
    char ** fileNames;
    int filesQty;
    int * slavesPids;   // 541   542   543
    int slavesQty;
    int * fdread;       // 5     6     7
    int * fdwrite;
} slaveManagerCDT;


/*
    1. Como se le asigna un nuevo fileName al esclavo una vez que termino? El problema es como identificar que termino.
    2. Una vez que el esclavo escribio en el fd del pipe, como hacemos para que esa informacion llegue a la aplicacion?
*/


slaveManagerADT newSlaveManager(char ** fileNames, int slavesQty, int filesQty){
    if(slavesQty< 1 || filesQty < 1 || fileNames == NULL){
        //Ver manejo de errores
        return NULL;
    }

    slaveManagerADT sm = calloc(1, sizeof(slaveManagerCDT));
    if (sm == NULL)
        return NULL;

    sm->fileNames = fileNames;
    sm->filesQty = filesQty;
    sm->slavesQty = filesQty > slavesQty ? slavesQty : filesQty;
    sm->slavesPids = malloc(sm->slavesQty * sizeof(int));

    if(sm->slavesPids == NULL){
        free(sm);
        return NULL;
    }

    return sm;
}

int initializeSlaves(slaveManagerADT sm){

    for(int i=0 ; i < sm->slavesQty ; ++i){
        
        pid_t pid = fork();

        switch (pid)
        {
        case -1:
            _exit(EXIT_FAILURE);    
        case 0:
            printf("Child %d, pid %d sleeping... \n", i, getpid());
            sleep(5);
            // aca deberiamos llamar a los esclavos
            printf("Child %d woke up! \n", i);
            _exit(EXIT_SUCCESS);
        default:
            sm->slavesPids[i] = pid;
            printf("father %d \n", i);
            break;
        }
    }
    return sm->slavesQty;
}

int hasNextData(){
    //Devuelve 0 si hay data 1 si no

}

void waitSlaves(slaveManagerADT sm){
    int state;
    for(int i=0 ; i<sm->slavesQty ; ++i){
        waitpid(sm->slavesPids[i], &state, 0);
    }
}