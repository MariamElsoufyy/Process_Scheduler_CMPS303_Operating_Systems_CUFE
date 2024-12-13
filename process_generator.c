
#include "queue.c"
/////functions Definitions
void clearResources(int);

int forkClkandScheduler(char *schedulingalgo, char *quantum, char *ProcessCount);
int forkClk();

/// global variables ///
int msgqID;
pid_t clk_pid, scheduler_pid;
Process_Data *Queuepointer;
Process_Data *PCB;

// argv[0] useless //argv[1] filename //argv[2] sched algo //argv[3] useless  //argv[4n  ] useless //argv[5] round robin Quantum
// process_generator.o testcase.txt -sch 5 -q 2
//    0                   1           2  3 4  5
int main(int argc, char *argv[])
{
    signal(SIGINT, clearResources);

    /// initializing command line variables
    char *filename = argv[1];
    char* schedulingalgorithm = argv[3];
    char* quantum = argv[5];

    /// Creating message Queue for sending processes to scheduler
    key_t key = ftok("schedulerqueue", 'S');
    msgqID = msgget(key, IPC_CREAT | 0666);
    if (msgqID == -1)
    {
        perror("Error in creating msg queue in process generator\n");
        exit(-1);
    }

    /// Reading input file
    FILE *processes_file = fopen(filename, "r");
    if (filename == NULL)
    {
        perror("Error in opening the file!");
        exit(-1);
    }

    Queue_LinkedList *processQueue = createQueue();
    int procid, arrtime, runtime, priority, processcount = 0;
    char line[100];

    while (fgets(line, sizeof(line), processes_file) != NULL)
    {
        if (line[0] != '#') // first character in line is # so skip comment
        {
            sscanf(line, "%d\t%d\t%d\t%d", &procid, &arrtime, &runtime, &priority);
            processcount++; // to count number of processes in the file

            Process_Data proc;
            proc.id = procid;
            proc.arrival = arrtime;
            proc.runtime = runtime;
            proc.priority = priority;
            enqueue(processQueue, proc);
        }
    }
    fclose(processes_file);
    char processcountstring[100];
    sprintf(processcountstring, "%d", processcount);
    forkClkandScheduler(schedulingalgorithm,quantum, processcountstring);

    initClk();

    // displayQueue(processQueue);

    while (!isEmpty(processQueue))
    {
        Process_Data process;
        peek(processQueue, &process);
        if (process.arrival == getClk())
        {

            dequeue(processQueue, &process); 
            struct procmsgbuff msg;
            msg.process = process;
            msg.mtype = 1;
            int send_val = msgsnd(msgqID, &msg, sizeof(msg.process), !IPC_NOWAIT); // Remove invalid flags
            if (send_val == -1)
            {
                perror("Error in sending in process generator");
            }
        }
    }

    kill(getpgrp(), SIGINT);
}

////////////////////////////////////functions////

void clearResources(int signum)
{
    // TODO Clears all resources in case of interruption
    kill(clk_pid, SIGINT);
    kill(scheduler_pid, SIGINT);
    /// free(Queuepointer);
    msgctl(msgqID, IPC_RMID, (struct msqid_ds *)0);

    exit(0);
}

///////////////////////////////////////////forking functions////

int forkClkandScheduler(char *schedulingalgo, char *quantum, char *ProcessCount)
{

    clk_pid = fork();
    if (clk_pid == -1)
    {
        perror("Error in forking clock");
        exit(-1);
    }
    else if (clk_pid == 0)
    {
        execl("./clk.out", "clk.out", NULL);
        sleep(3);
    }
    else
    {
        scheduler_pid = fork();

        if (scheduler_pid == -1)
        {
            perror("Error in fork");
            exit(-1);
        }
        else if (scheduler_pid == 0)
        {
            execl("./scheduler.out", "scheduler.out", schedulingalgo, quantum, ProcessCount, NULL);
        }
    }
}
