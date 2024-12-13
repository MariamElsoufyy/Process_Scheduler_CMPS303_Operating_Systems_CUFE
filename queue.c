
#include "structs.c"
#include <stdio.h>


void displayProcessData(Process_Data data){
    
    printf("#id arrival runtime priority\n");
    printf("%d\t%d\t%d\t%d\n",data.id,data.arrival,data.runtime,data.priority);
}

typedef struct Node {
    Process_Data data;
    struct Node* next;
} Node;

typedef struct Queue_LinkedList {
    Node* front;
    Node* rear;
} Queue_LinkedList;



Node* createNode(Process_Data data) {
    Node* newNode = (Node*)malloc(sizeof(Node));
    if (!newNode) {
        printf("Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }
    newNode->data = data;
    newNode->next = NULL;
    return newNode;
}


Queue_LinkedList* createQueue() {
    Queue_LinkedList* queue = (Queue_LinkedList*)malloc(sizeof(Queue_LinkedList));
    if (queue==NULL) {
        printf("Error in creating Queue\n");
        return NULL;
    }
    queue->front = queue->rear = NULL;
    return queue;
}

int isEmpty(Queue_LinkedList* queue) {
    return queue->front == NULL;
}

// Function to add an element to the queue
void enqueue(Queue_LinkedList* queue, Process_Data data) {
    Node* newNode = createNode(data);
    if (queue->rear == NULL) {
        queue->front = queue->rear = newNode;
        return;
    }
    queue->rear->next = newNode;
    queue->rear = newNode;
}


void dequeue(Queue_LinkedList* queue, Process_Data* proc) {
    if (isEmpty(queue)) {
        printf("Queue is empty, cannot dequeue\n");
        proc = NULL; 
        return;
    }

    Node* temp = queue->front;
    proc = &temp->data; 
    queue->front = queue->front->next;

    if (queue->front == NULL) {
        queue->rear = NULL;
    }

    free(temp);
}


void peek(Queue_LinkedList* queue, Process_Data* proc) {
    if (isEmpty(queue)) {
        printf("Queue is empty, nothing to peek\n");
        return;
    }
    *proc = queue->front->data;
}

void displayQueue(Queue_LinkedList* queue) {
    if (isEmpty(queue)) {
        printf("Queue is empty\n");
        return;
    }
    Node* current = queue->front;
    while (current != NULL) {
        displayProcessData(current->data);
        current = current->next;
    }
    printf("\n");
}



typedef struct Queue
{
    Process_Data *data; // Array of Process_Data* structs
    int front;
    int rear;
    int capacity; // number of elements existing in the queue

} Queue;