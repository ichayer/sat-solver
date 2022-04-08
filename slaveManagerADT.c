#include <stdio.h>
#include <sys/types.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string.h>
#include "slaveManagerADT.h"
#include "myerror.h"


typedef struct slaveManagerCDT {
    char ** fileNames;
    int filesQty;
    int * slavesPids;   
    int slavesQty;
    int * fdread;      
    int * fdwrite;
    int slaveFiles; // qty of files that had been sended to the slaves
    int appFiles;   // qty of files that the been sended to the app
} slaveManagerCDT;


/*
    1. Como se le asigna un nuevo fileName al esclavo una vez que termino? El problema es como identificar que termino. 
       Le puedo asignar otro archivo para que analice cuando remuevo la informacion del pipe? O tengo que seguir dando tareas aunque no lea?
    2. #define _GNU_SOURCE para reconocer getline??
*/


slaveManagerADT newSlaveManager(char ** fileNames, int filesQty){
    if(filesQty < 1 || fileNames == NULL)
        perrorexit("newSlaveManager");
    

    slaveManagerADT sm = calloc(1, sizeof(slaveManagerCDT));
    if (sm == NULL)
        perrorexit("newSlaveManager");

    sm->fileNames = fileNames;
    sm->filesQty = filesQty;
    sm->slavesQty = filesQty/10 + 1;
    
    sm->slavesPids = malloc(sm->slavesQty * sizeof(int));
    if(sm->slavesPids == NULL){
        free(sm);
        perrorexit("newSlaveManager");
    }

    sm->fdread = malloc(sm->slavesQty * sizeof(int));
    if(sm->fdread == NULL){
        free(sm);
        free(sm->slavesPids);
        perrorexit("newSlaveManager");;
    }
    
    sm->fdwrite = malloc(sm->slavesQty * sizeof(int));
    if(sm->fdwrite == NULL){
        free(sm);
        free(sm->slavesPids);
        free(sm->fdread);
        perrorexit("newSlaveManager");;
    }

    sm->slaveFiles = 0;
    sm->appFiles = 0;

    return sm;
}


/*
** asocia fd[idxFrom] a fdTo
*/
void remapfd(int * fd, int idxFrom, int fdTo){
    if (fd[idxFrom] != fdTo){
        
        if (dup2(fd[idxFrom], fdTo) == -1)
            perrorexit("initializeSlaves");

        if (close(fd[idxFrom]) == -1)
            perrorexit("initializeSlaves");
    
    }
}

int initializeSlaves(slaveManagerADT sm){

    for (int i = 0; i < sm->slavesQty; ++i){

        //printf("Slave %d - %d\n", i, sm->slavesQty);
        int parentToChildren[2];
        int childrenToParent[2];

        if (pipe(parentToChildren) == -1)
            perrorexit("initializeSlaves");

        if (pipe(childrenToParent) == -1)
            perrorexit("initializeSlaves");

        switch (fork())
        {
        case -1:
            exit(EXIT_FAILURE);

        case 0:

            if (close(parentToChildren[WRITE]) == -1)
                perrorexit("initializeSlaves");

            if (close(childrenToParent[READ]) == -1)
                perrorexit("initializeSlaves");

            remapfd(parentToChildren, READ, STDIN_FILENO);
            remapfd(childrenToParent, WRITE, STDOUT_FILENO);

            execv("./slave", (char **){NULL});
            perrorexit("initializeSlaves");

        default:
            if (close(parentToChildren[READ]) == -1)
                perrorexit("initializeSlaves");

            if (close(childrenToParent[WRITE]) == -1)
                perrorexit("initializeSlaves");

            write(parentToChildren[WRITE], sm->fileNames[2*i], strlen(sm->fileNames[i]));
            write(parentToChildren[WRITE], "\n", 1);
            write(parentToChildren[WRITE], sm->fileNames[2*i + 1], strlen(sm->fileNames[i]));
            write(parentToChildren[WRITE], "\n", 1);

            //Pruebas
            close(parentToChildren[WRITE]);
            //sleep(2);
            //char mensaje[300];
            //read(childrenToParent[READ], mensaje, 299);
            //printf("%s", mensaje);
            // fin pruebas

            sm->fdwrite[i] = childrenToParent[WRITE];
            sm->fdread[i] = parentToChildren[READ];
            break;
        }
    }
    return sm->slavesQty;
}

int hasNextData(slaveManagerADT sm){
    return sm->appFiles < sm->filesQty;
}


int retriveData(slaveManagerADT sm, char * buffer, int bufferLimit){
    char * mockData = "File name: test/prueba, slave ID: 21303, Number of variables: 5, Number of clauses: 3, CPU time : 0 s, SATISFIABLE ";
    strncpy(buffer,mockData,bufferLimit-1);
    sm->appFiles++;
    return strlen(mockData);
}


void freeSlaveManager(slaveManagerADT sm){
    free(sm->fdread);
    free(sm->fdwrite);
    free(sm->slavesPids);
    free(sm);
}

void waitSlaves(slaveManagerADT sm){
    int state;
    for(int i=0 ; i<sm->slavesQty ; ++i){
        waitpid(sm->slavesPids[i], &state, 0);
    }
}