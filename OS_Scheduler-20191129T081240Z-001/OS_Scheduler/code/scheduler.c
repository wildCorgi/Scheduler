
#include "queue.c"
#include <time.h>
void processDone(int);
void processKilled(int);
pid_t pid;
int x,y;
int algo;
int processesNumber;
int quantum ;
int finishedProcesses =0;
int PCBRCV;
int processID;
struct PCB  temp;
FILE *outputFile;
queue * pq;
msgPBuff receivedInfo;


void createMessageQueue(int *msgID,int ID);

void doRR();
void doHPF();
void doSRTN();

int main(int argc, char *argv[])
{
    signal(SIGUSR1,processDone);
    signal(SIGCHLD,processKilled);
    pq = createQueue();

     
    //TODO implement the scheduler :)
    //upon termination release the clock resources


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
       doHPF();
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
       doSRTN();

      
    }
    else
    { 
       //implement RR
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
       doRR();
    }
     

    destroyClk(true);
    raise(SIGKILL);
    
}






void doHPF()
{ 
    
    outputFile = fopen("./output.txt", "a");
    char outputString[] = "#At time x process y state arr w total z remain y wait k\n";
    fwrite(outputString, 1, strlen(outputString), outputFile);
    fclose(outputFile);
    x = getClk();
    while(finishedProcesses!=processesNumber)
    {
        x=getClk();
        if(pq->count != 0)
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
            
        }
   
            while(true)
            {

                PCBRCV = msgrcv(processID, &receivedInfo, sizeof(receivedInfo.pcb), processMType, !IPC_NOWAIT);


                if(PCBRCV==-1)
                {
                    break;
                }
                else
                {
                    if(temp.state == stateFinished && receivedInfo.pcb.processID==recievingDone) 
                        break;
                    else if(receivedInfo.pcb.processID!=recievingDone)
                          priorityEnqueue(pq,receivedInfo.pcb);
                
                } 
            } 
        
    }
}

void doSRTN()
{ 
    
   
    outputFile = fopen("./output.txt", "a");
    char outputString[] = "#At time x process y state arr w total z remain y wait k\n";
    fwrite(outputString, 1, strlen(outputString), outputFile);
    fclose(outputFile);
    x = getClk();
    while(finishedProcesses!=processesNumber)
    {


                temp = (pq->front->data)  ;   
                x = getClk();
                if(!temp.forked)
                {   
                    temp.forked = true;
                    pid = fork();
                    if (pid == -1)
                    {
                        perror("Error in fork!!");
                    }

                    else if (pid == 0)
                    { 

                        temp.startTime = x;
                        temp.state = stateStarted;
                        temp.waitingTime = x-temp.arrivalTime;                
                        char printString[120];
                        char str[64];
                        sprintf(str, "%d", temp.remainingTime);
                        outputFile = fopen("./output.txt", "a");
                        sprintf(printString,"At time %d process %d started arr %d total %d remain %d wait %d\n",  x, temp.processID, temp.arrivalTime ,temp.runTime ,temp.remainingTime, temp.waitingTime); 
                        fwrite(printString, sizeof(char), strlen(printString), outputFile);
                        fclose(outputFile);
                        execl("./process.out", "process.out ",str,NULL);

                    }
                    temp.forkID = pid;
                }
                else
                {
                    temp.state = stateResumed;
                    temp.waitingTime += getClk() - temp.lastStoppedTime;
                    kill(temp.forkID,SIGCONT);                    
                }
                

                
                while(true)
                {
 
                        PCBRCV = msgrcv(processID, &receivedInfo, sizeof(receivedInfo.pcb), processMType, !IPC_NOWAIT);
                        y = getClk();
                        temp.remainingTime= temp.remainingTime - (y-x);



                        if(PCBRCV==-1)
                        {
                            break;
                        }
                        else
                        {

                            if(temp.remainingTime <= receivedInfo.pcb.remainingTime && receivedInfo.pcb.processID!=recievingDone)      
                                priorityTEnqueue(pq,receivedInfo.pcb); 
                            else
                            {
                                if(receivedInfo.pcb.processID==recievingDone) 
                                {
                                temp.remainingTime= temp.remainingTime - (y-x);
                                kill(temp.forkID,SIGSTOP);
                                temp.state = stateStopped;
                                temp.lastStoppedTime = getClk();
                                break;
                                }
                          
                               
                                priorityTEnqueue(pq,receivedInfo.pcb); 
                            }
                        }
                        
                    
                    
                    
                }
        


        



    }

}




