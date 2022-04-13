#ifndef __POS_SHM_ADT_H__
#define __POS_SHM_ADT_H__

/* Constants */
#define SHM_NAME "myShm"
#define SEM_NAME "mySem"

typedef struct posShmCDT * posShmADT;

/** 
 * @brief Opens the semaphore and shared memory. Maps the shared memory into the current process
 *    
 * @param shm_name name to be given to the shared memory
 * @param sem_name name to be given to the semaphore
 * @param oflags bit mask that controls the operation of the call (O_CREAT, O_RDWR, O_RDONLY, etc)
 * @param mode permissions given to the file (S_IRUSR, S_IWUSR, etc)
 * @param shmSize shared memory size
 * @param prot permissions given to the mapping (PROT_READ, PROT_WRITE, etc)
 * 
 * @return posShmADT returns a handler of the shared memory and the synchronization between processes
 */
posShmADT newPosShmADT(const char * shm_name, const char * sem_name, int oflags, int mode, int shmSize, int prot);

/**
 * @brief Unmaps the shared memory
 *        Frees semaphore's resources
 *        Unlinks the shared memory and sempahore
 *        Frees the handler
 * 
 * @param shm a posShmADT previously created
 * @param master references the calling process: true -> master ; false -> view
 */
void shmClose(posShmADT shm);

/**
 * @brief Consecutive calls will be appended
 *        Writes into the shared memory
 *        
 * 
 * @param shm a posShmADT previously created
 * @param buffer string to be written into the shared memory
 */
void shmWrite(posShmADT shm, char * buffer);

/**
 * @brief Blocking function
 *        Reads from the shared memory the last read position
 * 
 * @param shm a posShmADT previously created
 * @param buffer buffer to retrieve string from the shared memory
 */
void shmRead(posShmADT shm, char * buffer);

#endif