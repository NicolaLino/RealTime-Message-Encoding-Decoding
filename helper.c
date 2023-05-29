#include "header.h"
#include "functions.h"
#include "constants.h"

void waitSemaphore(int semid, int sem_num) {
    struct sembuf wait_operation;
    wait_operation.sem_num = sem_num;
    wait_operation.sem_op = -1;
    wait_operation.sem_flg = 0;

    if (semop(semid, &wait_operation, 1) == -1) {
        perror("semop wait");
        exit(1);
    }
}

void signalSemaphore(int semid, int sem_num) {
    struct sembuf signal_operation;
    signal_operation.sem_num = sem_num;
    signal_operation.sem_op = 1;
    signal_operation.sem_flg = 0;

    if (semop(semid, &signal_operation, 1) == -1) {
        perror("semop signal");
        exit(1);
    }
}

void swapMessages(char* sharedMemory, int columnNumber) {
    int location1 = rand() % columnNumber;
    int location2 = rand() % columnNumber;

    char temp[100];
    strcpy(temp, &sharedMemory[location1 * 100]);
    strcpy(&sharedMemory[location1 * 100], &sharedMemory[location2 * 100]);
    strcpy(&sharedMemory[location2 * 100], temp);
}

int main(int argc, char **argv)
{
    int columnNumber = atoi(argv[1]);
    int shmKey = atoi(argv[2]);


    srand(time(NULL));

    int shmid = open_shmem();
    int semid = open_sem();


    
    int count = 0;
    while (count < 10) {
        count++;
        int sleepTime = rand() % 5 + 1;
        sleep(sleepTime);
        lock(semid);        
        printf("Helper %d is running\n", getpid());
        fflush(stdout);
         char *sharedMemory = attachSharedMemory(shmid);
        // Generate random locations for swapping
        int location1 = rand() % columnNumber;
        int location2 = rand() % columnNumber;

        // Ensure mutual exclusion during swapping
        // waitSemaphore(semid, location1);
        // waitSemaphore(semid, location2);

        // Swap messages in the shared memory
        swapMessages(sharedMemory, columnNumber);

        //Release the semaphores
        // signalSemaphore(semid, location1);
        // signalSemaphore(semid, location2);

        // Sleep for a random interval
        

        unlock(semid);
        // Detach from the shared memory
        if (shmdt(sharedMemory) == -1) {
            perror("shmdt");
            exit(1);
        }
    }

    
    return 0;
}