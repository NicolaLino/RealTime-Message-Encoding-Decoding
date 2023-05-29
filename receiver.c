#include "header.h"
#include "functions.h"
#include "constants.h"

int open_shmem();// put in header (repeated two times)
int checkColumns(int[], int);
void writeFile(char [][MAX_LENGTH], int);


int main(int argc, char **argv) // sender process
{   
    
    //signal(SIGUSR1, accessShmem);
    //pause();
    char *key = argv[1];
    int shmid = open_shmem();
    int semid = open_sem();
    int columns = atoi(argv[2]);
    int value;
    int read[columns];
    int bitmap[columns];
    char arrange[columns][MAX_LENGTH];
    memset(read, 0x0, sizeof(read));
    memset(bitmap, 0x0, sizeof(bitmap));


    printf("Im in reciever\n");
    int i = 0;
    srand(time(0) * 5.333 * getpid()); // to make it more random

    while(checkColumns(bitmap,columns) == 0){

       
        value =(int) (rand() % columns);

        if(bitmap[value] == 1){
            continue;
        }
        
        
        bitmap[value] = 1;

        usleep(600000);
        lock(semid);
        char* shared_data = (char *) shmat(shmid, NULL, 0);
        if (shared_data == (char *)(-1)) {
            perror("shmat");
            exit(1);
        }

        printf("String received from shared memory in reciever: %s\n", shared_data + (value * 100));
        strcpy(arrange[value], shared_data + (value * 100));


        unlock(semid);
        shmdt(shared_data); 
    
    }

    writeFile(arrange, columns);


    for (int i = 0 ; i < columns; i++){
        printf("%s\n", arrange[i]);
        fflush(stdout);
    }




 
    return 0;
}


void accessShmem(){

}

int checkColumns(int arr[], int columns){

    for (int i = 0; i < columns; i++){
        if(arr[i] == 0)
            return 0;

    }

    return 1;



}


void writeFile(char arrange[][MAX_LENGTH], int columns) {
    // Rest of the code remains the same
    FILE* file = fopen("receiver.txt", "w");
    if (file == NULL) {
        perror("Error opening file\n");
        exit(1);
    }

    for (int i = 0; i < columns; i++) {
        fprintf(file, "%s\n", arrange[i]);
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