
#include "queue.c"

int main(int argc, char *argv[])
{
    
    initClk();
 
    //TODO implement the scheduler :)
    //upon termination release the clock resources
    int algoQueue = msgget(algoQueueID, IPC_CREAT | 0644);
    algoInfo receivedInfo;
    int MSGRCV = msgrcv(algoQueueID, &receivedInfo, sizeof(receivedInfo.info), algoMType, !IPC_NOWAIT);

    int quantum;
    int algo = receivedInfo.info;
    while(true)
    {
    if (algo == 0)
    { // implement HPF
    }
    else if (algo == 1)
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
    }
    else
    { //implement RR
        quantum = algo - 1;
        queue *currentProcesses = createQueue();
        FILE *outputFile;
        outputFile = fopen("./output.txt", "a");
        msgPBuff receivedProcess;
        int waitingTime = 0;
        while (1)
        {
            char outputString[] = "#At time x process y state arr w total z remain y wait k \n";
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
    }
    destroyClk(true);
}
