#include "queue.h"
#include <iostream>
#include <cstdlib>
#include <cstring>
#include <cstdio>
using namespace std;

#define MAX_QUERY_LENGTH 1024

// Инициализация очереди
void initQueue(Queue* queue, const char* name) {
    queue->front = nullptr;  // Начало очереди
    queue->rear = nullptr;   // Конец очереди  
    queue->size = 0;         // Количество элементов
    strcpy(queue->name, name);  // Сохраняем имя очереди
}

// Освобождение памяти очереди
void freeQueue(Queue* queue) {
    QueueNode* current = queue->front;
    while (current != nullptr) {
        QueueNode* next = current->next;
        free(current->data);  // Освобождаем данные
        free(current);        // Освобождаем узел
        current = next;
    }
    queue->front = nullptr;
    queue->rear = nullptr;
    queue->size = 0;
}

// Добавление элемента в конец очереди
void queuePush(Queue* queue, const char* value) {
    QueueNode* newNode = (QueueNode*)malloc(sizeof(QueueNode));
    newNode->data = (char*)malloc(strlen(value) + 1);
    strcpy(newNode->data, value);
    newNode->next = nullptr;  // Новый элемент всегда последний
    
    if (queue->rear == nullptr) {
        // Очередь пуста - новый элемент становится и front и rear
        queue->front = newNode;
        queue->rear = newNode;
    } else {
        // Добавляем после текущего rear
        queue->rear->next = newNode;
        queue->rear = newNode;  // Обновляем указатель на конец
    }
    queue->size++;
}

// Удаление элемента из начала очереди
char queuePop(Queue* queue) {
    if (queue->front == nullptr) {
        cout << "Очередь пуста" << endl;
        return '\0';
    }
    
    QueueNode* temp = queue->front;
    queue->front = queue->front->next;  // Сдвигаем начало
    
    // Если удалили последний элемент
    if (queue->front == nullptr) {
        queue->rear = nullptr;
    } 
    char result = temp->data[0];
    free(temp);
    queue->size--;
    return result;  
}

// Просмотр первого элемента без удаления
const char* queuePeek(Queue* queue) {
    if (queue->front == nullptr) {
        return nullptr;
    }
    return queue->front->data;
}

// Получение размера очереди
int getQueueSize(Queue* queue) {
    return queue->size;
}

// Поиск очереди по имени
Queue* findQueueByName(Queue* queues, int* queueCount, const char* name) {
    for (int i = 0; i < *queueCount; i++) {
        if (strcmp(queues[i].name, name) == 0) {
            return &queues[i];
        }
    }
    return nullptr;
}

// Создание новой очереди
Queue* createQueue(Queue* queues, int* queueCount, const char* name) {
    Queue* newQueue = &queues[*queueCount];
    initQueue(newQueue, name);
    (*queueCount)++;
    return newQueue;
}

// Сохранение очередей в файл
void saveQueuesToFile(const char* filename, Queue* queues, int queueCount) {
    FILE* file = fopen(filename, "w");
    if (!file) {
        cout << "Ошибка открытия файла для записи" << endl;
        return;
    }
    
    for (int i = 0; i < queueCount; i++) {
        fprintf(file, "QUEUE %s\n", queues[i].name);
        
        // Сохраняем все элементы очереди
        QueueNode* current = queues[i].front;
        while (current != nullptr) {
            fprintf(file, "  QDATA %s\n", current->data);
            current = current->next;
        }
        fprintf(file, "END_QUEUE\n");
    }
    
    fclose(file);
}

// Загрузка очередей из файла
void loadQueuesFromFile(const char* filename, Queue* queues, int* queueCount) {
    FILE* file = fopen(filename, "r");
    if (!file) return;
    
    char line[MAX_QUERY_LENGTH];
    Queue* currentQueue = nullptr;
    
    while (fgets(line, sizeof(line), file)) {
        line[strcspn(line, "\n")] = 0;
        
        if (strncmp(line, "QUEUE ", 6) == 0) {
            char queueName[256];
            sscanf(line + 6, "%255s", queueName);
            currentQueue = findQueueByName(queues, queueCount, queueName);
            if (!currentQueue) {
                currentQueue = createQueue(queues, queueCount, queueName);
            }
        } 
        else if (strncmp(line, "  QDATA ", 8) == 0 && currentQueue) {
            queuePush(currentQueue, line + 8);  // Добавляем элемент в очередь
        }
        else if (strcmp(line, "END_QUEUE") == 0) {
            currentQueue = nullptr;
        }
    }
    
    fclose(file);
}

// Обработка команд для очередей
void executeQueueCommand(Queue* queues, int* queueCount, const char* command, const char* filename) {
    char cmd[50], queueName[256], value[512];
    
    // QPUSH - добавление в очередь
    if (sscanf(command, "QPUSH %255s %511[^\n]", queueName, value) == 2) {
        Queue* queue = findQueueByName(queues, queueCount, queueName);
        if (!queue) {
            queue = createQueue(queues, queueCount, queueName);
        }
        queuePush(queue, value);
        saveQueuesToFile(filename, queues, *queueCount);
        cout << "Успешно!" << endl;
    }
    // QPOP - удаление из начала очереди
    else if (sscanf(command, "QPOP %255s", queueName) == 1) {
        Queue* queue = findQueueByName(queues, queueCount, queueName);
        if (queue) {
            queuePop(queue);
            saveQueuesToFile(filename, queues, *queueCount);
            cout << "Успешно!" << endl;
        } else {
            cout << "Очередь не найдена" << endl;
        }
    }
    // QPEEK - просмотр первого элемента
    else if (sscanf(command, "QPEEK %255s", queueName) == 1) {
        Queue* queue = findQueueByName(queues, queueCount, queueName);
        if (queue) {
            const char* result = queuePeek(queue);
            if (result) {
                cout << result << endl;
            } else {
                cout << "Очередь пуста" << endl;
            }
        } else {
            cout << "Очередь не найдена" << endl;
        }
    }
    // QSIZE - получение размера очереди
    else if (sscanf(command, "QSIZE %255s", queueName) == 1) {
        Queue* queue = findQueueByName(queues, queueCount, queueName);
        if (queue) {
            cout << queue->size << endl;
        } else {
            cout << "Очередь не найдена" << endl;
        }
    }
    else {
        cout << "Неизвестная команда очереди" << endl;
    }
}