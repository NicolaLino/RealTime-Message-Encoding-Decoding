#include "header.h"
#include "functions.h"
#include "constants.h"

// Define the message structure
struct message {
    long type;              // Message type
    char text[MAX_MSG_SIZE];  // Message data
};

int main(int argc, char **argv) // sender child process
{

    int msgqid;

    key_t key = atoi(argv[1]);
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


    return 0;
}