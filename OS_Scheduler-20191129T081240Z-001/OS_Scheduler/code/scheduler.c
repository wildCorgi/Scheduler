
#include "queue.c"


void createMessageQueue(int *msgID,int ID);
int  recieveMSG(int ID);
PCB * receiveProcess(int processID, int* PCBRCV);

void doRR(int quantum,int algo, int processID);
void doHPF(int processID,int finishedProcesses,int processesNumber);
Node* pq ;
int main(int argc, char *argv[])
{
    PCB * pcbs = (PCB*)malloc(100 * sizeof(PCB)); 
    int processesNumber;
    int quantum ;
    int algo;
    int finishedProcesses =0;
    int PCBRCV;
    initClk();
 
    //TODO implement the scheduler :)
    //upon termination release the clock resources

    int processID;
    createMessageQueue(&processID, processQueueID);
    algo = atoi(argv[1]);
    quantum = atoi(argv[2]); 
    processesNumber = atoi(argv[3]);
    PCB  * temp;
    
    printf("The algo number is: %d\n",algo);
    printf("The quantum number is: %d\n",quantum);
    printf("The process number is: %d\n",processesNumber);
   
   
    if (algo == 1)
    { 
        // implement HPF

      msgPBuff receivedInfo;
      PCBRCV = msgrcv(processID, &receivedInfo, sizeof(receivedInfo.pcb), processMType, !IPC_NOWAIT);;
      temp = &receivedInfo.pcb;
     
      pq = newNode(temp, temp->priority); 
      while(PCBRCV != - 1)
      {
          temp = receiveProcess(processID,&PCBRCV);
          if(PCBRCV!=-1)
          {
            push(&pq,temp,temp->priority);
          }

      }
    

       doHPF(processID,finishedProcesses,processesNumber);
    }
   
    else if (algo == 2)
    { 
        //implement SRTN here
      
    }
   
    else
    { 
       //implement RR
       doRR(quantum,algo, processID);
    }
 
    destroyClk(true);
}






void doHPF(int processID,int finishedProcesses,int processesNumber)
{
    PCB * temp;
    FILE *outputFile;
    outputFile = fopen("./output.txt", "a");
    char outputString[] = "#At time x process y state arr w total z remain y wait k\n";
    fwrite(outputString, 1, strlen(outputString), outputFile);
    fclose(outputFile);
    while(finishedProcesses!=processesNumber)
    {

        temp = peek(&pq);
        pop(&pq);
        
        printf("Process %d arrived with rem time = %d \n",temp->processID,temp->runTime);
    
        int pid = fork();

        if (pid == -1)
        {
            perror("Error in fork!!");
        }
        else if (pid == 0)
        { 
            int x = getClk();
            temp->forked = true;
            temp->remainingTime = temp->runTime;
            int waitingTime = x - temp->arrivalTime;
            int totalTime = temp->runTime;
            char printString[120];
            char str[64];
            sprintf(str, "%d", temp->remainingTime);
            outputFile = fopen("./output.txt", "a");
            sprintf(printString,"At time %d process %d started arr %d total %d remain %d wait %d\n",  x, temp->processID, temp->arrivalTime, totalTime, temp->remainingTime, waitingTime); 
            fwrite(printString, sizeof(char), strlen(printString), outputFile);
            fclose(outputFile);
            alarm(temp->remainingTime);
            execl("./process.out", "process.out ",str,NULL);
            
        }
        temp->pid = pid;
        int PCBRCV = 0;
        msgPBuff receivedInfo;

        while(PCBRCV !=-1)
        {

            PCBRCV = msgrcv(processID, &receivedInfo, sizeof(receivedInfo.pcb), processMType, !IPC_NOWAIT);
            
            if(PCBRCV != -1)
            {
            push(&pq,&receivedInfo.pcb,receivedInfo.pcb.priority);
            }
            if(PCBRCV==-1)
            {
                finishedProcesses++;
            }

        } 
    }

}





