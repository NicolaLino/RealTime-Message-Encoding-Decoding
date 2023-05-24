#include "header.h"
#include "functions.h"
#include "constants.h"

key_t key;

int createShmem();


int main(int argc, char **argv)
{

    int shmid = createShmem();
    char keyy[20];
    pid_t Sender = fork();

    if (Sender == -1) {
        perror("fork");
        exit(-1);

    } else if (Sender == 0) {
        sprintf(keyy, "%d", key);
        execl("./sender", "sender", keyy, NULL);
    }




    wait(NULL);
    
    return 0;
}


int createShmem(){

    key = ftok(".", 'R');
    int shmid;

    if( (shmid = shmget(key, 1024, IPC_CREAT | 0666)) == -1){

        perror("create shmget error\n");
        exit(-1);

    }

    return shmid;

}



    




/*
parent process create these:

single sender process: 
1) gets the message from an input file (e.g. sender.txt) that contains multiple lines -- DONE
2) split the input file by column based on the blank character between words -- DONE
3) should create as many children processes as needed depending on the number of columns in the file
4) each child process, a column message must be sent containing all the words in that column.
5) The columns should be of equal size.
6) As such, if a column contains an empty string in any line, it should be replaced by the string “alright”.

children processes of sender:
1) each of them encode the column message that is passed to it before placing the encoded message in the shared memory

encoding:
    A) for the first column:
1) each word in the string, we add 1 to the first character modulo 26, 2 to the second character modulo 26, etc.
2) will place it in the first location in the shared memory.

    B) For the second column
1) each word in the string, we add 2 to the first character modulo 26, 4 to the second character modulo 26, etc
2)  will place it in the second location in the shared memory

    C) and so on
    D) Special characters must be encoded as follows in PDF
    E) Numbers are encoded as (1,000,000 - number)
    F) Each encoded column must have a prefix or suffix added to it so that the receiver process is able to identify the column number correctly.


user-defined number of helper processes: 
1) continuously swap the messages that are present in the shared memory to make it hard for spy processes to get all the columns of the file
2) example, a particular helper process might at some point swap between the encoded messages in locations 3 & 10 of the shared memory

Spy processes: 
1) Spy processes will continuously access the shared memory locations randomly to get the encoded messages before sending them to the master spy process

single master spy process: 
1) tries to order the columns in the right order after getting them from the spy processes
2) It will drop columns it already received.
3) When the master spy is confident it got all the columns, it tries to decode the messages
4)  decode the messages in a file called spy.txt before informing the parent process

single receiver process: 
1) continuously access the shared memory locations randomly to get the encoded messages
2) it will order the columns it gets in the right order and drops the columns it already received
3) When it is confident it got all the columns, it tries to decode the messages in a file called receiver.txt before informing the parent process

user-defined number of spy processes: 
1)
2)
3)

The parent process decides if the receiver process was able to get the correct file
from the sender before the master spy process. If true, then the operation is labeled
a successful operation. Otherwise, it is labeled as a failed operation

The simulation ends if any of the following is true:
– The number of failed decoding operations by the receiver exceeds a user-defined
threshold.
– The number of successful decoding operations by the receiver exceeds a userdefined threshold.

*/