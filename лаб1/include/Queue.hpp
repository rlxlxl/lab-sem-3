#pragma once

#include <iostream>
using namespace std;

template<typename T>
struct Queue {
    T* data = nullptr;
    int front = 0;
    int rear = 0;
    int capacity = 0;
    int size = 0;
};

template<typename T>
Queue<T> create_queue() {
    Queue<T> new_queue;
    new_queue.data = nullptr;
    new_queue.front = 0;
    new_queue.rear = 0;
    new_queue.capacity = 0;
    new_queue.size = 0;
    return new_queue;
}

template<typename T>
void resize_queue(Queue<T>& queue) {
    int new_capacity = queue.capacity;
    if (new_capacity == 0) {
        new_capacity = (queue.capacity + 1) * 2;
    } else {
        new_capacity = queue.capacity * 2;
    }
    T* new_data = new T[new_capacity]();
    int new_index = 0;
    for (int i = 0; i < queue.size; i++) {
        int index = (queue.front + i) % queue.capacity;
        new_data[new_index++] = queue.data[index]; 
    }
    delete[] queue.data;
    queue.data = new_data;
    queue.capacity = new_capacity;
    queue.front = 0;
    queue.rear = queue.size;
}

template<typename T>
void push_queue(Queue<T>& queue, T new_element) {
    if (queue.size == queue.capacity) {
        resize_queue(queue);
    }
    queue.data[queue.rear] = new_element;
    queue.rear = (queue.rear + 1) % queue.capacity;
    queue.size++;
}

template<typename T>
T pop_queue(Queue<T>& queue) {
    if (queue.size == 0) {
        cerr << "Ошибка: очередь пуста" << endl;
        return T();
    }
    T value = queue.data[queue.front];
    queue.front = (queue.front + 1) % queue.capacity;
    queue.size--;
    return value;
}

template<typename T>
void print_queue(Queue<T>& queue) {
    for (int i = 0; i < queue.size; i++) {
        int index = (queue.front + i) % queue.capacity;
        cout << queue.data[index] << " ";
    }
    cout << endl;
}

