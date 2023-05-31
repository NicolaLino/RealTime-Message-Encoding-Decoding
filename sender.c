#include "header.h"
#include "functions.h"
#include "constants.h"

int max_columns = 0;
int rows = 0;
pid_t pid;

int open_shmem(); // put in header (repeated two times)

char ***readFile();

int main(int argc, char **argv) // sender process
{
    char keyString[20];
    char index[3];
    char max_columns_send[3];
    char max_rows[3];
    char ***output = readFile();
    // pid_t senderChildren[max_columns];

    char *sh_key = argv[1];
    // printf("%s\n", sh_key);
    // fflush(stdout);

    // create message queue
    key_t key = ftok(".", 'm'); // For sender and senderchild
    int msgqid = msgget(key, IPC_CREAT | 0666);
    if (msgqid == -1)
    {
        perror("msgget");
        exit(EXIT_FAILURE);
    }

    sprintf(keyString, "%d", key);
    sprintf(max_columns_send, "%d", max_columns);
    sprintf(max_rows, "%d", rows);

    // key_t msgqid = createMsgq('m');
    key_t key2 = ftok(".", 's');
    int msgqidSP = msgget(key2, 0666);

    if (msgqidSP == -1)
    {
        perror("msgget");
        exit(EXIT_FAILURE);
    }

    struct message msgSP;
    msgSP.type = 1;
    strcpy(msgSP.text, max_columns_send);
    //printf("msgSP.text %s\n", msgSP.text);
    //fflush(stdout);

    if (msgsnd(msgqidSP, &msgSP, sizeof(msgSP.text) - sizeof(long), 0) == -1)
    { // send column count to parent
        perror("msgsnd");
        exit(EXIT_FAILURE);
    }

    strcpy(msgSP.text, max_rows);
    if (msgsnd(msgqidSP, &msgSP, sizeof(msgSP.text) - sizeof(long), 0) == -1)
    { // send column count to parent
        perror("msgsnd");
        exit(EXIT_FAILURE);
    }
    blue();
    printWordLine("Column");
    for (int i = 0; i < max_columns; i++) // create sender children
    {
        pid = fork();

        if (pid == -1) // Check for errors when forking child processes
        {
            printf("Error: Failed to fork child %d\n", i + 1);
            exit(EXIT_FAILURE);
        }

        else if (pid == 0) // Code executed in child process
        {
            sprintf(index, "%d", i);
            char *args[] = {"./senderChild", keyString, sh_key, index, max_columns_send, NULL};
            execv("./senderChild", args);
            perror("execv");
            // sleep(2);
            break;
        }
        else // Sender process
        {
            struct message msg;
            msg.type = (long)pid;
            char temp[MAX_MSG_SIZE] = "";
            for (int j = 0; j < rows; j++)
            {
                strcat(temp, output[j][i]);
                strcat(temp, " ");
            }
            strcpy(msg.text, temp);
            red();
            printf("Column %d: %s\n", i + 1, msg.text);
            resetColor();
            
            if (msgsnd(msgqid, &msg, sizeof(msg.text), 0) == -1)
            {
                perror("msgsnd");
                exit(EXIT_FAILURE);
            }
        }
    }

    // printf("\nEND\n");
    blue();
    printLine();
    resetColor();

    for (int i = 0; i < max_columns; i++)
    {
        wait(NULL);
    }
    kill(getppid(), SIGINT);
    int shmid = open_shmem();
    int semid = open_sem();

    // char* shared_data = (char *) shmat(shmid, NULL, 0);
    // if (shared_data == (char *)(-1)) {
    //     perror("shmat");
    //     exit(1);
    // }

    // lock(semid);
    // for (int i = 0 ; i < max_columns; i++){
    //         char* message = shared_data + (i * 100);
    //         printf("Message %d: %s\n", i, message);

    // }
    // unlock(semid);

    // shmdt(shared_data);

    // wait for all children to finish

    return 0;
}

// Function to read the file sender.txt
char ***readFile()
{
    FILE *file = fopen("sender.txt", "r");
    if (file == NULL)
    {
        printf("Failed to open the file.\n");
        exit(EXIT_FAILURE);
    }

    char columns[MAX_LENGTH][MAX_LENGTH];
    char line[MAX_LENGTH];

    // First loop to determine the maximum number of columns and rows
    while (fgets(line, sizeof(line), file))
    {
        int column_count = 0;
        char *token = strtok(line, " ");

        while (token != NULL)
        {
            token = strtok(NULL, " ");
            column_count++;
        }

        if (column_count >= max_columns)
        {
            max_columns = column_count;
        }

        rows++;
    }

    printf("Max columns: %d\n", max_columns);
    printf("Rows: %d\n", rows);

    // Reset the file pointer
    fseek(file, 0, SEEK_SET);

    // To store the output
    char ***output = (char ***)malloc(rows * sizeof(char **));
    for (int i = 0; i < rows; i++)
    {
        output[i] = (char **)malloc(max_columns * sizeof(char *));
        for (int j = 0; j < max_columns; j++)
        {
            output[i][j] = (char *)malloc(MAX_LENGTH * sizeof(char));
        }
    }

    int row_count = 0;

    // Store each word for its row and column
    while (fgets(line, sizeof(line), file))
    {
        int len = strlen(line);
        if (len > 0 && line[len - 1] == '\n')
        {
            line[len - 1] = '0';
            line[len - 2] = ' ';
        }
        // printf("%s\n", line);

        int column_count = 0;
        char *token = strtok(line, " ");

        while (token != NULL)
        {
            if (strcmp(token, "0") == 0)
            {
                if (column_count != max_columns)
                    strcpy(output[row_count][column_count], "Alright");
            }
            else
            {
                strcpy(output[row_count][column_count], token);
            }
            token = strtok(NULL, " ");
            column_count++;
        }

        while (column_count < max_columns)
        {
            strcpy(output[row_count][column_count], "Alright");
            column_count++;
        }

        row_count++;
    }

    // Close the file
    fclose(file);
    blue();
    printf("|===================================== Input Message ======================================|\n");
    resetColor();

    red();
    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < max_columns; j++)
        {
            printf("%s\t", output[i][j]);
        }
        printf("\n");
    }
    resetColor();

    return output;
}
