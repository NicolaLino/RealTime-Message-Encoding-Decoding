#include "header.h"
#include "functions.h"
#include "constants.h"

int max_columns = 0;
int rows = 0;
pid_t pid;

// Define the message structure
struct message {
    long type;              // Message type
    char text[MAX_MSG_SIZE];  // Message data
};

char*** readFile();

int main(int argc, char **argv) // sender process
{
    char*** output = readFile();
    //pid_t senderChildren[max_columns];

    // create message queue
    key_t key = ftok(".", 'm');
    int msgqid = msgget(key, IPC_CREAT | 0666);
    if (msgqid == -1) {
        perror("msgget");
        exit(-1);
    }

    
    for (int i = 0; i < max_columns; i++) // create sender children
    {
        pid = fork();

        if (pid == -1) // Check for errors when forking child processes
        {
            printf("Error: Failed to fork child %d\n", i + 1);
            exit(-1);
        }
        
        else if (pid == 0) // Code executed in child process
        {
            execl("./senderChild", "senderChild", NULL);
            break;
        } 
        else // Sender process
        {
            //senderChildren[i] = pid;    // PIDs are saved to send each coulmn to each child process
            printf("pid is %d\n",pid);

            // Compose the message
            struct message msg;
            msg.type = (long)pid;
            printf("type is %ld\n", msg.type);
            

            //Rami 

            // char temp [MAX_MSG_SIZE] = "";
            // for (int j=0; j<max_columns; j++){
            //     strcat(temp, output[j][i]);
            //     strcat(temp, " ");
            //     printf("%s\n", temp);
            // }
            // printf("%s\n", temp);
            // strcpy(msg.text, temp);
            // printf("%s\n", msg.text);

            //Rami

           // snprintf(msg.text, sizeof(msg.text), );

            // Send the message
            // if (msgsnd(msgqid, &msg, sizeof(msg.mtext), 0) == -1) {
            //     perror("msgsnd");
            //     exit(EXIT_FAILURE);
            // }
            

        }
    }



    printf("\nEND");

    //Wait for child processes to finish
    for (int i = 0; i < max_columns; i++) {
        wait(NULL);
    }

    return 0;
}

// Function to read the file sender.txt
char*** readFile()
{
    FILE* file = fopen("sender.txt", "r");
    if (file == NULL) {
        printf("Failed to open the file.\n");
        exit(-1);
    }

    char columns[MAX_LENGTH][MAX_LENGTH];
    char line[MAX_LENGTH];
    

    // First loop to determine the maximum number of columns and rows
    while (fgets(line, sizeof(line), file)) {
        int column_count = 0;
        char* token = strtok(line, " ");

        while (token != NULL) {
            token = strtok(NULL, " ");
            column_count++;
        }

        if (column_count >= max_columns) {
            max_columns = column_count;
        }

        rows++;
    }

    printf("Max columns: %d\n", max_columns);
    printf("Rows: %d\n", rows);

    // Reset the file pointer
    fseek(file, 0, SEEK_SET);

    // To store the output
    char*** output = (char***)malloc(rows * sizeof(char**));
    for (int i = 0; i < rows; i++) {
        output[i] = (char**)malloc(max_columns * sizeof(char*));
        for (int j = 0; j < max_columns; j++) {
            output[i][j] = (char*)malloc(MAX_LENGTH * sizeof(char));
        }
    }

    int row_count = 0;

    // Store each word for its row and column
    while (fgets(line, sizeof(line), file)) {
        int len = strlen(line);
        if (len > 0 && line[len - 1] == '\n') {
            line[len - 1] = '0';
            line[len - 2] = ' ';
        }
        printf("%s\n", line);

        int column_count = 0;
        char* token = strtok(line, " ");

        while (token != NULL) {
            if (strcmp(token, "0") == 0) {
                strcpy(output[row_count][column_count], "Alright");
            }
            else {
                strcpy(output[row_count][column_count], token);
            }
            token = strtok(NULL, " ");
            column_count++;
        }

        while (column_count < max_columns) {
            strcpy(output[row_count][column_count], "Alright");
            column_count++;
        }

        row_count++;
    }

    // Close the file
    fclose(file);

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < max_columns; j++) {
            printf("%s\t", output[i][j]);
        }
        printf("\n");
    }

    return output;
}
