#include "header.h"
#include "functions.h"
#include "constants.h"

int stopFlag = 0;

void swapMessages(char* sharedMemory, int columnNumber) {
    int location1 = rand() % columnNumber;
    int location2 = rand() % columnNumber;

    char temp[100];
    strcpy(temp, &sharedMemory[location1 * 100]);
    strcpy(&sharedMemory[location1 * 100], &sharedMemory[location2 * 100]);
    strcpy(&sharedMemory[location2 * 100], temp);
}

// void handleSignal(int signum) {
//     stopFlag = 1;
// }

int main(int argc, char **argv)
{
    int columnNumber = atoi(argv[1]);
    int shmKey = atoi(argv[2]);


    srand(time(NULL));

    int shmid = open_shmem();
    int semid = open_sem();

    // signal(SIGUSR1, handleSignal);
    printf("Helper %d is running\n", getpid());
    
    while (!stopFlag) {
        int sleepTime = rand() % 5 + 1;
        sleep(sleepTime);
        lock(semid);        
        
        fflush(stdout);
        char *sharedMemory = attachSharedMemory(shmid);
        // Generate random locations for swapping
        int location1 = rand() % columnNumber;
        int location2 = rand() % columnNumber;

        // Swap messages in the shared memory
        swapMessages(sharedMemory, columnNumber);


        unlock(semid);
        // Detach from the shared memory
        if (shmdt(sharedMemory) == -1) {
            perror("shmdt");
            exit(1);
        }

        if(stopFlag) {
            printf("Helper %d is terminating\n", getpid());
            fflush(stdout);
            exit(0);
        }
    }

    
    return 0;
}