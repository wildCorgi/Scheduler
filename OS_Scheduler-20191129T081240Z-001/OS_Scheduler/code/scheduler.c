
#include "queue.c"


void createMessageQueue(int *msgID,int ID);
int  recieveMSG(int ID);
PCB receiveProcess(int processID, int* PCBRCV);
void doRR(int quantum,int algo, int processID);
void doHPF(int processID);
int main(int argc, char *argv[])
{
    
    int algo =0;
    initClk();
 
    //TODO implement the scheduler :)
    //upon termination release the clock resources

    int algoQueue;
    int processID;
    createMessageQueue(&algoQueue,algoQueueID);   
    createMessageQueue(&processID, processQueueID);
    algo = recieveMSG(algoQueue);
   

    int quantum;
    
    
   
   
    if (algo == 0)
    { 
        // implement HPF
       printf("The algo number is: %d\n",algo);
       doHPF(processID);
    }
   
    else if (algo == 1)
    { //implement SRTN here
        queue * currentProcesses = createQueue();
        msgPBuff receivedProcess;
        //PCB *crntptr;
        node * excutedtPtr = NULL;
        while (1)
        {
            int PCBRCV;
        fml:
            PCBRCV = 1;
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
                        time = getClk();
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
                    time = getClk();
                }
            }
        }
    }
   
    else
    { 
       //implement RR
        
       printf("The algo number is: %d \n",algo);
       doRR(quantum,algo, processID);
    }
 
    destroyClk(true);
}






void doHPF(int processID)
{
    int inActive = 0;
    FILE *outputFile;
    outputFile = fopen("./output.txt", "a");
    char outputString[] = "#At time x process y state arr w total z remain y wait k\n";
    fwrite(outputString, 1, strlen(outputString), outputFile);
    fclose(outputFile);
    int rc;
    int num_messages =1;
    struct msqid_ds buf;
    int totalTime = 0;  
    int waitingTime = 0;
    int pid;
    int x;
    node * root;
    node * dataNode;
    PCB data;
    int rec_val = 0;
    queue * currentProcesses = createQueue();
    while(true)
    {
        x = getClk();
        rc = msgctl(processID, IPC_STAT, &buf);
        num_messages = buf.msg_qnum;
        rec_val=0;
        while(num_messages>0 )
        {
              

            
            rc = msgctl(processID, IPC_STAT, &buf);
            num_messages = buf.msg_qnum;
            printf("The messages in the queue is : %d\n",num_messages);
            if(num_messages == 0)
            { 
                   break;
            }
            PCB rec = receiveProcess(processID, &rec_val);
            if(rec_val!=-1)
            {
                enqueue(currentProcesses, rec);
                inActive++;
                write(1,"Enqueued!!\n",strlen("Enqueued!!\n"));
            }
        }

        //write(1,"checkpoint1\n",12);
        //printf("the cp count is: %d\n",currentProcesses->count);
        dataNode = currentProcesses->front; 
        x = getClk();
        if(dataNode==NULL)
        {
              // write(sizeof(char),"bnull y afal7\n",strlen("bnull y afal7"));
        } 
        for(int i=0;i<currentProcesses->count;i++)
        {

           //write(1,"checkpoint2\n",12);
           // printf("the i is: %d\n",i);
           //printf("the cp count is: %d\n",currentProcesses->count);
            x = getClk();
            dataNode = findNode(currentProcesses,i);          
            data = dataNode->data;
            if(dataNode==NULL)
                continue;
            //printf("the arrival time is: %d\n",dataNode->data.arrivalTime);
            //printf("the current time is: %d\n",x);
          
            if(data.arrivalTime <= x && data.forked ==  false)
            {
                write(1,"checkpoint3\n",12);

                pid = fork();
            
                if (pid == -1)
                {
                    perror("Error in fork!!");
                }
                else if (pid == 0)
                { 
                    inActive--;
                    printf("sha3'al el7 \n");
                    write(1,"checkpoint4\n",12);
                    data.forked = true;
                    data.remainingTime = data.runTime;
                    totalTime = data.runTime;
                    char printString[120];
                    char str[64];
                    sprintf(str, "%d", data.remainingTime);
                    printf("the remaining time is: %d\n",data.remainingTime);
                    outputFile = fopen("./output.txt", "a");
                    sprintf(printString,"At time %d process %d started arr %d total %d remain %d wait %d\n",  x, data.processID, data.arrivalTime, totalTime, data.remainingTime, waitingTime); 
                    fwrite(printString, sizeof(char), strlen(printString), outputFile);
                    fclose(outputFile);
                    execl("./process.out", "process.out ",str,NULL);
                }
                else 
                {
                    data.pid=pid;
                }
              
                

            }
            else
            {
                write(sizeof(char),"A7A bgdddd!!!!",strlen("A7A bgdddd!!!!"));
            }    

        }

    }

   

}





void doRR(int quantum,int algo, int  processID)
{
        quantum = algo - 2;
        queue *currentProcesses = createQueue();
        FILE *outputFile;
        outputFile = fopen("./output.txt", "a");
        char outputString[] = "#At time x process y state arr w total z remain y wait k \n";
        fwrite(outputString, 1, sizeof(outputString), outputFile);
        printf("passed a checkpoint. \n");
        int waitingTime = 0;
        int counter = 0;
        int PCBRCV;

        while (counter!=2)
        {
            printf("\nThe algo number is: %d \n",algo);
            PCB rec = receiveProcess(processID, &PCBRCV);
            
            if (PCBRCV > 0)
            {
                enqueue(currentProcesses, rec);
                printf("Enqueued correctly \n");
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
                rec = receiveProcess(processID, &PCBRCV);
                if (PCBRCV > 0) {
                    enqueue(currentProcesses, rec);
                
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

 PCB receiveProcess(int processID, int * PCBRCV)
{

    msgPBuff receivedInfo;
     (*PCBRCV) = msgrcv(processID, &receivedInfo, sizeof(receivedInfo.pcb), processMType, IPC_NOWAIT);
    
    if((*PCBRCV) == -1)
    {
        perror("Error recieving !!!!!!!!!");
    }
    else
    {
        return (receivedInfo.pcb);
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


