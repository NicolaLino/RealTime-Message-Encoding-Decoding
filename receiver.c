#include "header.h"
#include "functions.h"
#include "constants.h"

int open_shmem();// put in header (repeated two times)

int main(int argc, char **argv) // sender process
{   
    char *key = argv[1];
    int shmid = open_shmem();
    int semid = open_sem();

    
    char* shared_data = (char *) shmat(shmid, NULL, 0);
    if (shared_data == (char *)(-1)) {
        perror("shmat");
        exit(1);
    }

    lock(semid);
    printf("String received from shared memory: %s\n", shared_data);
    unlock(semid);
    
    shmdt(shared_data); 


    return 0;
}
