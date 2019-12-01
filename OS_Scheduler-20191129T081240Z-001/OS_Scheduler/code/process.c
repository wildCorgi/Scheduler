#include "headers.h"

/* Modify this file as needed*/
int remainingtime;
int main(int agrc, char * argv[])
{
    initClk();
    
    //TODO it needs to get the remaining time from somewhere
    //remainingtime = ??;
    remainingtime=stoi(argv[1]);
    printf("A7A %d\n", remainingtime);
    int lastclk=getClk;
    while (remainingtime > 0)
    {
        if(lastclk!=getClk()){
            remainingtime = remainingtime-1;
            lastclk=getClk();
            }
        printf("not A7A %d\n", remainingtime);
    }
    
    destroyClk(false);
    
    return 0;
}
