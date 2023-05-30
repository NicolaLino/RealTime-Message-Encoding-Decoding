#include "header.h"
#include "constants.h"
#include "functions.h"


int main(int argc, char **argv)
{
    
    struct message msg;
    int columns = atoi(argv[2]);
    int rows = atoi(argv[3]);
    int bitmap[columns];
    char arrange[columns][MAX_MSG_SIZE];
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
        

        int value = msg.text[0] - '0';
        value = value - 1;
        //If column is already read, then get another index
        if (bitmap[value] == 1)
        {
            continue;
        }



        //set column to read
        bitmap[value] = 1;
        // printf("Message recieved in Master Spy %s\n",msg.text);
        fflush(stdout); 
        strcpy(arrange[value], msg.text);
        char *decode = decodeMessage(arrange[value]);
        strcpy(arrange[value], decode);
        purple();   
        printf("Decoded String ==============> [%s]\n", arrange[value]);   
        resetColor();
    }

    int maxsize = 0;
    char** output = ColumntoRow(arrange, columns, &maxsize);

    writeFile(output, columns, maxsize, rows, "spy.txt");
    blue();
    printWordLine("Master spy");
    resetColor();
    purple();
    for (int i = 0; i < rows; i++)
    {
        printf("%s\n", output[i]);
        fflush(stdout);
    }
    resetColor();
    blue();
    printLine();
    resetColor();




    //printf("GOT ALL COLUMNS IN MASTER_SPY :(\n");
    //fflush(stdout);


    kill(getppid(), SIGUSR1);



    return 0;
}