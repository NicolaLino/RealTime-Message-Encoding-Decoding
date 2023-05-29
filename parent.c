#include "header.h"
#include "functions.h"
#include "constants.h"

key_t key;
union semun
{
    int val;
    struct semid_ds *buf;
    ushort *array;
};

int createSharedMemory();
int createSemaphore();

int helper_count = 2; // number of helper processes
int spies_count = 2;  // number of spies processes

int main(int argc, char **argv)
{

    int shmid = createSharedMemory();
    int semid = createSemaphore();
    char shm_key[20];
    sprintf(shm_key, "%d", key);

    pid_t senderPid = fork(); // single sender process
    if (senderPid == -1)
    {
        perror("fork");
        exit(-1);
    }
    else if (senderPid == 0)
    {
        execl("./sender", "sender", shm_key, NULL);
        perror("execl");

    }

    // Fork the helper processes
    pid_t helperPids[helper_count];
    for (int i = 0; i < helper_count; i++)
    {
        helperPids[i] = fork();
        if (helperPids[i] == 0)
        {
            int columnNumber = 5;
            char columnNumberStr[20];
            sprintf(columnNumberStr, "%d", columnNumber);
            printf("columnNumberStr = %s\n", columnNumberStr);
            execl("./helper", "helper",shm_key, columnNumberStr,  NULL);
            perror("helper execl");
            exit(0);
        }
        else if (helperPids[i] == -1)
        {
            perror("helper fork");
            exit(1);
        }
    }

    // Fork the spy processes
    // pid_t spyPids[spies_count];
    // for (int i = 0; i < spies_count; i++)
    // {
    //     spyPids[i] = fork();
    //     if (spyPids[i] == 0)
    //     {
    //         // Code for spy process
    //         // ...
    //         exit(0);
    //     }
    //     else if (spyPids[i] == -1)
    //     {
    //         perror("fork");
    //         exit(1);
    //     }
    // }

    // Fork the master spy process
    // pid_t masterSpyPid = fork();
    // if (masterSpyPid == 0)
    // {
    //     // Code for master spy process
    //     // ...
    //     exit(0);
    // }
    // else if (masterSpyPid == -1)
    // {
    //     perror("fork");
    //     exit(1);
    // }

    pid_t receiverPid = fork(); // single receiver process
    if (receiverPid == -1)
    {
        perror("fork");
        exit(-1);
    }
    else if (receiverPid == 0)
    {
        execl("./receiver", "receiver", shm_key, NULL);
    }

    // Wait for all child processes to complete
    for (int i = 0; i < helper_count; i++)
    {
        waitpid(helperPids[i], NULL, 0);
    }
    // for (int i = 0; i < spies_count; i++) {
    //     waitpid(spyPids[i], NULL, 0);
    // }
    waitpid(senderPid, NULL, 0);
    waitpid(receiverPid, NULL, 0);
    // waitpid(masterSpyPid, NULL, 0);

    // Remove the shared memory segment
    deleteSharedMemory(shmid);

    // Remove the semaphore
    removeSemaphore(SEM_SEED);

    return 0;
}

int createSharedMemory()
{

    key = ftok(".", MEM_SEED);
    int shmid;
    if ((shmid = shmget(key, 100, IPC_CREAT | 0666)) == -1)
    {
        perror("create shmget error\n");
        exit(-1);
    }
    return shmid;
}

int createSemaphore()
{
    key_t semKey = ftok(".", SEM_SEED);

    int semid = semget(semKey, 1, IPC_CREAT | 0666);
    if (semid == -1)
    {
        perror("semget -- sim_system");
        exit(-1);
    }
    /* Set the value of the semaphore to 1 */
    union semun arg;
    arg.val = 1;
    if (semctl(semid, 0, SETVAL, arg) == -1)
    {
        perror("semctl");
        exit(-1);
    }
    printf("semaphore is created in parent\n");
    return semid;
}

/*
parent process create these:

single sender process:
1) gets the message from an input file (e.g. sender.txt) that contains multiple lines -- DONE
2) split the input file by column based on the blank character between words -- DONE
3) should create as many children processes as needed depending on the number of columns in the file
4) each child process, a column message must be sent containing all the words in that column.
5) The columns should be of equal size.
6) As such, if a column contains an empty string in any line, it should be replaced by the string “alright”.

children processes of sender:
1) each of them encode the column message that is passed to it before placing the encoded message in the shared memory

encoding:
    A) for the first column:
1) each word in the string, we add 1 to the first character modulo 26, 2 to the second character modulo 26, etc.
2) will place it in the first location in the shared memory.

    B) For the second column
1) each word in the string, we add 2 to the first character modulo 26, 4 to the second character modulo 26, etc
2)  will place it in the second location in the shared memory

    C) and so on
    D) Special characters must be encoded as follows in PDF
    E) Numbers are encoded as (1,000,000 - number)
    F) Each encoded column must have a prefix or suffix added to it so that the receiver process is able to identify the column number correctly.


user-defined number of helper processes:
1) continuously swap the messages that are present in the shared memory to make it hard for spy processes to get all the columns of the file
2) example, a particular helper process might at some point swap between the encoded messages in locations 3 & 10 of the shared memory

Spy processes:
1) Spy processes will continuously access the shared memory locations randomly to get the encoded messages before sending them to the master spy process

single master spy process:
1) tries to order the columns in the right order after getting them from the spy processes
2) It will drop columns it already received.
3) When the master spy is confident it got all the columns, it tries to decode the messages
4)  decode the messages in a file called spy.txt before informing the parent process

single receiver process:
1) continuously access the shared memory locations randomly to get the encoded messages
2) it will order the columns it gets in the right order and drops the columns it already received
3) When it is confident it got all the columns, it tries to decode the messages in a file called receiver.txt before informing the parent process

user-defined number of spy processes:
1)
2)
3)

The parent process decides if the receiver process was able to get the correct file
from the sender before the master spy process. If true, then the operation is labeled
a successful operation. Otherwise, it is labeled as a failed operation

The simulation ends if any of the following is true:
– The number of failed decoding operations by the receiver exceeds a user-defined
threshold.
– The number of successful decoding operations by the receiver exceeds a userdefined threshold.

*/