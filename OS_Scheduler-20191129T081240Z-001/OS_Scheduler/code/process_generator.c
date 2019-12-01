#include "headers.h"

void clearResources(int);

int main(int argc, char * argv[])
{
    signal(SIGINT, clearResources);
    // TODO Initialization
    // 1. Read the input files.
    // 2. Ask the user for the chosen scheduling algorithm and its parameters, if there are any.
    // 3. Initiate and create the scheduler and clock processes.
    // 4. Use this function after creating the clock process to initialize clock
    initClk();
    // To get time use this
    int x = getClk();
    printf("current time is %d\n", x);
    // TODO Generation Main Loop
    // 5. Create a data structure for processes and provide it with its parameters.
    // 6. Send the information to the scheduler at the appropriate time.
    // 7. Clear clock resources
    destroyClk(true);
}

void clearResources(int signum)
{
    //TODO Clears all resources in case of interruption
}
<<<<<<< Updated upstream
=======

void sendProcess(struct PCB  pcb,int processMsgID)
{
    int send_val;
    struct msgPBuff temp;
    temp.pcb  = pcb;
    temp.mtype = processMType;
    send_val = msgsnd(processMsgID, &temp, sizeof(temp), !IPC_NOWAIT);
    if(send_val ==-1)
           perror("Errror in Sending Process");
}


void createMessageQueue(int *msgID,int ID)
{
(*msgID)= msgget((ID),IPC_CREAT | 0666 );
}


void readFile(queue * pcbs)
{

    int info[4];
    FILE * fp; 
    fp = fopen("Input.txt", "r");
    char line[256];
    while(fgets(line, sizeof(line), fp) != NULL)
    {
        if(line[0] != '#')
        {
            sscanf(line,"%d\t%d\t%d\t%d", &info[0], &info[1],&info[2], &info[3]);
            printf("%d\t%d\t%d\t%d\n",info[0], info[1],info[2],info[3]);
            struct PCB temp;
            temp.processID = info[0];
            temp.arrivalTime = info[1];
            temp.runTime = info[2];
            temp.priority = info[3];
            temp.forked = 0;
            enqueue(pcbs,temp);
        }
        else
            continue; 
    }
    fclose(fp);


}
>>>>>>> Stashed changes
