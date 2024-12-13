
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

typedef struct Queue {
    Node* front;
    Node* rear;
} Queue;



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


Queue* createQueue() {
    Queue* queue = (Queue*)malloc(sizeof(Queue));
    if (queue==NULL) {
        printf("Error in creating Queue\n");
        return NULL;
    }
    queue->front = queue->rear = NULL;
    return queue;
}

int isEmpty(Queue* queue) {
    return queue->front == NULL;
}

// Function to add an element to the queue
void enqueue(Queue* queue, Process_Data data) {
    Node* newNode = createNode(data);
    if (queue->rear == NULL) {
        queue->front = queue->rear = newNode;
        return;
    }
    queue->rear->next = newNode;
    queue->rear = newNode;
}


void dequeue(Queue* queue, Process_Data* proc) {
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


void peek(Queue* queue, Process_Data* proc) {
    if (isEmpty(queue)) {
        printf("Queue is empty, nothing to peek\n");
        return;
    }
    *proc = queue->front->data;
}

void displayQueue(Queue* queue) {
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



