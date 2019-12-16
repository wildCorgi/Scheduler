#include "headers.h"
#include <time.h>

/* Modify this file as needed*/
clock_t remainingtime;
int main(int agrc, char * argv[])
{
    //initClk();
    //testing
    //TODO it needs to get the remaining time from somewhere
    //remainingtime = ??;
    clock_t current_time,start_t,last;
    current_time = 0;
    remainingtime=(clock_t)atoi(argv[1]);
    //printf("felP\n");
    last=0;
    
    while (remainingtime > current_time)
    {  

       // current_time = (clock_t)getClk();
        start_t = clock();
        current_time = (double)((start_t)/(CLOCKS_PER_SEC)) ;
        if(last!=current_time)
        {
            last=current_time;
            printf("The process is in second number :  %d \n",(int)last);
        }
        
    }

   
    kill(getppid(),SIGUSR1);
    //destroyClk(false);
    exit(0);
    
    
}