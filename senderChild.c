#include "header.h"
#include "functions.h"
#include "constants.h"

// Define the message structure

int open_shmem();
void validateInput(int, char **);
//char *encodeMessage(char *, int);
int open_sem();
void lock(int);
void unlock(int);
char *decodeMessage(char *);

key_t key;
int shmkey;
int Index;
int shmid;
int max_columns;
int column_number;

int main(int argc, char **argv) // sender child process
{
    char *shared_data;
    validateInput(argc, argv);
    Index--;
    int shmid = open_shmem();
    int semid = open_sem();

    int msgqid;

    msgqid = msgget(key, 0666);
    if (msgqid == -1)
    {
        perror("msgget");
        exit(EXIT_FAILURE);
    }

    // Receive messages in the child process
    struct message msg;
    if (msgrcv(msgqid, &msg, sizeof(msg.text), getpid(), 0) == -1)
    {
        perror("msgrcv");
        exit(EXIT_FAILURE);
    }
    sleep(2);

    // Process the received message
    // printf("Child process %d received message: %s\n", getpid(), msg.text);
    char *encodedMessage = encodeMessage(msg.text, column_number);

    usleep(300000);
    lock(semid);

    shared_data = (char *)shmat(shmid, NULL, 0);
    if (shared_data == (char *)(-1))
    {
        perror("shmat");
        exit(EXIT_FAILURE);
    }

    sleep(1);
    printf("Child process [%d] Encoded message: [%s]\n", getpid(), encodedMessage);
    strcpy(shared_data + (Index * 100), encodedMessage);

    // printf("String read from shared memory: %s\n", shared_data + (Index * 100));

    unlock(semid);
    if (shmdt(shared_data) < 0) // deattach the shared memory
    {
        perror("shmdt");
        exit(EXIT_FAILURE);
    }

    //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    // decode message process done like this to avoid errors
    // char *decodedMessage = decodeMessage(encodedMessage);
    // printf("Child process %d Decoded message: %s\n", getpid(), decodedMessage);
    //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

    return 0;
}

void validateInput(int argc, char **argv)
{
    if (argc != 5)
    {
        printf("\nNot enough Args!\n");
        exit(EXIT_FAILURE);
    }

    else
    {
        if (!(key = atoi(argv[1])))
        {
            perror("\nError: The first argument must be an integer");
            exit(EXIT_FAILURE);
        }

        if (!(shmkey = atoi(argv[2])))
        {
            perror("\nError: The second argument must be an integer");
            exit(EXIT_FAILURE);
        }

        if (!(Index = atoi(argv[3]) + 1))
        {
            perror("\nError: The third argument must be an integer");
            exit(EXIT_FAILURE);
        }
        
        if (!(column_number = atoi(argv[3]) + 1))
        {
            perror("\nError: The third argument must be an integer");
            exit(EXIT_FAILURE);
        }
        
        if (!(max_columns = atoi(argv[4])))
        {
            perror("\nError: The fourth argument must be an integer");
            exit(EXIT_FAILURE);
        }
    }
}
