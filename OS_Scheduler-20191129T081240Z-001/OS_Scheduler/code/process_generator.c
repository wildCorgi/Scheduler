
#include "headers.h"
#include "queue.c"

void clearResources(int);
void readFile(queue *pcbs);
void createMessageQueue(int *msgID, int ID);
void sendProcess(struct PCB pc, int processMsgID);
void sendAlgoNumber(int algoMsgID, int schedulingAlgo);
int main(int argc, char *argv[])
{
    //signal(SIGINT, clearResources);
    bool END = false;
    int schedulingAlgo = -1;
    int quantum = 0;
    int processMsgID, algoMsgID;
    int pid;

    // TODO Initialization
    // 1. Read the input files.

    createMessageQueue(&processMsgID, processQueueID);
    createMessageQueue(&algoMsgID, algoQueueID);

    queue *pcbs = createQueue();
    readFile(pcbs);

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

    schedulingAlgo += quantum - 1;
    sendAlgoNumber(algoMsgID, schedulingAlgo);

    // 3. Initiate and create the scheduler and clock processes

    pid = fork();

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
        execl("./scheduler.out", "scheduler.out", NULL);
    }

    // 4. Use this function after creating the clock process to initialize clock

    initClk();

    while (true)
    {
        int x = getClk();
        if (pcbs->front != NULL)
        {
            int x = getClk();
            struct PCB temp;
            perror("a7aih");
            if(pcbs->front == NULL)
                continue;
            if (pcbs->front->data.arrivalTime == x)
            {
                temp = dequeue(pcbs)->data;
                temp.forked=false;
                sendProcess(temp, processMsgID);
            }
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

void sendAlgoNumber(int algoMsgID, int schedulingAlgo)
{

    algoInfo algo;

    algo.info = schedulingAlgo;
    algo.mtype = algoMType;

    int algosend = msgsnd(algoMsgID, &algo, sizeof(algo.info), !IPC_NOWAIT);

    if (algosend == -1)
    {
        perror("Error Sending");
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

void readFile(queue *pcbs)
{

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
            enqueue(pcbs, temp);
        }
        else
            continue;
    }
    fclose(fp);
}