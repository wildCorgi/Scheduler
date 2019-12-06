
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
    { 
        //implement SRTN here
        printf("The algo number is: %d",algo);
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
        msgPBuff receivedProcess;
        while (1)
        {
            printf("\nThe algo number is: %d \n",algo);
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


