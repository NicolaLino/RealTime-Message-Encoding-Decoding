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
    sleep(2);

    // Process the received message
    printf("Child process %d received message: %s\n", getpid(), msg.text);
    char *encodedMessage = encodeMessage(msg.text, column_number);

    shared_data = (char *)shmat(shmid, NULL, 0);
    if (shared_data == (char *)(-1))
    {
        perror("shmat");
        exit(1);
    }
    sleep(1);
    printf("Child process %d Encoded message: %s\n", getpid(), encodedMessage);
    strcpy(shared_data + (Index * 100), encodedMessage);

    sleep(1);

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
        if (!(column_number = atoi(argv[3])+1))
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
    // Split the message into words
    char *words[max_columns];
    char *token = strtok(message, " ");
    int num_words = 0;
    while (token != NULL && num_words < max_columns)
    {
        words[num_words] = malloc((strlen(token) + 1) * sizeof(char));
        strcpy(words[num_words], token);
        num_words++;
        token = strtok(NULL, " ");
    }

    char *encodedMessage = malloc(((MAX_LENGTH + 1) * max_columns) * sizeof(char)); // Allocate memory for encoded message
    encodedMessage[0] = '\0';                                                       // Initialize encodedMessage as an empty string
    // add prefix to encoded message
    char column_number_string[10];
    sprintf(column_number_string, "%d ", column);
    strcpy(encodedMessage, column_number_string);

    for (int r = 0; r < num_words; r++)
    {
        char *word = words[r];
        int message_length = strlen(word);
        int shift = 0;
        char *encodedWord = malloc((message_length + 1) * sizeof(char)); // Allocate memory for encoded word
        encodedWord[0] = '\0';                                           // Initialize encodedWord as an empty string

        for (int i = 0; i < message_length; i++)
        {
            if (word[i] >= 'a' && word[i] <= 'z')
            {
                shift += column;
                encodedWord[i] = (word[i] - 'a' + shift) % 26 + 'a';
            }
            else if (word[i] >= 'A' && word[i] <= 'Z')
            {
                shift += column;
                encodedWord[i] = (word[i] - 'A' + shift) % 26 + 'A';
            }
            else if (word[i] == '!')
            {
                encodedWord[i] = '1';
            }
            else if (word[i] == '?')
            {
                encodedWord[i] = '2';
            }
            else if (word[i] == ',')
            {
                encodedWord[i] = '3';
            }
            else if (word[i] == ';')
            {
                encodedWord[i] = '4';
            }
            else if (word[i] == ':')
            {
                encodedWord[i] = '5';
            }
            else if (word[i] == '%')
            {
                encodedWord[i] = '6';
            }
            else if (word[i] >= '0' && word[i] <= '9')
            {
                int num = word[i] - '0';
                num = 1000000 - num;
                char numStr[10];
                sprintf(numStr, "%d", num);
                encodedWord[i] = numStr[0];
            }
            else
            {
                encodedWord[i] = word[i]; // Copy non-encodable characters as is
            }
        }
        encodedWord[message_length] = '\0'; // Add null-terminator to the encoded word
        strcat(encodedMessage, encodedWord);
        strcat(encodedMessage, " ");
        free(encodedWord); // Free the memory allocated for encodedWord
    }

    encodedMessage[strlen(encodedMessage) - 1] = '\0'; // Remove the trailing space character
    for (int i = 0; i < num_words; i++)
    {
        free(words[i]); // Free the memory allocated for each word
    }

    return encodedMessage;
}
