#include "constants.h"
#include "functions.h"

int stopFlag = 0;

// void handleSignal(int signum) {
//     stopFlag = 1;
// }

int main(int argc, char **argv)
{
    yellow();
    printf("Spy process started\n");
    resetColor();
    
    int shmid = open_shmem();
    int semid = open_sem();
    int columns = atoi(argv[2]);
    int value;
    struct message msgSP;

    srand(time(0) * 5.333 * getpid());

    // Message queue between the spies and the master spy
    key_t key2 = ftok(".", 'y');
    int msgqSpy = msgget(key2, 0666);

    if (msgqSpy == -1)
    {
        perror("msgget");
        exit(EXIT_FAILURE);
    }

    // signal(SIGUSR2, handleSignal);

    int i = 0;
    while (!stopFlag)
    {
        value = (int)(rand() % columns);
        sleep(1);
        lock(semid);

        char *shared_data = attachSharedMemory(shmid);

        // printf("String received from shared memory in Spy: %s\n", shared_data + (value * 100));

        msgSP.type = 1;
        strcpy(msgSP.text, shared_data + (value * 100));

        if (msgsnd(msgqSpy, &msgSP, sizeof(msgSP.text) - sizeof(long), 0) == -1)
        {
            perror("msgsnd");
            unlock(semid);
            shmdt(shared_data);
            exit(EXIT_FAILURE);
        }
        else
        {
            // printf("msg sent from spy is: %s\n", msgSP.text);
            // fflush(stdout);
        }

        unlock(semid);
        shmdt(shared_data);

        if (stopFlag)
        {
            printf("Spy %d is terminating\n", getpid());
            fflush(stdout);
            exit(EXIT_SUCCESS);
        }
    }

    return 0;
}
