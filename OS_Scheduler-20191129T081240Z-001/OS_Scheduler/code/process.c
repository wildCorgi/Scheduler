#include "headers.h"
#include <time.h>

/* Modify this file as needed*/
int remainingtime;
int main(int agrc, char * argv[])
{
    initClk();
    //testing
    //TODO it needs to get the remaining time from somewhere
    //remainingtime = ??;
    int current_time = 0;
    double c1;
    remainingtime=atoi(argv[1]);
    printf("felP\n");
    
    while (remainingtime > current_time)
    {
        c1 = clock();
        current_time = (double)((c1)/(CLOCKS_PER_SEC)) ;
        printf("Current P time: %d\n",current_time);
    }
    destroyClk(false);
    
    return 0;
}
