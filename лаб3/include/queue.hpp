#pragma once
#include <iostream>
using namespace std;

template<typename T>
class Queue {
private:
    T* data;
    int front;
    int rear;
    int capacity;
    int size;

    void resize() {
        int new_cap = (capacity == 0) ? 2 : capacity * 2;
        T* new_data = new T[new_cap];

        for (int i = 0; i < size; i++) {
            int idx = (front + i) % (capacity == 0 ? 1 : capacity);
            new_data[i] = data[idx];
        }

        delete[] data;
        data = new_data;
        capacity = new_cap;
        front = 0;
        rear = size;
    }

public:
    Queue() : data(nullptr), front(0), rear(0), capacity(0), size(0) {}
    
    ~Queue() {
        delete[] data;
    }

    void push(const T& value) {
        if (size == capacity) {
            resize();
        }
        data[rear] = value;
        rear = (rear + 1) % capacity;
        size++;
    }

    T pop() {
        if (size == 0) {
            cerr << "Ошибка: очередь пуста" << endl;
            return T();
        }
        T value = data[front];
        front = (front + 1) % capacity;
        size--;

        if (size == 0) {
            front = rear = 0;
        }

        return value;
    }

    bool is_empty() const {
        return size == 0;
    }

    int get_size() const {
        return size;
    }

    void print() const {
        if (size == 0) {
            cout << "Очередь пуста" << endl;
            return;
        }
        for (int i = 0; i < size; i++) {
            int idx = (front + i) % capacity;
            cout << data[idx] << " ";
        }
        cout << endl;
    }
};
