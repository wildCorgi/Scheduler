
#include "headers.h"
#include "queue.c"

void clearResources(int);
int readFile(PCB * pcbs );
void createMessageQueue(int *msgID, int ID);
void sendProcess(struct PCB pc, int processMsgID);
void sendAlgoNumber(int algoMsgID, int schedulingAlgo);
int main(int argc, char *argv[])
{
    //signal(SIGINT, clearResources);
    bool END = false;
    int schedulingAlgo = -1;
    int quantum = 0;
    int processesNumber =5;
    int processMsgID;
    PCB * pcbs = (PCB*)malloc(100 * sizeof(PCB)); 

    // TODO Initialization
    // 1. Read the input files.

    createMessageQueue(&processMsgID, processQueueID);


    processesNumber = readFile(pcbs);

    // 2. Ask the user for the chosen scheduling algorithm and its parameters, if there are any.

    while (schedulingAlgo < 1 || schedulingAlgo > 3)
    {

        printf("\nEnter the wanted scheduling algorithm:\n1.HPF\n2.SRTN\n3.RR\n");
        scanf("%d", &schedulingAlgo);
    }

    if (schedulingAlgo == 3)
    {
        while (quantum < 1)
        {
            printf("\nEnter the wanted Quantum:\n");
            scanf("%d", &quantum);
        }
    }




    // 3. Initiate and create the scheduler and clock processes

    int pid = fork();

    if (pid == -1)
    {
        perror("Error in fork!!");
    }
    else if (pid == 0)
    {
        execl("./clk.out", "clk.out", NULL);
    }

    pid = fork();

    if (pid == -1)
    {
        perror("Error in fork!!");
    }
    else if (pid == 0)
    {
        char str[64];
        sprintf(str, "%d", schedulingAlgo);
        char str1[64];
        sprintf(str1, "%d", quantum);
        char str2[64];
        sprintf(str2, "%d", processesNumber);

        execl("./scheduler.out", "scheduler.out", str,str1,str2,NULL);
    }

    // 4. Use this function after creating the clock process to initialize clock

    initClk();
    int i=0;

    while (true)
    {
        
            int x = getClk();
  
            while(i < processesNumber && pcbs[i].arrivalTime <= x )
            {
                x = getClk();
                sendProcess(pcbs[i], processMsgID);
                i++;
                       
                   
                printf("ba3t le child\n");

            }
    }

    // 7. Clear clock resources

    destroyClk(true);
}

void clearResources(int signum)
{
    //TODO Clears all resources in case of interruption
}

void sendProcess(struct PCB pcb, int processMsgID)
{
    int send_val;
    struct msgPBuff temp;

    temp.pcb = pcb;
    temp.mtype = processMType;

    send_val = msgsnd(processMsgID, &temp, sizeof(temp.pcb), !IPC_NOWAIT);

    if (send_val == -1)
    {
        perror("Error in Sending Process");
    }
}

void createMessageQueue(int *msgID, int ID)
{

    (*msgID) = msgget((ID), IPC_CREAT | 0666);

    if ((*msgID) == -1)
    {
        perror("Error in create");
    }
}

int readFile(PCB * pcbs )
{
    int i=0;
    int info[4];
    FILE *fp;
    fp = fopen("Input.txt", "r");
    char line[256];
    while (fgets(line, sizeof(line), fp) != NULL)
    {
        if (line[0] != '#')
        {
            sscanf(line, "%d\t%d\t%d\t%d", &info[0], &info[1], &info[2], &info[3]);
            printf("%d\t%d\t%d\t%d\n", info[0], info[1], info[2], info[3]);
            struct PCB temp;
            temp.processID = info[0];
            temp.arrivalTime = info[1];
            temp.runTime = info[2];
            temp.priority = info[3];
            pcbs[i] = temp;
            i++;
        }
        else
            continue;
    }
    fclose(fp);
    return i;
}