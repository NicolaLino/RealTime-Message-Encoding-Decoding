#include "header.h"
#include "functions.h"
#include "constants.h"

void swapMessages(char *sharedMemory, int location1, int location2)
{
    char temp[100];

    strcpy(temp, &sharedMemory[location1]);
    strcpy(&sharedMemory[location1], &sharedMemory[location2]);
    strcpy(&sharedMemory[location2], temp);
}

int main(int argc, char **argv)
{
    int shmid;
    char *sharedMemory;
    int columnNumber = atoi(argv[2]);

    printf("argv[1] = %s\n", argv[1]);
    shmid = shmget(atoi(argv[1]), 0, 0);
    if (shmid == -1)
    {
        perror("Error opening shared memory from Helper");
        exit(-2);
    }

    sharedMemory = attachSharedMemory(shmid);

    int count;
    while (count < 10)
    {
        count++;
        // Generate random locations to swap
        int location1 = rand() % (columnNumber * 100);
        int location2 = rand() % (columnNumber * 100);

        // Swap the locations
        swapMessages(sharedMemory, location1, location2);

        // Sleep for random interval before swapping again

        int sleepTime = rand() % 5 + 1;
        sleep(sleepTime);
    }

    detachSharedMemory(sharedMemory);

    return 0;
}