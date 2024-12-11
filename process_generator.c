#include "headers.h"

void clearResources(int);
// Define the Process_Data struct
typedef struct Process_Data {
    int id;
    int arrival;
    int runtime;
    int priority;
} Process_Data;
// Define the Queue structure
typedef struct Queue
 {
    Process_Data *data;      // Array of Process_Data structs
    int front;
    int rear;
    int capacity;
    int size;
    
} Queue;



int main(int argc, char *argv[])
{
    signal(SIGINT, clearResources);
    // TODO Initialization
    // 1. Read the input files.
    FILE *processes_file = fopen("testcase.txt", "r");
    if (processes_file == NULL) 
    {
        perror("Error in opening the file!");
        exit(-1);
    }
    int processes_number=0;
    char line[100];
    while (fgets(line, sizeof(line), processes_file) != NULL) 
    {
        if (line[0] != '#')        //first character in line is # so skip comment
        {
            processes_number++;   //to count number of processes in the file
        }
    }
    fclose(processes_file);
    processes_file = fopen("testcase.txt", "r");
    char str[100];
    printf("number of processes = %d\n", processes_number);
    fgets(str, 100, processes_file); // skips first line
    for (int i = 0; i < processes_number - 1; i++) //creates a process object for each process
    {
        struct Process_Data Temp_Process;
        fscanf(processes_file, "%d %d %d %d", &Temp_Process.id, &Temp_Process.arrival, &Temp_Process.runtime, &Temp_Process.priority);
        // Temp_Process.remTime=Temp_Process.runningtime;
        // Temp_Process.state=Stopped;
        // Temp_Process.waitingTime=0;
        // Temp_Process.idTextFile=newProcess.id;
        // enqueue(newProcess, inputProcesses, lines);
    }
    // fadl bas a enqueue kol mara al struct dah gwa al queue bt3tyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyy
    
    // 2. Read the chosen scheduling algorithm and its parameters, if there are any from the argument list.
    // 3. Initiate and create the scheduler and clock processes.
    // 4. Use this function after creating the clock process to initialize clock.
    initClk();
    // To get time use this function. 
    int x = getClk();
    printf("Current Time is %d\n", x);
    // TODO Generation Main Loop
    // 5. Create a data structure for processes and provide it with its parameters.
    // 6. Send the information to the scheduler at the appropriate time.
    // 7. Clear clock resources
    destroyClk(true);
}

void clearResources(int signum)
{
    //TODO Clears all resources in case of interruption
}
