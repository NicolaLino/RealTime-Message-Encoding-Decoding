#include "header.h"
#include "functions.h"
#include "constants.h"

// Define the message structure

int open_shmem();
void validateInput(int, char **);
char *encodeMessage(char *, int);
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
        exit(-1);
    }

    // Receive messages in the child process
    struct message msg;
    if (msgrcv(msgqid, &msg, sizeof(msg.text), getpid(), 0) == -1)
    {
        perror("msgrcv");
        exit(-1);
    }
    //sleep(2);

    // Process the received message
    // printf("Child process %d received message: %s\n", getpid(), msg.text);
    char *encodedMessage = encodeMessage(msg.text, column_number);

    usleep(300000);
    lock(semid);

    shared_data = (char *)shmat(shmid, NULL, 0);
    if (shared_data == (char *)(-1))
    {
        perror("shmat");
        exit(1);
    }
    //sleep(1);
    printf("Child process %d Encoded message: %s\n", getpid(), encodedMessage);
    strcpy(shared_data + (Index * 100), encodedMessage);

    printf("String read from shared memory: %s\n", shared_data + (Index * 100));

    unlock(semid);
    if (shmdt(shared_data) < 0) // deattach the shared memory
    {
        perror("shmdt");
        exit(-1);
    }


    //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    // decode message process done like this to avoid errors
    // char *decodedMessage = decodeMessage(encodedMessage);
    // printf("Child process %d Decoded message: %s\n", getpid(), decodedMessage);
    //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

    //while(1);


    return 0;
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
        if (!(column_number = atoi(argv[3]) + 1))
        {
            perror("\nError: The third argument must be an integer");
            exit(-4);
        }
        if (!(max_columns = atoi(argv[4])))
        {
            perror("\nError: The fourth argument must be an integer");
            exit(-5);
        }
    }
}

char *encodeMessage(char *message, int column)
{   
    char *encodedMessage = malloc((strlen(message) + 1) * sizeof(char)); // Allocate memory for encoded message
    encodedMessage[0] = '\0';                                           // Initialize encodedMessage as an empty string
    
    // Add the column number to the encoded message
    char column_number[10];
    sprintf(column_number, "%d ", column);
    strcat(encodedMessage, column_number);
    
    char *token = strtok(message, " ");                                 // Get the first token (word)
    while (token != NULL)
    {
        int message_length = strlen(token);
        int shift = 0;
        char *encodedWord = malloc((message_length + 1) * sizeof(char)); // Allocate memory for encoded word
        encodedWord[0] = '\0';                                           // Initialize encodedWord as an empty string
        int number = atoi(token);
        if (number){
            number = 1000000 - number;
            sprintf(encodedWord, "%d", number);
        }
        else{
            for (int i = 0; i < message_length; i++)
            {
                if (token[i] >= 'a' && token[i] <= 'z')
                {
                    shift += column;
                    encodedWord[i] = (token[i] - 'a' + shift) % 26 + 'a';
                }
                else if (token[i] >= 'A' && token[i] <= 'Z')
                {
                    shift += column;
                    encodedWord[i] = (token[i] - 'A' + shift) % 26 + 'A';
                }
                else if (token[i] == '!')
                {
                    encodedWord[i] = '1';
                }
                else if (token[i] == '?')
                {
                    encodedWord[i] = '2';
                }
                else if (token[i] == ',')
                {
                    encodedWord[i] = '3';
                }
                else if (token[i] == ';')
                {
                    encodedWord[i] = '4';
                }
                else if (token[i] == ':')
                {
                    encodedWord[i] = '5';
                }
                else if (token[i] == '%')
                {
                    encodedWord[i] = '6';
                }else{
                    encodedWord[i] = token[i];
                }
            }
            encodedWord[message_length] = '\0';
        }
        strcat(encodedMessage, encodedWord);
        strcat(encodedMessage, " ");
        free(encodedWord); // Free memory allocated for encoded word
        token = strtok(NULL, " "); // Get the next token (word)
    }
    encodedMessage[strlen(encodedMessage) - 1] = '\0'; // Remove the last space
    return encodedMessage;
}


char *decodeMessage(char *encodedMessage)
{
    char *token = strtok(encodedMessage, " "); // Get the first token (column number)
    int column = atoi(token);

    char *decodedMessage = NULL; // Initialize decodedMessage as NULL
    size_t decodedLength = 0;

    token = strtok(NULL, " "); // Get the next token (encoded word)
    while (token != NULL)
    {
        int message_length = strlen(token);
        int shift = 0;
        char *decodedWord = malloc((message_length + 1) * sizeof(char)); // Allocate memory for decoded word
        decodedWord[0] = '\0';                                           // Initialize decodedWord as an empty string

        int number = atoi(token);
        if (number)
        {
            number = 1000000 - number;
            sprintf(decodedWord, "%d", number);
        }
        else
        {
            for (int i = 0; i < message_length; i++)
            {
                if (token[i] >= 'a' && token[i] <= 'z')
                {
                    shift += column;
                    decodedWord[i] = (token[i] - 'a' - shift + 26 + 26 + 26) % 26 + 'a';
                }
                else if (token[i] >= 'A' && token[i] <= 'Z')
                {
                    shift += column;
                    decodedWord[i] = (token[i] - 'A' - shift + 26 + 26 + 26) % 26 + 'A';
                }
                else if (token[i] == '1')
                {
                    decodedWord[i] = '!';
                }
                else if (token[i] == '2')
                {
                    decodedWord[i] = '?';
                }
                else if (token[i] == '3')
                {
                    decodedWord[i] = ',';
                }
                else if (token[i] == '4')
                {
                    decodedWord[i] = ';';
                }
                else if (token[i] == '5')
                {
                    decodedWord[i] = ':';
                }
                else if (token[i] == '6')
                {
                    decodedWord[i] = '%';
                }
                else
                {
                    decodedWord[i] = token[i]; // Copy non-decodable characters as is
                }
            }
            decodedWord[message_length] = '\0'; // Add null-terminator to the decoded word
        }

        // Reallocate memory for decodedMessage
        size_t decodedWordLength = strlen(decodedWord);
        decodedMessage = realloc(decodedMessage, (decodedLength + decodedWordLength + 1) * sizeof(char));

        // Concatenate decodedWord to decodedMessage
        strcpy(decodedMessage + decodedLength, decodedWord);
        decodedLength += decodedWordLength;

        // Append a space to separate words
        decodedMessage[decodedLength] = ' ';
        decodedLength++;

        free(decodedWord); // Free the memory allocated for decodedWord
        token = strtok(NULL, " "); // Get the next token (encoded word)
    }

    if (decodedLength > 0)
    {
        // Remove the trailing space character
        decodedMessage[decodedLength - 1] = '\0';
    }
    else
    {
        // If the decoded message is empty, free the memory and set decodedMessage to NULL
        free(decodedMessage);
        decodedMessage = NULL;
    }

    return decodedMessage;
}
