#ifndef __FUNCTIONS__
#define __FUNCTIONS__

#include "header.h"
#include "constants.h"

int open_shmem(){

    key_t sh_key = ftok(".", MEM_SEED);
    int shmid;
	if((shmid=shmget(sh_key,0,0))==-1){
		perror("Error opening shared memory from Parent");
		exit(-2);
	}
	return shmid;
}

int open_sem(){
    key_t semKey = ftok(".", SEM_SEED);

    int semid = semget(semKey, 1, IPC_CREAT | 0666);
    if (semid == -1)
    {
        perror("semget -- sim_system");
        exit(-1);
    }
    printf("semaphore is opend\n");
    return semid;

}

void lock(int semid)
{
    struct sembuf acquire = {0, -1, SEM_UNDO};
    if (semop(semid, &acquire, 1) == -1) {
        perror("semop -- lock");
        exit(1);
    }
}
void unlock(int semid)
{
    struct sembuf release = {0, 1, SEM_UNDO};
    if (semop(semid, &release, 1) == -1) {
        perror("semop -- unlock");
        exit(1);
    }
}

// set the text color to green
void green() {
    printf("\033[32;1m");
    fflush(stdout);
}

// set the text color to red
void red() {
    printf("\033[31;1m");
    fflush(stdout);
}

void blue() {
    printf("\033[34;1m");
    fflush(stdout);
}

// set the text color to purple
void purple() {
    printf("\033[35m");
    fflush(stdout);
}

// set the text color to yellow
void yellow() {
    printf("\033[33;1m");
    fflush(stdout);
}

// reset the text attributes
void resetColor() {
    printf("\033[0m");
    fflush(stdout);
}


#endif