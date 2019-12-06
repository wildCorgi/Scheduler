
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
    }
    else
    { //implement RR
        quantum = algo - 1;
        queue *currentProcesses = createQueue();
        msgPBuff receivedProcess;
        while (1)
        {
            int PCBRCV = msgrcv(processQueueID, &receivedProcess, sizeof(receivedProcess), processMType, IPC_NOWAIT);
            //PCB *crntptr;
            if (PCBRCV > 0) {
                enqueue(currentProcesses, receivedProcess.pcb);
            }
            node * crntPtr = currentProcesses->front;
            while (crntPtr != NULL)
            {
                if (!crntPtr->data.forked)
                {
                    int forkTime = getClk();
                    int forking = fork();
                    if (forking == -1)
                    {
                        perror("Couldn't fork process. ");
                    }
                    else if(forking == 0){
                        execl("./process.out","process.out",NULL);
                    }
                    else if (forking > 1)
                    {
                        crntPtr->data.forked = true;
                        crntPtr->data.forkID = forking;
                        while(getClk() < forkTime + 2);
                        int pid = crntPtr->data.forkID; 
                        kill(pid, SIGSTOP);
                    }
                }
                else{
                    int pizzaTime = getClk();
                    kill(crntPtr->data.forkID, SIGCONT);
                    while(getClk() < pizzaTime + 2);
                    kill(crntPtr->data.forkID, SIGSTOP);
                }
                crntPtr = crntPtr->next;
            }
        }
    }
    }
    destroyClk(true);
}
