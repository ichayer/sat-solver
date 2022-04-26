// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

/* Local headers */
#include "../include/slaveManagerADT.h"

typedef struct slaveManagerCDT {
    char ** fileNames;
    int filesQty;
    
    int * slavesPids;   
    int slavesQty;
    int * fdread;      
    int * fdwrite;
    
    int maxfd;
    fd_set rfds;
    int availableData;       // rfds with available data
    int lastIndexRetrived;  // index of the last fd to read data from
    
    int * pendingFiles; // files that each slave has pending to process
    int slaveFiles;     // qty of files that had been sended to the slaves
    int appFiles;       // qty of files that had been sended to the app
} slaveManagerCDT;

/** 
 * Delivers a file to the slave of index slaveIdx, just if there are available
 * files to be sended, otherwise, it does nothing. It also increments the number
 * of sended files. 
 */
static void deliverFile(slaveManagerADT sm, int slaveIdx);

/**
 * Searches the next file descriptor with available data in the fdread vector from the
 * shared memory using the fd_set rfds stored in the struct sm.
 */
static int getIdxOfFdWithData(slaveManagerADT sm);

/**
 * Frees memory to exit.
 */
static void safeSlaveManagerExit(slaveManagerADT sm, char * msg);

slaveManagerADT newSlaveManager(char ** fileNames, int filesQty){

    if(filesQty < 1 || NULL == fileNames)
        perrorExit("Invalid parameters for newSlaveManager");
    
    slaveManagerADT sm = calloc(1, sizeof(slaveManagerCDT));
    if (NULL == sm)
        perrorExit("Error initializing newSlaveManager");

    sm->fileNames = fileNames;
    sm->filesQty = filesQty;
    sm->slavesQty = filesQty/10 + 1;
    FD_ZERO(&(sm->rfds));
    sm->availableData = 0;
    sm->slaveFiles = 0;
    sm->appFiles = 0;
    
    sm->slavesPids = malloc(sm->slavesQty * sizeof(int));
    if(NULL == sm->slavesPids){
        free(sm);
        perrorExit("Error in malloc function");
    }

    sm->fdread = malloc(sm->slavesQty * sizeof(int));
    if(NULL == sm->fdread){
        free(sm->slavesPids);
        free(sm);
        perrorExit("Error in malloc function");
    }
    
    sm->fdwrite = malloc(sm->slavesQty * sizeof(int));
    if(NULL == sm->fdwrite){
        free(sm->slavesPids);
        free(sm->fdread);
        free(sm);
        perrorExit("Error in malloc function");
    }

    sm->pendingFiles = calloc(sm->slavesQty, sizeof(int));
    if(NULL == sm->pendingFiles){
        free(sm->slavesPids);
        free(sm->fdread);
        free(sm->fdwrite);
        free(sm);
        perrorExit("Error in malloc function");
    }
    return sm;
}

int initializeSlaves(slaveManagerADT sm){

    if(NULL == sm)
        perrorExit("Invalid parameters for initializeSlaves");

    for (int i = 0; i < sm->slavesQty; ++i){

        int parentToChildren[2];
        int childrenToParent[2];

        if (-1 == pipe(parentToChildren) || -1 == pipe(childrenToParent))
            safeSlaveManagerExit(sm, "Error creating pipe");

        int pid = fork();

        switch (pid) 
        {
        case -1:
            safeSlaveManagerExit(sm, "Error in fork");

        case 0:

            if (-1 == close(parentToChildren[WRITE]) || -1 == close(childrenToParent[READ]))
                safeSlaveManagerExit(sm, "Error closing pipe");
            
            // Close pipes that comunicate with previous slaves
            for(int j=0 ; j<i ; ++j){
                if (-1 == close(sm->fdread[j])|| -1 == close(sm->fdwrite[j]))
                    safeSlaveManagerExit(sm, "Error closing pipe");
            }

            remapfd(parentToChildren, READ, STDIN_FILENO);
            remapfd(childrenToParent, WRITE, STDOUT_FILENO);

            execv("./slave", (char **){NULL});
            safeSlaveManagerExit(sm, "Error with execv while trying to open ./slave");
            
            break;
            
        default:
            if (-1 == close(parentToChildren[READ]) || -1 == close(childrenToParent[WRITE]))
                safeSlaveManagerExit(sm, "Error closing pipe");

            sm->fdread[i] = childrenToParent[READ];
            sm->fdwrite[i] = parentToChildren[WRITE];
            sm->slavesPids[i] = pid;

            //Each slave starts with two files
            deliverFile(sm, i);
            deliverFile(sm, i);

            break;
        }
    }

    sm->maxfd = maxValue(sm->fdread, sm->slavesQty);
    return sm->slavesQty;
}

