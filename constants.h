#ifndef __CONSTANTS__
#define __CONSTANTS__

#define MAX_MSG_SIZE 100
#define SEM_SEED 1234
#define MEM_SEED 2314



struct message {
    long type;            // Message type
    char text[MAX_MSG_SIZE];  // Message data
};



#endif