void doRR()
{



    outputFile = fopen("./output.txt", "a");
    char outputString[] = "#At time x process y state arr w total z remain y wait k\n";
    fwrite(outputString, 1, strlen(outputString), outputFile);
    fclose(outputFile);
    x = getClk();
    while(finishedProcesses!=processesNumber)
    {


                temp = (pq->front->data)  ;   
                x = getClk();
                if(!temp.forked)
                {   
                    temp.forked = true;
                    pid = fork();
                    if (pid == -1)
                    {
                        perror("Error in fork!!");
                    }

                    else if (pid == 0)
                    { 

                        temp.startTime = x;
                        temp.state = stateStarted;
                        temp.waitingTime = x-temp.arrivalTime;                
                        char printString[120];
                        char str[64];
                        sprintf(str, "%d", temp.remainingTime);
                        outputFile = fopen("./output.txt", "a");
                        sprintf(printString,"At time %d process %d started arr %d total %d remain %d wait %d\n",  x, temp.processID, temp.arrivalTime ,temp.runTime ,temp.remainingTime, temp.waitingTime); 
                        fwrite(printString, sizeof(char), strlen(printString), outputFile);
                        fclose(outputFile);
                        if(quantum>temp.remainingTime)
                           alarm(quantum);

                        execl("./process.out", "process.out ",str,NULL);

                    }
                    temp.forkID = pid;
                }
                else
                {
                    temp.state = stateResumed;
                    temp.waitingTime += getClk() - temp.lastStoppedTime;
                    if(quantum>temp.remainingTime)
                        alarm(quantum);

                    kill(temp.forkID,SIGCONT);                    
                }
                

                
                while(true)
                {
 
                        PCBRCV = msgrcv(processID, &receivedInfo, sizeof(receivedInfo.pcb), processMType, !IPC_NOWAIT);
                        y = getClk();
                        temp.remainingTime= temp.remainingTime - (y-x);



                        if(PCBRCV==-1)
                        {
                            break;
                        }
                        else
                        {

                            if(temp.remainingTime <= receivedInfo.pcb.remainingTime && receivedInfo.pcb.processID!=recievingDone)      
                                priorityTEnqueue(pq,receivedInfo.pcb); 
                            else
                            {
                                if(receivedInfo.pcb.processID==recievingDone) 
                                {
                                temp.remainingTime= temp.remainingTime - (y-x);
                                kill(temp.forkID,SIGSTOP);
                                temp.state = stateStopped;
                                temp.lastStoppedTime = getClk();
                                break;
                                }
                          
                               
                                priorityTEnqueue(pq,receivedInfo.pcb); 
                            }
                        }
                        
                    
                    
                    
                }
        


        



    }



}
 





void processDone(int signum)
{

                if(algo == 1)
                {
                    finishedProcesses++;
                    temp.state = stateFinished;
                    y = getClk();
                    temp.finishTime = getClk();
                    temp.remainingTime = 0;
                    temp.TA = y - temp.arrivalTime;
                    temp.WTA = ((double)(temp.TA)/(double)(temp.runTime)) ;
                    temp.waitingTime = y-temp.runTime + (y-x-temp.runTime);
                    char printString[200];
                    outputFile = fopen("./output.txt", "a");
                    sprintf(printString,"At time %d process %d finished arr %d total %d remain %d wait %d TA %d WTA %.2f\n",y, temp.processID, temp.arrivalTime, temp.runTime,0, temp.waitingTime,temp.TA,temp.WTA); 
                    fwrite(printString, sizeof(char), strlen(printString), outputFile);
                    fclose(outputFile); 

                }
                else if(algo == 2)
                {

                    finishedProcesses++;
                    temp.state = stateFinished;
                    y = getClk();
                    temp.finishTime = getClk();
                    temp.remainingTime = 0;
                    temp.TA = y - temp.arrivalTime;
                    temp.WTA = ((double)(temp.TA)/(double)(temp.runTime)) ;
                    temp.waitingTime = y-temp.arrivalTime - temp.runTime;
                    char printString[200];
                    outputFile = fopen("./output.txt", "a");
                    sprintf(printString,"At time %d process %d finished arr %d total %d remain %d wait %d TA %d WTA %.2f\n",y, temp.processID, temp.arrivalTime, temp.runTime,0, temp.waitingTime,temp.TA,temp.WTA); 
                    fwrite(printString, sizeof(char), strlen(printString), outputFile);
                    fclose(outputFile); 
                    dequeue(pq);
                }
                else if(algo ==3)
                {

                    finishedProcesses++;
                    temp.state = stateFinished;
                    y = getClk();
                    temp.finishTime = getClk();
                    temp.remainingTime = 0;
                    temp.TA = y - temp.arrivalTime;
                    temp.WTA = ((double)(temp.TA)/(double)(temp.runTime)) ;
                    temp.waitingTime = y-temp.arrivalTime - temp.runTime;
                    char printString[200];
                    outputFile = fopen("./output.txt", "a");
                    sprintf(printString,"At time %d process %d finished arr %d total %d remain %d wait %d TA %d WTA %.2f\n",y, temp.processID, temp.arrivalTime, temp.runTime,0, temp.waitingTime,temp.TA,temp.WTA); 
                    fwrite(printString, sizeof(char), strlen(printString), outputFile);
                    fclose(outputFile); 
                    dequeue(pq);
                }
                    signal(SIGUSR1,processDone);
                    signal(SIGCHLD,processKilled);

}
void processKilled(int signnum)
{   
  
        kill(pid,SIGKILL);
                            signal(SIGUSR1,processDone);
                    signal(SIGCHLD,processKilled);
}


void createMessageQueue(int *msgID,int ID)
{

    (*msgID) = msgget((ID),IPC_CREAT | 0666 );
   
   if((*msgID)==-1)
    {
        perror("Error in create");
    }

}


