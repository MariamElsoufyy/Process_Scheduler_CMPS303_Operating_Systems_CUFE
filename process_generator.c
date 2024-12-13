
#include "queue.c"
/////functions Definitions
void clearResources(int);

int forkScheduler(int schedulingalgo, int quantum, int ProcessCount);
int forkClk();
void forkClockScheduler(char *algorthmNo, char *quantum, char *countProcesses);

/// global variables ///
int msgqID;
pid_t clkpid, schedpid;
Process_Data *Queuepointer;
Process_Data *PCB;

// argv[0] useless //argv[1] filename //argv[2] sched algo //argv[3] useless  //argv[4n  ] useless //argv[5] round robin Quantum
// process_generator.o testcase.txt -sch 5 -q 2
//    0                   1           2  3 4  5
int main(int argc, char *argv[])
{
    signal(SIGINT, clearResources);

    char *filename = argv[1];
    int schedulingalgorithm = atoi(argv[3]);
    int quantum = atoi(argv[5]);

    key_t key = ftok("schedulerqueue", 'S');
    msgqID = msgget(key, IPC_CREAT | 0666);

    FILE *processes_file = fopen(filename, "r");
    if (filename == NULL)
    {
        perror("Error in opening the file!");
        exit(-1);
    }

    Queue *processQueue = createQueue();
    int procid, arrtime, runtime, priority, processcount = 0;
    char line[100];

    Process_Data *procpointer;
    while (fgets(line, sizeof(line), processes_file) != NULL)
    {
        printf("ya mosahel yarab");
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

    displayQueue(processQueue);

    return 0;
    printf("%d \n ", processcount);
    char c[100];
    snprintf(c, sizeof(c), "%d", processcount);
    forkClk();

    kill(getppid(), SIGINT);
}

////////////////////////////////////functions////

void clearResources(int signum)
{
    // TODO Clears all resources in case of interruption
    kill(clkpid, SIGINT);
    kill(schedpid, SIGINT);
    /// free(Queuepointer);
    msgctl(msgqID, IPC_RMID, (struct msqid_ds *)0);
    exit(0);
}

///////////////////////////////////////////forking functions////

int forkClk()
{

    int clk_pid = fork();
    printf("%d \n ", clk_pid);
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
    return clk_pid;
}

int forkScheduler(int schedulingalgo, int quantum, int ProcessCount)
{

    int scheduler_pid = fork();
    if (scheduler_pid == -1)
    {
        perror("Error in fork");
        exit(-1);
    }
    else if (scheduler_pid == 0)
    {
        execl("./scheduler.out", "scheduler.out", schedulingalgo, quantum, ProcessCount, NULL);
    }

    return scheduler_pid;
}

void forkClockScheduler(char *algorthmNo, char *quantum, char *countProcesses)
{
    int clk_pid = fork();
    if (clk_pid == -1)
    {
        perror("Error in fork");
        exit(-1);
    }
    else if (clk_pid == 0)
    {
        execl("./clk.out", "clk.out", NULL);
        sleep(3);
    }
    else
    {
        int scheduler_pid = fork();
        if (scheduler_pid == -1)
        {
            perror("Error in fork");
            exit(-1);
        }
        else if (scheduler_pid == 0)
        {
            execl("./scheduler.out", "scheduler.out", algorthmNo, quantum, countProcesses, NULL);
        }
    }
}