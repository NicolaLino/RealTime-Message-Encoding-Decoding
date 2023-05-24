#include "header.h"
#include "functions.h"
#include "constants.h"

// Define the message structure

int open_shmem(int);
void validateInput(int, char**);
struct message {
    long type;              // Message type
    char text[MAX_MSG_SIZE];  // Message data
};

key_t key;
int shmkey;
int Index;
int shmid;
int max_columns;

int main(int argc, char **argv) // sender child process
{

    validateInput(argc, argv);
    Index--;
    int shmid = open_shmem(shmkey);


    int msgqid;
    
    msgqid = msgget(key, 0666);
    if (msgqid == -1) {
        perror("msgget");
        exit(-1);
    }

    // Receive messages in the child process
    struct message msg;
    if (msgrcv(msgqid, &msg, sizeof(msg.text), getpid(), 0) == -1) {
        perror("msgrcv");
        exit(-1);
    }


    // Process the received message
    printf("Child process %d received message: %s\n", getpid(), msg.text);

    char* shared_data = shmat(shmid, NULL, 0);
    if (shared_data == (char *)(-1)) {
        perror("shmat");
        exit(1);
    }

    strcpy(shared_data + (Index * 1024), msg.text);
    printf("String read from shared memory: %s\n", shared_data + (Index * 1024));

  
       /* for (int i = 0 ; i < max_columns; i++){
            char* message = shared_data + (i * 1024);
            printf("Message %d: %s\n", i, message);

        }*/

    //printf("String read from shared memory: %s\n", shared_data);

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

    }
}