#pragma once

#include <iostream>
using namespace std;

template<typename T>
struct Stack {
    T* data = nullptr;
    int top_index = 0;
    int capacity = 0;
};

template<typename T>
Stack<T> create_stack() {
    Stack<T> new_stack;
    return new_stack;
}

template<typename T>
void resize_stack(Stack<T>& stack) {
    int new_capacity = stack.capacity;
    if (new_capacity == 0) {
        new_capacity = (stack.capacity + 1) * 2;
    } else {
        new_capacity = stack.capacity * 2;
    }
    T* new_data = new T[new_capacity]();
    for (int i = 0; i < stack.top_index; i++) {
        new_data[i] = stack.data[i];
    }
    delete[] stack.data;
    stack.data = new_data;
    stack.capacity = new_capacity;
}

template<typename T>
void push_stack(Stack<T>& stack, T new_element) {
    if (stack.top_index >= stack.capacity) {
        resize_stack(stack);
    }
    stack.data[stack.top_index] = new_element;
    stack.top_index++;
}

template<typename T>
void pop_stack(Stack<T>& stack) {
    if (stack.top_index == 0) {
        cerr << "Ошибка: список пуст" << endl;
        return;
    }
    stack.top_index--;
}

template<typename T>
void print_stack(Stack<T>& stack) {
    if (stack.top_index == 0) {
        cout << "Список пуст" << endl;
        return;
    }
    cout << "Список элементов: ";
    for (int i = 0; i <= stack.top_index - 1; i++) {
        cout << stack.data[i] << ", ";
    }
    cout << endl;
}