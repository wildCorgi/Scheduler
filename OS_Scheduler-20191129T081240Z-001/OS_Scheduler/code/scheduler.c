#include "queue.c"
#include <time.h>
#include <math.h>
void processDone(int);
void alarmREC(int);
pid_t pid;
int x = 0 , y = 0;
int algo;
int processesNumber;
int quantum ;
int lastT;
int finishedProcesses =0;
int PCBRCV;
bool smallerRecieved;
int processID;
int totalRTime = 0;
double TWTA = 0;
int totalWaiting = 0;
double deviation =0;
node * looper;
struct PCB  temp;
FILE *outputFile;
FILE *outputFile1;
queue * pq;
nqueue * npq;
msgPBuff receivedInfo;


void createMessageQueue(int *msgID,int ID);
void writeStarting();
void writeStartState();
void writeFinishState();
void writeResumeState();
void writeFinalState();
void setStartState();
void setResumeState();
void setFinishState();
void firstRecieve();
void firstRecieveSRTN();
void firstRecieveRR();
void secondRecieve();
void secondRecievePr();
void recieveRR();


void doHPF();
void doSRTN();
void doRR();

int main(int argc, char *argv[])
{
            signal(SIGUSR1,processDone);
            signal(SIGCHLD,SIG_IGN);
            signal(SIGALRM,alarmREC);
            pq = createQueue();
            npq = createNQueue();
            
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
                firstRecieve();
                doHPF();
            }
            else if (algo == 2)
            { 
                //implement SRTN here     
                firstRecieveSRTN();
                doSRTN();
            }
            else
            { 
            //implement RR
                firstRecieveRR();
                doRR();
            }
            
            writeFinalState();
            destroyClk(true);
            raise(SIGKILL);
    
}






void doHPF()
{ 
        writeStarting();
    
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
                
                setStartState();
                char str[64];
                sprintf(str, "%d", temp.remainingTime);
                writeStartState();
                execl("./process.out", "process.out ",str,NULL);

            }
                setStartState();
                temp.forkID = pid;
        }
                secondRecieve();
    }
}

void doSRTN()
{ 
    
   
    writeStarting();

    while(finishedProcesses!=processesNumber)
    {
            smallerRecieved = false;
            x = getClk();

            if(pq->count != 0 )    
            {  
                temp = (pq->front->data)  ;   
                dequeue(pq);

                
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
                        setStartState();
                        char str[64];
                        sprintf(str, "%d", temp.remainingTime);
                        writeStartState();
                        execl("./process.out", "process.out ",str,NULL);

                    }
                        setStartState();    
                        temp.forkID = pid;                
                }
                else
                {

                    setResumeState();
                    writeResumeState();
                    kill(temp.forkID,SIGCONT);                    
                }

                
            }   
                        
            secondRecievePr();
        
    }

}




void doRR()
{



    writeStarting();
   
    while(finishedProcesses!=processesNumber)
    {
            x = getClk();

            if(pq->count != 0)    
            {  

                temp = (pq->front->data)  ;   
                dequeue(pq);
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

                        setStartState();
                        char str[64];
                        sprintf(str, "%d", temp.remainingTime);
                        writeStartState();
                        execl("./process.out", "process.out ",str,NULL);
                    }

                    setStartState();
                    if(quantum < temp.remainingTime)
                    { 
                        alarm(quantum);
                    }
                    temp.forkID = pid;
                }
                else
                {
                    setResumeState();
                    writeResumeState();
                    if(quantum < temp.remainingTime)
                    { 
                        alarm(quantum);
                    }
                    kill(temp.forkID,SIGCONT);                    
                }
            }    

            recieveRR();    


    }



}
 


void processDone(int signum)
{

                
           
                      
                    setFinishState();
                    writeFinishState();
                    signal(SIGUSR1,processDone);    

}








void createMessageQueue(int *msgID,int ID)
{

    (*msgID) = msgget((ID),IPC_CREAT | 0666 );
   
   if((*msgID)==-1)
    {
        perror("Error in create");
    }

}




