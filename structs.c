#include "headers.h"


typedef struct Process_Data
{
    int id;
    int arrival;
    int runtime;
    int priority;
    int waittime;
    int finishtime;
    int remainingtime;
    int state; // 1 - started 2-stopped 3-resumed 4-finished
    int TA;
    int WTA;
} Process_Data;



struct procmsgbuff
{
    long mtype;
    struct Process_Data process;
};



