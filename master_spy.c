#include "header.h"
#include "constants.h"
#include "functions.h"


int main(int argc, char **argv)
{
    
    struct message msg;
    int columns = atoi(argv[2]);
    int bitmap[columns];
    memset(bitmap, 0x0, sizeof(bitmap));
    
    key_t key2 = ftok(".", 'y');
    int msgqSpy = msgget(key2, 0666);

    if (msgqSpy == -1)
    {
        perror("msgget");
        exit(-1);
    }

    while(!checkColumns(bitmap, columns)){

        
        if (msgrcv(msgqSpy, &msg, sizeof(msg.text), 1, 0) == -1)
        {
            perror("msgrcv");
            exit(-1);
        }
        

        
        //If column is already read, then get another index
        if (bitmap[(int)(msg.text[0])] == 1)
        {
            continue;
        }

        //set column to read
        bitmap[(int)(msg.text[0])] = 1;
        printf("Message recieved in Master Spy %s\n",msg.text);
        fflush(stdout);
        

    }

    printf("GOT ALL COLUMNS IN MASTER_SPY :(");
    fflush(stdout);



    return 0;
}