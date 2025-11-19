#ifndef QUEUE_H
#define QUEUE_H

struct QueueNode {
    char* data;
    QueueNode* next;
};

struct Queue {
    QueueNode* front;
    QueueNode* rear;
    int size;
    char name[50];
};

void initQueue(Queue* queue, const char* name);
void freeQueue(Queue* queue);
void queuePush(Queue* queue, const char* value);
char queuePop(Queue* queue);
const char* queuePeek(Queue* queue);
int getQueueSize(Queue* queue);

void executeQueueCommand(Queue* queues, int* queueCount, const char* command, const char* filename);
void saveQueuesToFile(const char* filename, Queue* queues, int queueCount);
void loadQueuesFromFile(const char* filename, Queue* queues, int* queueCount);
Queue* findQueueByName(Queue* queues, int* queueCount, const char* name);
Queue* createQueue(Queue* queues, int* queueCount, const char* name);

#endif
