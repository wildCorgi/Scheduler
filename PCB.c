#include <stdio.h>      //if you don't use scanf/printf change this include
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/file.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <sys/msg.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

typedef struct PCB {
    int processID;
    int runTime;
    int arrivalTime;
    int finishTime;
    int priority;
    int turnaroundTime;
    enum state {
        READY = 0;
        RUNNING = 1;
    }
}