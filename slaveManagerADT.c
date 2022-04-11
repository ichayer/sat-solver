#include "slaveManagerADT.h"
#include "myerror.h"


typedef struct slaveManagerCDT {
    char ** fileNames;
    int filesQty;
    
    int * slavesPids;   
    int slavesQty;
    int * fdread;      
    int * fdwrite;
    
    int maxfd;
    fd_set rfds;
    int avilableData;       // rfds with avilable data
    int lastIndexRetrived;  // index of the last fd to read data from
    
    int slaveFiles; // qty of files that had been sended to the slaves
    int appFiles;   // qty of files that the been sended to the app
} slaveManagerCDT;


/*
    1. Como se le asigna un nuevo fileName al esclavo una vez que termino? El problema es como identificar que termino. 
       Le puedo asignar otro archivo para que analice cuando remuevo la informacion del pipe? O tengo que seguir dando tareas aunque no lea?
    2. #define _GNU_SOURCE para reconocer getline??
    3. Es necesario hacer un wait de los esclavos?
    4. Chequear errores
    5. Puedo definir una maxima cantidad de esclavos?
    6. Esta bien liberar los esclavos cuando hacemos el free del adt?
*/

void deliverFile(slaveManagerADT sm, int slaveIdx);
void remapfd(int * fd, int idxFrom, int fdTo);
int hasNextData(slaveManagerADT sm);
int getIdxOfFdWithData(slaveManagerADT sm);
int maxValue(int * vec, int qty, fd_set * rfds);
void setFds(int * vec, int qty, fd_set * rfds);

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

    FD_ZERO(&(sm->rfds));

    sm->avilableData = 0;
    sm->slaveFiles = 0;
    sm->appFiles = 0;

    return sm;
}

int initializeSlaves(slaveManagerADT sm){

    for (int i = 0; i < sm->slavesQty; ++i){

        int parentToChildren[2];
        int childrenToParent[2];

        if (pipe(parentToChildren) == -1)
            perrorexit("initializeSlaves");

        if (pipe(childrenToParent) == -1)
            perrorexit("initializeSlaves");

        int pid = fork();

        switch (pid)
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

            sm->fdread[i] = childrenToParent[READ];
            sm->fdwrite[i] = parentToChildren[WRITE];
            sm->slavesPids[i] = pid;

            deliverFile(sm, i);
            deliverFile(sm, i);

            break;
        }
    }

    sm->maxfd = maxValue(sm->fdread, sm->slavesQty, &sm->rfds);
    return sm->slavesQty;
}

int retriveData(slaveManagerADT sm, char * buffer, int bufferLimit){
    
    if(sm->avilableData <= 0){
        setFds(sm->fdread, sm->slavesQty, &sm->rfds);
        if((sm->avilableData = select(sm->maxfd + 1, &sm->rfds, NULL, NULL, NULL))==-1){
            printf("%d\n", sm->avilableData);
            perrorexit("select");
        }
        sm->lastIndexRetrived = -1;
    }

    sm->avilableData-=1;
    int idx = getIdxOfFdWithData(sm);   
    sm->appFiles++;

    int i=0;
    int c;

    while((read(sm->fdread[idx], &c, 1))>0 && c!='>' && c!='\0' && i<(bufferLimit-2)){
        buffer[i++] = c;
    }
    buffer[i++] = '>';
    buffer[i] = 0;

    if(sm->slaveFiles < sm->filesQty)
        deliverFile(sm, idx);

    return i;
}


void freeSlaveManager(slaveManagerADT sm){
    
    for(int i=0 ; i<sm->slavesQty ; ++i){
        close(sm->fdwrite[i]);
        //close(sm->fdread[i]);
    }
    
    free(sm->fdread);
    free(sm->fdwrite);
    free(sm->slavesPids);
    free(sm);
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

int hasNextData(slaveManagerADT sm){
    return sm->appFiles < sm->filesQty;
}

int getIdxOfFdWithData(slaveManagerADT sm){
    for(int i=sm->lastIndexRetrived+1; i<sm->slavesQty ; ++i){
        if(FD_ISSET(sm->fdread[i],&sm->rfds)){
            return sm->lastIndexRetrived = i;
        }
    }
    return -1;
}

void deliverFile(slaveManagerADT sm, int slaveIdx){
    if(sm->slaveFiles < sm->filesQty){
        write(sm->fdwrite[slaveIdx], sm->fileNames[sm->slaveFiles], strlen(sm->fileNames[sm->slaveFiles]));
        write(sm->fdwrite[slaveIdx], "\n", 1);
        sm->slaveFiles++;
    }
}

int maxValue(int * vec, int qty, fd_set * rfds){    
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