int retriveData(slaveManagerADT sm, char * buffer, int bufferLimit){

    if(NULL == sm || NULL == buffer || bufferLimit < 0)
        perrorExit("Invalid parameters for retriveData");
    
    // retriveData returns the output of a file each time it is called.
    // In sm->availableData is stored how many files had available data
    // when select was called, so that no to call it unnecessary if it is
    // already known that there was available data.
    if(sm->availableData <= 0){
        setFds(sm->fdread, sm->slavesQty, &sm->rfds);

        if(-1 == (sm->availableData = select(sm->maxfd + 1, &sm->rfds, NULL, NULL, NULL)))
            safeSlaveManagerExit(sm, "Error in select");
        
        sm->lastIndexRetrived = -1;
    }

    int idx = getIdxOfFdWithData(sm);
    sm->availableData-=1;   
    sm->appFiles++;

    int i=0;
    char c;

    while((read(sm->fdread[idx], &c, 1))>0 && c!='\n' && c!='\0' && i<(bufferLimit-2))
        buffer[i++] = c;
    
    buffer[i++] = '\n';
    buffer[i] = 0;

    sm->pendingFiles[idx]--;
    if((sm->slaveFiles < sm->filesQty) && (!sm->pendingFiles[idx]))
        deliverFile(sm, idx);  
    
    return i;
}

int hasNextData(slaveManagerADT sm){
    
    if(NULL == sm)
        perrorExit("Invalid parameters for hasNextData");

    return sm->appFiles < sm->filesQty;
}

void freeSlaveManager(slaveManagerADT sm){

    if(NULL == sm)
        perrorExit("Invalid parameters for freeSlaveManager");
    
    for(int i=0 ; i<sm->slavesQty ; ++i){
        close(sm->fdwrite[i]);
        close(sm->fdread[i]);
    }
    
    free(sm->pendingFiles);
    free(sm->fdread);
    free(sm->fdwrite);
    free(sm->slavesPids);
    free(sm);
}

static int getIdxOfFdWithData(slaveManagerADT sm){

    if(NULL == sm)
        perrorExit("Invalid parameters for getIdxOfFdWithData");
        
    for(int i=sm->lastIndexRetrived+1; i<sm->slavesQty ; ++i){
        if(FD_ISSET(sm->fdread[i],&sm->rfds))
            return sm->lastIndexRetrived = i;
        
    }
    return -1;
}

static void deliverFile(slaveManagerADT sm, int slaveIdx){

    if(NULL == sm || slaveIdx < 0)
        perrorExit("Invalid parameters for deliverFile");

    if(sm->slaveFiles < sm->filesQty){
        write(sm->fdwrite[slaveIdx], sm->fileNames[sm->slaveFiles], strlen(sm->fileNames[sm->slaveFiles]));
        write(sm->fdwrite[slaveIdx], "\n", 1);
        sm->slaveFiles++;
        sm->pendingFiles[slaveIdx]++;
    }
}

static void safeSlaveManagerExit(slaveManagerADT sm, char * msg){

    if(NULL == sm)
        perrorExit("Invalid parameters for safeSlaveManagerExit");

    freeSlaveManager(sm);
    perrorExit(msg);
}