void doRR(int quantum,int algo, int  processID)
{
 //        int inActive = 0;
    // FILE *outputFile;
    // outputFile = fopen("./output.txt", "a");
    // char outputString[] = "#At time x process y state arr w total z remain y wait k\n";
    // fwrite(outputString, 1, strlen(outputString), outputFile);
    // fclose(outputFile);
    // int rc;
    // int num_messages =1;
    // struct msqid_ds buf;
    // int totalTime = 0;  
    // int waitingTime = 0;
    // int pid;
    // int x;
    // node * root;
    // node * dataNode;
    // PCB data;
    // int rec_val = 0;
    // queue * currentProcesses = createQueue();
    // while(true)
    // {
    //     x = getClk();
    //     rc = msgctl(processID, IPC_STAT, &buf);
    //     num_messages = buf.msg_qnum;
    //     rec_val=0;
    //     while(num_messages>0 )
    //     {
              

            
    //         rc = msgctl(processID, IPC_STAT, &buf);
    //         num_messages = buf.msg_qnum;
    //         printf("The messages in the queue is : %d\n",num_messages);
    //         if(num_messages == 0)
    //         { 
    //                break;
    //         }
    //         PCB rec = receiveProcess(processID, &rec_val);
    //         if(rec_val!=-1)
    //         {
    //             enqueue(currentProcesses, rec);
    //             inActive++;
    //             write(1,"Enqueued!!\n",strlen("Enqueued!!\n"));
    //         }
    //     }

    //     //write(1,"checkpoint1\n",12);
    //     //printf("the cp count is: %d\n",currentProcesses->count);
    //     dataNode = currentProcesses->front; 
    //     x = getClk();
    //     if(dataNode==NULL)
    //     {
    //           // write(sizeof(char),"bnull y afal7\n",strlen("bnull y afal7"));
    //     } 
    //     for(int i=0;i<currentProcesses->count;i++)
    //     {

    //        //write(1,"checkpoint2\n",12);
    //        // printf("the i is: %d\n",i);
    //        //printf("the cp count is: %d\n",currentProcesses->count);
    //         x = getClk();
    //         dataNode = findNode(currentProcesses,i);          
    //         data = dataNode->data;
    //         if(dataNode==NULL)
    //             continue;
    //         //printf("the arrival time is: %d\n",dataNode->data.arrivalTime);
    //         //printf("the current time is: %d\n",x);
          
    //         if(data.arrivalTime <= x && data.forked ==  false)
    //         {
    //             write(1,"checkpoint3\n",12);

    //             pid = fork();
            
    //             if (pid == -1)
    //             {
    //                 perror("Error in fork!!");
    //             }
    //             else if (pid == 0)
    //             { 
    //                 inActive--;
    //                 printf("sha3'al el7 \n");
    //                 write(1,"checkpoint4\n",12);
    //                 data.forked = true;
    //                 data.remainingTime = data.runTime;
    //                 totalTime = data.runTime;
    //                 char printString[120];
    //                 char str[64];
    //                 sprintf(str, "%d", data.remainingTime);
    //                 printf("the remaining time is: %d\n",data.remainingTime);
    //                 outputFile = fopen("./output.txt", "a");
    //                 sprintf(printString,"At time %d process %d started arr %d total %d remain %d wait %d\n",  x, data.processID, data.arrivalTime, totalTime, data.remainingTime, waitingTime); 
    //                 fwrite(printString, sizeof(char), strlen(printString), outputFile);
    //                 fclose(outputFile);
    //                 execl("./process.out", "process.out ",str,NULL);
    //             }
    //             else 
    //             {
    //                 data.pid=pid;
    //             }
              
                

    //         }
    //         else
    //         {
    //             write(sizeof(char),"A7A bgdddd!!!!",strlen("A7A bgdddd!!!!"));
    //         }    

    //     }

    // }
    
}
 




 PCB * receiveProcess(int processID, int * PCBRCV)
 {

    msgPBuff receivedInfo;
     (*PCBRCV) = msgrcv(processID, &receivedInfo, sizeof(receivedInfo.pcb), processMType, IPC_NOWAIT);
    
    PCB * temp = &receivedInfo.pcb;
    if((*PCBRCV)!=-1)   
    {
        return temp;
    }  
    else 
        return temp;

}

void createMessageQueue(int *msgID,int ID)
{

    (*msgID) = msgget((ID),IPC_CREAT | 0666 );
   
   if((*msgID)==-1)
    {
        perror("Error in create");
    }

}


