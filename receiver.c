#include "header.h"
#include "functions.h"
#include "constants.h"

int open_shmem(int);// put in header (repeated two times)

int main(int argc, char **argv) // sender process
{   
    char *key = argv[1];
    int shmkey = atoi(key);
    
    int shmid = open_shmem(shmkey);

    char* shared_data = (char *) shmat(shmid, NULL, 0);
    if (shared_data == (char *)(-1)) {
        perror("shmat");
        exit(1);
    }
    printf("String received from shared memory: %s\n", shared_data);


    return 0;
}

int open_shmem(int key){

    int shmid;
	if((shmid=shmget(key,0,0))==-1){
		perror("Error opening shared memory from Parent");
		exit(-2);
	}
	return shmid;
}