#include "queue.c"



int main(int argc, char *argv[])
{
    initClk();

    int scheduling_algo = atoi(argv[1]);
    int quantum = atoi(argv[2]);
    int processcount = atoi(argv[3]);

    key_t key = ftok("schedulerqueue", 'S');
    int msgqID_PG = msgget(key, IPC_CREAT | 0666);
    if (msgqID_PG == -1)
    {
        perror("Error in creating msg queue in scheduler\n");
        exit(-1);
    }
    int count =0;
    while (1)
    {
        struct procmsgbuff msg;
        printf("shced %d\n ", getClk());
        int rec_val = msgrcv(msgqID_PG, &msg, sizeof(msg.process), 1, !IPC_NOWAIT);
        displayProcessData(msg.process);

        
    }


}
