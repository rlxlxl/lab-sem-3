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
    new_stack.data = nullptr;
    new_stack.top_index = 0;
    new_stack.capacity = 0;
    return new_stack;
}

template<typename T>
void resize_stack(Stack<T>& stack) {
    int new_capacity = (stack.capacity == 0) ? 4 : stack.capacity * 2;
    T* new_data = new T[new_capacity]();
    for (int i = 0; i < stack.top_index; i++) new_data[i] = stack.data[i];
    delete[] stack.data;
    stack.data = new_data;
    stack.capacity = new_capacity;
}

template<typename T>
void push_stack(Stack<T>& stack, T new_element) {
    if (stack.top_index >= stack.capacity) resize_stack(stack);
    stack.data[stack.top_index] = new_element;
    stack.top_index++;
}

template<typename T>
T pop_stack(Stack<T>& stack) {
    if (stack.top_index == 0) {
        cerr << "Ошибка: стек пуст" << endl;
        return T();
    }
    stack.top_index--;
    return stack.data[stack.top_index];
}

template<typename T>
void print_stack(Stack<T>& stack) {
    for (int i = 0; i < stack.top_index; i++) cout << stack.data[i] << " ";
    cout << endl;
}