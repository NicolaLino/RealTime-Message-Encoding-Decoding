#include "header.h"
#include "functions.h"
#include "constants.h"

// Define the message structure

int open_shmem(int);
void validateInput(int, char **);
char *encodeMessage(char *, int);
struct message
{
    long type;               // Message type
    char text[MAX_MSG_SIZE]; // Message data
};

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
    int shmid = open_shmem(shmkey);

    int msgqid;

    msgqid = msgget(key, 0666);
    if (msgqid == -1)
    {
        perror("msgget");
        exit(-1);
    }

    // Receive messages in the child process
    struct message msg;
    if (msgrcv(msgqid, &msg, sizeof(msg.text), getpid(), 0) == -1)
    {
        perror("msgrcv");
        exit(-1);
    }

    //Process the received message
    printf("Child process %d received message: %s\n", getpid(), msg.text);
    // char *encodedMessage = encodeMessage(msg.text, column_number);


    shared_data = (char *)shmat(shmid, NULL, 0);
    if (shared_data == (char *)(-1))
    {
        perror("shmat");
        exit(1);
    }

    // printf("\nChild process %d Encoded message: %s\n", getpid(), encodedMessage);
    strcpy(shared_data + (Index * 100), msg.text);

    printf("String read from shared memory: %s\n", shared_data + (Index * 100));

    return 0;
}

int open_shmem(int key)
{

    int shmid;
    if ((shmid = shmget(key, 0, 0)) == -1)
    {
        perror("Error opening shared memory from Parent");
        exit(-2);
    }
    return shmid;
}

void validateInput(int argc, char **argv)
{
    if (argc != 5)
    {
        printf("\nNot enough Args!\n");
    }

    else
    {  
        if (!(key = atoi(argv[1])))
        {
            perror("\nError: The first argument must be an integer");
            exit(-1);
        }

        if (!(shmkey = atoi(argv[2])))
        {
            perror("\nError: The second argument must be an integer");
            exit(-3);
        }

        if (!(Index = atoi(argv[3]) + 1))
        {
            perror("\nError: The third argument must be an integer");
            exit(-4);
        }

        if (!(max_columns = atoi(argv[4])))
        {
            perror("\nError: The fourth argument must be an integer");
            exit(-5);
        }
        if (!(column_number = atoi(argv[5])))
        {
            perror("\nError: The fifth argument must be an integer");
            exit(-6);
        }
    }
}


char* encodeMessage(char *message, int column)
{
    int message_length = strlen(message);
    char *encodedMessage = malloc((message_length + 1) * sizeof(char)); // Allocate memory for encoded message
    int shift = 0;
    for (int i = 0; i < message_length; i++)
    {
        if (message[i] >= 'a' && message[i] <= 'z')
        {
            shift += (column + 1);
            printf("%d", shift);
            encodedMessage[i] = (message[i] - 'a' + shift) % 26 + 'a';
        }
        else if (message[i] >= 'A' && message[i] <= 'Z')
        {
            shift += (column + 1);
            printf("%d", shift);
            encodedMessage[i] = (message[i] - 'A' + shift) % 26 + 'A';
        }
        else if (message[i] == '!')
        {
            encodedMessage[i] = '1';
        }
        else if (message[i] == '?')
        {
            encodedMessage[i] = '2';
        }
        else if (message[i] == ',')
        {
            encodedMessage[i] = '3';
        }
        else if (message[i] == ';')
        {
            encodedMessage[i] = '4';
        }
        else if (message[i] == ':')
        {
            encodedMessage[i] = '5';
        }
        else if (message[i] == '%')
        {
            encodedMessage[i] = '6';
        }
        else if (message[i] >= '0' && message[i] <= '9')
        {
            int num = message[i] - '0';
            num = 1000000 - num;
            char numStr[10];
            sprintf(numStr, "%d", num);
            encodedMessage[i] = numStr[0];
        }
        else
        {
            encodedMessage[i] = message[i]; // Copy non-encodable characters as is
        }
    }
    return encodedMessage;
}
