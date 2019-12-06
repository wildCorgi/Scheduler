
#include "queue.c"


void createMessageQueue(int *msgID,int ID);
int  recieveMSG(int ID);
void doRR(int quantum,int algo);
int main(int argc, char *argv[])
{
    
    int algo =0;
    initClk();
 
    //TODO implement the scheduler :)
    //upon termination release the clock resources

    int algoQueue;
    createMessageQueue(&algoQueue,algoQueueID);   
    
    algo = recieveMSG(algoQueue);
    printf("The algo number is: %d",algo);

    int quantum;
    
    
    printf("\nThe algo number is: %d \n",algo);
   
    if (algo == 0)
    { 
        // implement HPF
        printf("The algo number is: %d",algo);
    }
   
    else if (algo == 1)
<<<<<<< HEAD
    { 
        //implement SRTN here
        printf("The algo number is: %d",algo);
=======
    { //implement SRTN here
        queue *currentProcesses = createQueue();
        msgPBuff receivedProcess;
        //PCB *crntptr;
        node *excutedtPtr = NULL;
        while (1)
        {
        fml:
            int PCBRCV = 1;
            while (PCBRCV > 0)
            {
                PCBRCV = msgrcv(processQueueID, &receivedProcess, sizeof(receivedProcess), processMType, IPC_NOWAIT);
                remainingTimeEnqueue(currentProcesses, receivedProcess.pcb);
            }
            node *crntPtr = currentProcesses->front;
            if (crntPtr == NULL)
                goto fml;
            if (excutedtPtr == NULL)
            {
                excutedtPtr = crntPtr;
            }
            if (!excutedtPtr->data.forked)
            {
                int forkTime = getClk();
                int time = getClk();
                int forking = fork();
                if (forking == -1)
                {
                    perror("Couldn't fork process. ");
                }
                else if (forking == 0)
                {
                    execl("./process.out", "process.out", NULL);
                }
                else
                {
                    crntPtr->data.forked = true;
                    crntPtr->data.forkID = forking;
                    if (excutedtPtr != currentProcesses->front)
                    {
                        int pid = excutedtPtr->data.forkID;
                        kill(pid, SIGSTOP);
                    }
                    if (getClk() != time)
                    {
                        excutedtPtr->data.runTime--;
                        time = getClk;
                    }
                }
            }
            else
            {
                int time = getClk();
                kill(crntPtr->data.forkID, SIGCONT);
                if (excutedtPtr != currentProcesses->front)
                {
                    int pid = excutedtPtr->data.forkID;
                    kill(pid, SIGSTOP);
                }
                if (getClk() != time)
                {
                    excutedtPtr->data.runTime--;
                    time = getClk;
                }
            }
        }
>>>>>>> cab854a0241ab7e1b08ce2175589dcff55a8e198
    }
   
    else
    { 
       //implement RR
        
       printf("The algo number is: %d",algo);
       //doRR(quantum,algo);
    }
 
    destroyClk(true);
}


void doRR(int quantum,int algo)
{
     quantum = algo - 2;
        queue *currentProcesses = createQueue();
        FILE *outputFile;
        outputFile = fopen("./output.txt", "a");
        msgPBuff receivedProcess;
        int waitingTime = 0;
        while (1)
        {
<<<<<<< HEAD
            printf("\nThe algo number is: %d \n",algo);
=======
            char outputString[] = "#At time x process y state arr w total z remain y wait k \n";
>>>>>>> cab854a0241ab7e1b08ce2175589dcff55a8e198
            int PCBRCV = msgrcv(processQueueID, &receivedProcess, sizeof(receivedProcess), processMType, IPC_NOWAIT);
            fwrite(outputString, 1, sizeof(outputString), outputFile);
            if (PCBRCV > 0)
            {
                enqueue(currentProcesses, receivedProcess.pcb);
            }
            node *crntPtr = currentProcesses->front;
            while (crntPtr != NULL)
            {
                if (!crntPtr->data.forked)
                {

                    int forkTime = (int) getClk();
                    int forking = fork();
                    if (forking == -1)
                    {
                        perror("Couldn't fork process. ");
                    }
                    else if (forking == 0)
                    {

                        crntPtr->data.remainingTime = crntPtr->data.runTime - quantum;
                        int totalTime = crntPtr->data.runTime - crntPtr->data.remainingTime;

                        char printString[100];
                        sprintf(printString," At time %d process %d started arr %d total %d remain %d wait %d", (int) forkTime, crntPtr->data.processID, crntPtr->data.arrivalTime, totalTime, crntPtr->data.remainingTime, waitingTime);

                        execl("./process.out", "process.out", NULL);
                    }
                    else
                    {

                        
                        crntPtr->data.remainingTime = crntPtr->data.runTime - quantum;
                        int totalTime = crntPtr->data.runTime - crntPtr->data.remainingTime;
                        char printString[100];
                        sprintf(printString, " At time %d process %d started arr %d total %d remain %d wait %d", (int) forkTime, crntPtr->data.processID, crntPtr->data.arrivalTime, totalTime, crntPtr->data.remainingTime, waitingTime);
                        crntPtr->data.forked = true;
                        crntPtr->data.forkID = forking;
                        while ( (int) getClk() < forkTime + quantum);
                        int forkID = crntPtr->data.forkID;
                        kill(forkID, SIGSTOP);
                        crntPtr->data.remainingTime = crntPtr->data.remainingTime  - quantum; 
                        printString[100];
                        sprintf(printString," At time %d process %d stopped arr %d total %d remain %d wait %d", (int) forkTime, crntPtr->data.processID, crntPtr->data.arrivalTime, totalTime, crntPtr->data.remainingTime, waitingTime);
                    }
                }
                else
                {                    
                    int totalTime = crntPtr->data.runTime - crntPtr->data.remainingTime;
                    int resumeTime = (int) getClk();
                    char printString[100];
                    sprintf(printString, " At time %d process %d started arr %d total %d remain %d wait %d", (int) resumeTime, crntPtr->data.processID, crntPtr->data.arrivalTime, totalTime, crntPtr->data.remainingTime, waitingTime);
                    kill(crntPtr->data.forkID, SIGCONT);
                    while ( (int) getClk() < resumeTime + quantum);
                    kill(crntPtr->data.forkID, SIGSTOP);
                    printString[100];
                    sprintf(printString, " At time %d process %d stopped arr %d total %d remain %d wait %d", (int) getClk(), crntPtr->data.processID, crntPtr->data.arrivalTime, totalTime, crntPtr->data.remainingTime, waitingTime);
                }
                int PCBRCV = msgrcv(processQueueID, &receivedProcess, sizeof(receivedProcess), processMType, IPC_NOWAIT);
                if (PCBRCV > 0) {
                    enqueue(currentProcesses, receivedProcess.pcb);
                }                
                crntPtr = crntPtr->next;
            }
        }
}

int recieveMSG(int ID)
{



    algoInfo receivedInfo;
    int MSGRCV = msgrcv(ID, &receivedInfo, sizeof(receivedInfo.info), algoMType, IPC_NOWAIT);
    
    if(MSGRCV == -1)
    {
        perror("Error recieving !!!!!!!!!");
    }
    else
    {
         return (receivedInfo.info);
    }  

}

void createMessageQueue(int *msgID,int ID)
{

    (*msgID) = msgget((ID),IPC_CREAT | 0666 );
   
   if((*msgID)==-1)
    {
        perror("Error in create");
    }

}


