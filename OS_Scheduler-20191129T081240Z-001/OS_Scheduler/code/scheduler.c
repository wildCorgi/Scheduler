
#include "queue.c"

void processDone(int);
void processKilled(int);
pid_t pid;
int x;
int algo;
int processesNumber;
int quantum ;
int finishedProcesses =0;
int PCBRCV;
struct PCB  temp;
FILE *outputFile;
queue * pq;
msgPBuff receivedInfo;


void createMessageQueue(int *msgID,int ID);

void doRR(int quantum,int algo, int processID);

void doHPF(int processID,int finishedProcesses,int processesNumber);
void doSRTN()

int main(int argc, char *argv[])
{
    signal(SIGUSR1,processDone);
    signal(SIGCHLD,processKilled);
    pq = createQueue();

     
    //TODO implement the scheduler :)
    //upon termination release the clock resources

    int processID;
    createMessageQueue(&processID, processQueueID);
    
    algo = atoi(argv[1]);
    quantum = atoi(argv[2]); 
    processesNumber = atoi(argv[3]);


    initClk();
    

    if (algo == 1)
    { 
        // implement HPF
      
      

      while(true)
      {

            PCBRCV = msgrcv(processID, &receivedInfo, sizeof(receivedInfo.pcb), processMType, !IPC_NOWAIT);;

            if(receivedInfo.pcb.processID != recievingDone)
            {
     
                    priorityEnqueue(pq,receivedInfo.pcb);
  
            }
            else
            {
                break;
            } 
            
      }
       doHPF(processID,finishedProcesses,processesNumber);
    }
   




    else if (algo == 2)
    { 
        //implement SRTN here
             

      while(true)
      {

            PCBRCV = msgrcv(processID, &receivedInfo, sizeof(receivedInfo.pcb), processMType, !IPC_NOWAIT);;

            if(receivedInfo.pcb.processID != recievingDone)
            {
     
                    priorityTEnqueue(pq,receivedInfo.pcb);
  
            }
            else
            {
                break;
            } 
            
      }
       doSRTN(processID,finishedProcesses,processesNumber);

      
    }





   
    else
    { 
       //implement RR
       doRR(quantum,algo, processID);
    }
     

    destroyClk(true);
    raise(SIGKILL);
    
}






void doHPF(int processID,int finishedProcesses,int processesNumber)
{ 
    
   
    outputFile = fopen("./output.txt", "a");
    char outputString[] = "#At time x process y state arr w total z remain y wait k\n";
    fwrite(outputString, 1, strlen(outputString), outputFile);
    fclose(outputFile);
    x = getClk();
    while(finishedProcesses!=processesNumber)
    {


        temp = (pq->front->data)  ;   
        dequeue(pq);  
      
     

        pid = fork();

        if (pid == -1)
        {
            perror("Error in fork!!");
        }

        else if (pid == 0)
        { 
            
            temp.startTime = x;
            temp.waitingTime = getClk()-temp.arrivalTime;
            temp.remainingTime = temp.runTime;
            temp.finishTime = temp.runTime+temp.startTime;
            char printString[120];
            char str[64];
            sprintf(str, "%d", temp.remainingTime);
            outputFile = fopen("./output.txt", "a");
            sprintf(printString,"At time %d process %d started arr %d total %d remain %d wait %d\n",  x, temp.processID, temp.arrivalTime ,temp.runTime ,temp.remainingTime, temp.waitingTime); 
            fwrite(printString, sizeof(char), strlen(printString), outputFile);
            fclose(outputFile);
            execl("./process.out", "process.out ",str,NULL);

        }
        



        while(true)
        {

            PCBRCV = msgrcv(processID, &receivedInfo, sizeof(receivedInfo.pcb), processMType, !IPC_NOWAIT);


            if(PCBRCV==-1)
            {
                finishedProcesses++;
                break;
            }
            else
            {
               priorityEnqueue(pq,receivedInfo.pcb);
            } 
        } 

    }

}





void doRR(int quantum,int algo, int  processID)
{

}
 





void processDone(int signum)
{

                if(algo == 1)
                {

                x = getClk();
                temp.remainingTime = temp.runTime;
                temp.TA = x - temp.arrivalTime;
                temp.WTA = ((double)(temp.TA)/(double)(temp.runTime)) ;
                temp.waitingTime = getClk()-temp.runTime;
                char printString[200];
                outputFile = fopen("./output.txt", "a");
                sprintf(printString,"At time %d process %d finished arr %d total %d remain %d wait %d TA %d WTA %.2f\n",x, temp.processID, temp.arrivalTime, temp.runTime,0, temp.waitingTime,temp.TA,temp.WTA); 
                fwrite(printString, sizeof(char), strlen(printString), outputFile);
                fclose(outputFile);  
                }

}
void processKilled(int signnum)
{   
  
        kill(pid,SIGKILL);
}


void createMessageQueue(int *msgID,int ID)
{

    (*msgID) = msgget((ID),IPC_CREAT | 0666 );
   
   if((*msgID)==-1)
    {
        perror("Error in create");
    }

}


