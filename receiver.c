#include "header.h"
#include "functions.h"
#include "constants.h"

int open_shmem(); // put in header (repeated two times)
// int checkColumns(int[], int);
void writeFile(char**, int, int, int);

int main(int argc, char **argv) // sender process
{
    // signal(SIGUSR1, accessShmem);
    // pause();
    char *key = argv[1];
    int shmid = open_shmem();
    int semid = open_sem();
    int columns = atoi(argv[2]);
    int rows = atoi(argv[3]);
    int value;
    int read[columns];
    int bitmap[columns];
    char arrange[columns][MAX_MSG_SIZE];
    memset(read, 0x0, sizeof(read));
    memset(bitmap, 0x0, sizeof(bitmap));

    printf("Im in reciever\n");
    int i = 0;
    srand(time(0) * 5.333 * getpid()); // to make it more random

    while (!checkColumns(bitmap, columns))
    {
        // Generate a random value(index)
        value = (int)(rand() % columns);

        // If column is already read, then get another index
        if (bitmap[value] == 1)
        {
            continue;
        }

        // set column to read
        bitmap[value] = 1;

        // lock with semaphore
        usleep(600000);
        lock(semid);

        // Read from the shared memory
        char *shared_data = attachSharedMemory(shmid);

        printf("String received from shared memory in reciever: %s\n", shared_data + (value * 100));
        strcpy(arrange[value], shared_data + (value * 100));
        char *decode = decodeMessage(arrange[value]);
        strcpy(arrange[value], decode);

        // unlock
        unlock(semid);
        shmdt(shared_data);
    }

    // fix this to return

    int maxsize = 0; // Declare maxsize variable
    //char** output = ColumntoRow(arrange, columns, &maxsize);

   // writeFile(output, columns, maxsize);
    printf("MAXXXXXX SIZE : %d\n\n\n\n\n", maxsize);
    char** output = ColumntoRow(arrange, columns, &maxsize);

    writeFile(output, columns, maxsize, rows);

    for (int i = 0; i < rows; i++)
    {
        printf("%s\n", output[i]);
        fflush(stdout);
    }


    return 0;
}

/*int checkColumns(int arr[], int columns)
{

    for (int i = 0; i < columns; i++)
    {
        if (arr[i] == 0)
            return 0;
    }

    return 1;
}*/

void writeFile(char **arrange, int columns, int maxsize, int rows)
{
    // Rest of the code remains the same
    FILE *file = fopen("receiver.txt", "w");
    if (file == NULL)
    {
        perror("Error opening file\n");
        exit(1);
    }

    for (int i = 0; i < rows; i++)
    {
        //if(arrange[i][0] != '\0')
            fprintf(file, "%s\n", arrange[i]);
        
        //else
            //break;
    }

    fclose(file);
}

/*
                        --ToDo List--

1- Bitmap to trace columns --> done
2- put the columns in order in an array arr[rows][length] --> done
3- make sure all is gotten -->done
4- decode the file
5- write in file -->done
6- notify the parent


*/