void writeStarting()
{
    outputFile = fopen("./scheduler.log", "a");
    char outputString[] = "#At time x process y state arr w total z remain y wait k\n";
    fwrite(outputString, 1, strlen(outputString), outputFile);
    fclose(outputFile);
}
void writeStartState()
{
        char printString[120];
       
        outputFile = fopen("./scheduler.log", "a");
        sprintf(printString,"At time %d process %d started arr %d total %d remain %d wait %d\n",  y, temp.processID, temp.arrivalTime ,temp.runTime ,temp.remainingTime, temp.waitingTime); 
        fwrite(printString, sizeof(char), strlen(printString), outputFile);
        fclose(outputFile);

}
void writeFinishState()
{
                    char printString[200];
                    outputFile = fopen("./scheduler.log", "a");
                    sprintf(printString,"At time %d process %d finished arr %d total %d remain %d wait %d TA %d WTA %.2f\n",y, temp.processID, temp.arrivalTime, temp.runTime,0, temp.waitingTime,temp.TA,temp.WTA); 
                    fwrite(printString, sizeof(char), strlen(printString), outputFile);
                    fclose(outputFile); 

}
void writePauseState()
{
                    char printString[200];
                    outputFile = fopen("./scheduler.log", "a");
                    sprintf(printString,"At time %d process %d paused arr %d total %d remain %d wait %d\n",y, temp.processID, temp.arrivalTime, temp.runTime,temp.remainingTime, temp.waitingTime); 
                    fwrite(printString, sizeof(char), strlen(printString), outputFile);
                    fclose(outputFile); 
                                    
}
void writeResumeState()
{
                    char printString[200];
                    outputFile = fopen("./scheduler.log", "a");
                    sprintf(printString,"At time %d process %d resumed arr %d total %d remain %d wait %d\n",y, temp.processID, temp.arrivalTime, temp.runTime,temp.remainingTime, temp.waitingTime); 
                    fwrite(printString, sizeof(char), strlen(printString), outputFile);
                    fclose(outputFile); 
                                    
}
void writeFinalState()
{
  
                    double AWTA = (double)(TWTA/processesNumber);
                    y = getClk();
                    int count = npq->count;

                    for(int i = 0 ; i<count;i++)
                    {
                            deviation += pow((dequeueN(npq) - (AWTA)),2);
                            printf("deviation is %.2f\n",deviation);
                    }
                    deviation = sqrt(deviation/count);
                    char printString[200];
                    outputFile1 = fopen("./scheduler.perf", "a");
                    sprintf(printString,"CPU Utilization = %.2f %% \nAvg WTA = %.2f\nAvg Waiting = %.2f\nStd WTA = %.2f \n",((double)totalRTime/(double)y)*100,AWTA,((double)totalWaiting/(double)processesNumber),deviation); 
                    fwrite(printString, sizeof(char), strlen(printString), outputFile1);
                    fclose(outputFile1); 
                                    
}
void setStartState()
{
                    y = getClk();
                    lastT=y;
                    totalRTime += temp.runTime;
                    temp.state = stateStarted;                   
                    temp.startTime = y;
                    temp.waitingTime = y-temp.arrivalTime;
                    temp.remainingTime = temp.runTime;
                    temp.finishTime = 0;
                    temp.lastStoppedTime = y;
}
void setFinishState()
{
                    y = getClk();
                    finishedProcesses++;
                    temp.state = stateFinished;
                    temp.finishTime = y;
                    temp.remainingTime = 0;
                    temp.TA = y - temp.arrivalTime;
                    temp.WTA = ((double)(temp.TA)/(double)(temp.runTime)) ;
                    TWTA+=temp.WTA;
                    //temp.waitingTime = y-temp.lastStoppedTime-temp.runTime;
                    temp.waitingTime=y-temp.arrivalTime-temp.runTime;
                    totalWaiting += temp.waitingTime;
                    enqueueN(npq,temp.WTA);
}
void setPauseState()
{    
                    y = getClk();
                    temp.state = stateStopped;
                    //temp.remainingTime -= (y-(temp.lastStoppedTime));
                    temp.lastStoppedTime = y;                  
}
void setResumeState()
{   
                    y=getClk();
                    temp.state = stateResumed;
                    temp.waitingTime += (y - temp.lastStoppedTime);
                    temp.lastStoppedTime = y;       
}
void firstRecieve()
{
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
}

void firstRecieveSRTN()
{
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
}
void firstRecieveRR()
{
        while(true)
        {
            PCBRCV = msgrcv(processID, &receivedInfo, sizeof(receivedInfo.pcb), processMType, !IPC_NOWAIT);;

            if(receivedInfo.pcb.processID != recievingDone)
            {
     
                    enqueue(pq,receivedInfo.pcb);
  
            }
            else
            {
                break;
            } 
        }
}
void secondRecieve()
{

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


void secondRecievePr()
{
                while(true)
                {

                        PCBRCV = msgrcv(processID, &receivedInfo, sizeof(receivedInfo.pcb), processMType, !IPC_NOWAIT);
                        
                        y=getClk();
                        if(y != lastT)
                        {                        
                        temp.remainingTime -= (y - lastT);
                        lastT = y;
                        }

                        if(PCBRCV==-1 )
                        {
                            
                         
                              break;
                         

                        }
                        else
                        {

                            if(temp.remainingTime <= receivedInfo.pcb.remainingTime && receivedInfo.pcb.processID!=recievingDone)      
                            {
                                    priorityTEnqueue(pq,receivedInfo.pcb); 
                            }
                            else if(temp.remainingTime > receivedInfo.pcb.remainingTime && receivedInfo.pcb.processID!=recievingDone)
                            {
                                smallerRecieved =true;
                                priorityTEnqueue(pq,receivedInfo.pcb);               
                            }
                            else if (receivedInfo.pcb.processID==recievingDone && smallerRecieved )
                            {
                                setPauseState();
                                writePauseState();
                                priorityTEnqueue(pq,temp);    
                                kill(temp.forkID,SIGSTOP);
                                break;
                            }
                            else if(temp.state == stateFinished ||temp.state == stateStopped)
                            {
                                break;
                            }
                        }
                        
                    
                    
                    
                }
        


}
void recieveRR()
{

                while(true)
                {
                       
                       
                        PCBRCV = msgrcv(processID, &receivedInfo, sizeof(receivedInfo.pcb), processMType, !IPC_NOWAIT);

                        if(PCBRCV==-1)
                        {
                            break;
                        }
                        else
                        {
                                if(receivedInfo.pcb.processID != recievingDone)
                                {
                                    enqueue(pq,receivedInfo.pcb);
                                }
                                else if(temp.state == stateFinished ||temp.state == stateStopped)
                                {
                                    break;
                                }

                        }   
                }
}
void alarmREC(int signnum)
{   

                            alarm(0);
                            setPauseState();
                            writePauseState();
                            enqueue(pq,temp);
                            kill(temp.forkID,SIGSTOP);  
                            signal(SIGALRM,alarmREC);
}


