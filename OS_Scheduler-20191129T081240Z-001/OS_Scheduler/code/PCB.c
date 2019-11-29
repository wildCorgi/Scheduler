#include "headers.h"

typedef struct PCB {
    int processID;
    int runTime;
    int arrivalTime;
    int finishTime;
    int priority;
    int turnaroundTime;
    enum state {
        READY = 0 ,
        RUNNING = 1
    };
}