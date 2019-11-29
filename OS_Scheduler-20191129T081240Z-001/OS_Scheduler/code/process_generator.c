
#include "queue.c"

void clearResources(int);
void readFile(queue * pcbs);
void createMessageQueue(int *msgID,int ID);
void sendProcess(struct PCB  pc,int processMsgID);

int main(int argc, char * argv[])
{
    //signal(SIGINT, clearResources);
    
    // TODO Initialization
    int schedulingAlgo=0;
    int quantum = 0;
    int processMsgID,algoMsgID;
    int pid;
    // 1. Read the input files.
    createMessageQueue(&processMsgID,processQueueID);
    createMessageQueue(&algoMsgID,algoQueueID); 
    queue * pcbs = createQueue();   
    readFile(pcbs);
    
    
    // 2. Ask the user for the chosen scheduling algorithm and its parameters, if there are any.
    retake:
    printf("\nEnter the wanted scheduling algorithm:\n1.HPF\n2.SRTN\n3.RR\n");
    scanf("%d",&schedulingAlgo);
    if(schedulingAlgo<1 || schedulingAlgo>3)
    goto retake;
    if(schedulingAlgo == 3)
    {
        printf("\nEnter the wanted Quantum:\n");
        scanf("%d",&quantum);
    }    
    schedulingAlgo += quantum-1;
  
    // 3. Initiate and create the scheduler and clock processes.
    pid = fork();

    if(pid==-1)
    {
        perror("Error in fork!!");
    }
    else if (pid == 0)
    {
        execl("./clk.out","clk.out",NULL);
    }

 /*    pid = fork();
    
    if(pid==-1)
    {
        perror("Error in fork!!");
    }
    else if (pid == 0)
    {
       // execl("./scheduler.out","scheduler.out",NULL);
    }
  */ 

  

    
    // 4. Use this function after creating the clock process to initialize clock

    initClk();
    while(1)
    {      
        int x = getClk();
        
        printf("A7A %d\n",pcbs->front->data.arrivalTime);
       
        if(pcbs->front!=NULL)
        {
                 printf("%d\n",x);
        while(pcbs->front->data.arrivalTime == x)
        {
          printf("NOT A7A %d\n",x);
    
         struct PCB  temp = dequeue(pcbs)->data ;    
         sendProcess(temp,processMsgID);      
     
         if(pcbs->front==NULL)
         break;
        
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

void sendProcess(struct PCB  pcb,int processMsgID)
{
    int send_val;
    struct msgPBuff temp;
    temp.pcb  = pcb;
    temp.mtype = processMType;
    send_val = msgsnd(processMsgID, &temp, sizeof(temp), !IPC_NOWAIT);
    if(send_val ==-1)
           perror("Errror in Sending Process");
}


void createMessageQueue(int *msgID,int ID)
{
(*msgID)= msgget((ID),IPC_CREAT | 0666 );
}


void readFile(queue * pcbs)
{

    int info[4];
    FILE * fp; 
    fp = fopen("Input.txt", "r");
    char line[256];
    while(fgets(line, sizeof(line), fp) != NULL)
    {
        if(line[0] != '#')
        {
            sscanf(line,"%d\t%d\t%d\t%d", &info[0], &info[1],&info[2], &info[3]);
            printf("%d\t%d\t%d\t%d\n",info[0], info[1],info[2],info[3]);
            struct PCB temp;
            temp.processID = info[0];
            temp.arrivalTime = info[1];
            temp.runTime = info[2];
            temp.priority = info[3];
            enqueue(pcbs,temp);
        }
        else
            continue; 
    }
    fclose(fp);


}