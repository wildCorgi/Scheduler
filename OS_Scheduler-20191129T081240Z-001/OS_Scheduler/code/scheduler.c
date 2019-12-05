#include "headers.h"


int main(int argc, char * argv[])
{
    
    initClk();
    
    //TODO implement the scheduler :)
    //upon termination release the clock resources
    int algoQueue = msgget(algoQueueID, IPC_CREAT | 0644);
    algoInfo receivedInfo;
    int MSGRCV = msgrcv(algoQueueID, &receivedInfo, sizeof(receivedInfo.info),300, !IPC_NOWAIT);
 
    int quantum;
    int algo = receivedInfo.info;
    if(algo == 0){ // implement HPF
    }
    else if (algo == 1){ //implement SRTN here
    }
    else{ //implement RR 
        quantum = algo - 1;
        
    }
    destroyClk(true);
}
