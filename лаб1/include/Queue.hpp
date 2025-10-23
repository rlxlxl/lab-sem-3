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
    Queue<T> q;
    return q;
}

template<typename T>
void resize_queue(Queue<T>& q) {
    int old_cap = q.capacity;
    int new_cap = (old_cap == 0) ? 2 : old_cap * 2;

    T* new_data = new T[new_cap]();
    for (int i = 0; i < q.size; i++) {
        int idx = (q.front + i) % (old_cap == 0 ? 1 : old_cap);
        new_data[i] = q.data[idx];
    }

    delete[] q.data;
    q.data = new_data;
    q.capacity = new_cap;
    q.front = 0;
    q.rear = q.size;
}


template<typename T>
void push_queue(Queue<T>& q, T new_element) {
    if (q.size == q.capacity) {
        resize_queue(q);
    }

    q.data[q.rear] = new_element;
    q.rear = (q.rear + 1) % q.capacity;
    q.size++;
}


template<typename T>
T pop_queue(Queue<T>& q) {
    if (q.size == 0) {
        cerr << "Ошибка: очередь пуста" << endl;
        return T();
    }

    q.rear = (q.rear - 1 + q.capacity) % q.capacity;
    T value = q.data[q.rear];
    q.size--;

    if (q.size == 0) {
        q.front = q.rear = 0;
    }

    return value;
}


template<typename T>
void print_queue(Queue<T>& q) {
    for (int i = 0; i < q.size; i++) {
        int idx = (q.front + i) % q.capacity;
        cout << q.data[idx] << " ";
    }
    cout << endl;
}
