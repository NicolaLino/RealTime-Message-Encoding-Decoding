#include "header.h"
#include "functions.h"
#include "constants.h"

key_t key;
key_t semKey;
union semun
{
    int val;
    struct semid_ds *buf;
    ushort *array;
};

int status; // determine the status of faliure or success

int createSharedMemory();
int createSemaphore();
void handler1(int signal);
void handler2();
void handler3();

int helper_count = DEFAULT_HELPER_COUNT; // number of helper processes
int spies_count = DEFAULT_SPIES_COUNT;   // number of spy processes

void validateInput(int argc, char **argv)
{
    if (argc != 3)
    {
        printf("\nInsufficient number of arguments. Default values are selected:\n");
        printf("helper_count = %d\n", helper_count);
        printf("spies_count = %d\n", spies_count);
    }
    else
    {

        if (!(helper_count = atoi(argv[1])))
        {
            perror("\nError: The first argument must be an integer");
            exit(-1);
        }

        if (!(spies_count = atoi(argv[2])))
        {
            perror("\nError: The second argument must be an integer");
            exit(-1);
        }
    }
}

int main(int argc, char **argv)
{
    validateInput(argc, argv);
    printf("Number of Helpers= %d\n", helper_count);
    printf("Number of Spies= %d\n", spies_count);
    sleep(2);
    // signal for masterspy
    signal(SIGUSR1, handler2);
    signal(SIGUSR2, handler3);

    char columns[3];
    int shmid = createSharedMemory();
    int semid = createSemaphore();

    key_t key = ftok(".", 's'); // For parent and sender
    int msgQPS = msgget(key, IPC_CREAT | 0666);
    if (msgQPS == -1)
    {
        perror("msgget");
        exit(-1);
    }

    key_t keySpy = ftok(".", 'y'); // For spies and master-spy
    int msgqSpy = msgget(keySpy, IPC_CREAT | 0666);
    if (msgqSpy == -1)
    {
        perror("msgget");
        exit(-1);
    }

    char shm_key[20];
    sprintf(shm_key, "%d", key);
    char sem_key[20];
    sprintf(sem_key, "%d", semKey);

    // pid_t openglPID = fork();
    // switch (openglPID)
    // {
    // case -1: // failed to create opengl id
    //     exit(-1);
    //     break;

    // case 0: // currently in child
    //     execl("./opengl", "OpenGL", NULL);
    //     break;
    // default:
    //     sleep(2); // wait a bit for the opengl to run
    //     break;
    // }

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
        exit(1);
    }

    signal(SIGINT, handler1);
    pause();

    struct message msg;
    if (msgrcv(msgQPS, &msg, sizeof(msg.text), 1, 0) == -1)
    {
        perror("msgrcv");
        exit(-1);
    }
    strcpy(columns, msg.text);

    if (msgrcv(msgQPS, &msg, sizeof(msg.text), 1, 0) == -1)
    {
        perror("msgrcv");
        exit(-1);
    }

    printf("MAX columns is %s\n\n", msg.text);
    fflush(stdout);

    // Fork the helper processes
    pid_t helperPids[helper_count];
    for (int i = 0; i < helper_count; i++)
    {
        helperPids[i] = fork();
        if (helperPids[i] == 0)
        {
            execl("./helper", "helper", columns, shm_key, sem_key, NULL);
            perror("helper execl");
            exit(1);
        }
        else if (helperPids[i] == -1)
        {
            perror("helper fork");
            exit(1);
        }
    }

    // Fork the spy processes
    pid_t spyPids[spies_count];
    for (int i = 0; i < spies_count; i++)
    {
        spyPids[i] = fork();
        if (spyPids[i] == 0)
        {
            execl("./spy", "spy", shm_key, columns, NULL);
            perror("spy execl");
            exit(1);
        }
        else if (spyPids[i] == -1)
        {
            perror("spy fork");
            exit(1);
        }
    }

    // Fork the master spy process
    pid_t masterSpyPid = fork();
    if (masterSpyPid == 0)
    {
        execl("./master_spy", "master_spy", shm_key, columns, msg.text, NULL);
        perror("master_spy execl");
        exit(0);
    }
    else if (masterSpyPid == -1)
    {
        perror("fork");
        exit(1);
    }

    pid_t receiverPid = fork(); // single receiver process
    if (receiverPid == -1)
    {
        perror("fork");
        exit(-1);
    }
    else if (receiverPid == 0)
    {
        execl("./receiver", "receiver", shm_key, columns, msg.text, NULL);
        perror("execl");
        exit(1);
    }
    waitpid(receiverPid, NULL, 0);
    waitpid(masterSpyPid, NULL, 0);

    // Wait for all child processes to complete
    for (int i = 0; i < helper_count; i++)
    {
        kill(helperPids[i], SIGKILL);
        waitpid(helperPids[i], NULL, 0);
    }
    for (int i = 0; i < spies_count; i++)
    {
        kill(spyPids[i], SIGKILL);
        waitpid(spyPids[i], NULL, 0);
    }
    // waitpid(senderPid, NULL, 0);

    // Remove the shared memory segment
    deleteSharedMemory(shmid);

    // Remove the semaphore
    removeSemaphore(SEM_SEED);

    if (status)
    {
        printf("==================Operation Successful==================\n");
    }
    else
    {
        printf("==================Operation Failed==================\n");
    }
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
    semKey = ftok(".", SEM_SEED);

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

    printf("Semaphore is created in parent\n");
    return semid;
}

void handler1(int signal)
{
    printf("Shared Memory is full and ready!\n");
}

void handler2() // master
{
    printf(" master spy is ready!\n");
    exit(5);
    status = 0;
}

void handler3() // Reciever
{
    printf("Reciever is Finished!!\n");
    exit(6);
    status = 1;
}