#ifndef __FUNCTIONS__
#define __FUNCTIONS__

#include "header.h"
#include "constants.h"

char *attachSharedMemory(int shmid)
{
    char *sharedMemory;

    // Attach to the shared memory
    sharedMemory = shmat(shmid, NULL, 0);
    if (sharedMemory == (char *)-1)
    {
        perror("shmat");
        exit(1);
    }

    return sharedMemory;
}

void detachSharedMemory(char *sharedMemory)
{
    // Detach from the shared memory
    if (shmdt(sharedMemory) == -1)
    {
        perror("shmdt");
        exit(1);
    }
}

void deleteSharedMemory(int shmid)
{
    // Delete the shared memory segment
    if (shmctl(shmid, IPC_RMID, NULL) == -1)
    {
        perror("shmctl -- delete");
        exit(1);
    }
}

int open_shmem()
{
    // Generate a unique key for the shared memory segment
    key_t sh_key = ftok(".", MEM_SEED);

    int shmid;
    // Attempt to open an existing shared memory segment or create a new one
    if ((shmid = shmget(sh_key, 0, 0)) == -1)
    {
        perror("Error opening shared memory from Parent");
        exit(-2);
    }
    // Return the shared memory identifier
    return shmid;
}

int open_sem()
{
    // Generate a unique key for the semaphore
    key_t semKey = ftok(".", SEM_SEED);

    int semid;
    // Attempt to open an existing semaphore or create a new one
    semid = semget(semKey, 1, IPC_CREAT | 0666);
    if (semid == -1)
    {
        perror("semget -- sim_system");
        exit(-1);
    }
    printf("Semaphore is opened\n");
    // Return the semaphore identifier
    return semid;
}

void lock(int semid)
{
    // Define a semaphore operation to acquire a lock
    struct sembuf acquire = {0, -1, SEM_UNDO};
    // Attempt to perform the semaphore operation to acquire the lock
    if (semop(semid, &acquire, 1) == -1)
    {
        perror("semop -- lock");
        exit(1);
    }
}

void unlock(int semid)
{
    // Define a semaphore operation to release the lock
    struct sembuf release = {0, 1, SEM_UNDO};
    // Attempt to perform the semaphore operation to release the lock
    if (semop(semid, &release, 1) == -1)
    {
        perror("semop -- unlock");
        exit(1);
    }
}

// remove the semaphore
void removeSemaphore(int seed)
{
    key_t semKey = ftok(".", seed);

    int semid = semget(semKey, 1, 0666);
    if (semid == -1)
    {
        perror("semget -- remove");
        exit(-1);
    }

    if (semctl(semid, 0, IPC_RMID, 0) == -1)
    {
        perror("semctl -- remove");
        exit(1);
    }

    printf("Semaphore is removed\n");
}

// set the text color to green
void green()
{
    printf("\033[32;1m");
    fflush(stdout);
}

// set the text color to red
void red()
{
    printf("\033[31;1m");
    fflush(stdout);
}

void blue()
{
    printf("\033[34;1m");
    fflush(stdout);
}

// set the text color to purple
void purple()
{
    printf("\033[35m");
    fflush(stdout);
}

// set the text color to yellow
void yellow()
{
    printf("\033[33;1m");
    fflush(stdout);
}

// reset the text attributes
void resetColor()
{
    printf("\033[0m");
    fflush(stdout);
}

#endif