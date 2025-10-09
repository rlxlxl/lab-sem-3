 #pragma once

#include <iostream>
using namespace std;

template<typename T>
struct StackNode {
    T data;
    StackNode* next;
    
    StackNode(T value) : data(value), next(nullptr) {}
};

template<typename T>
struct Stack {
    StackNode<T>* top = nullptr;
    int size = 0;
};

template<typename T>
Stack<T> create_stack() {
    Stack<T> new_stack;
    new_stack.top = nullptr;
    new_stack.size = 0;
    return new_stack;
}

template<typename T>
void push_stack(Stack<T>& stack, T new_element) {
    StackNode<T>* new_node = new StackNode<T>(new_element);
    new_node->next = stack.top;
    stack.top = new_node;
    stack.size++;
}

template<typename T>
T pop_stack(Stack<T>& stack) {
    if (stack.top == nullptr) {
        cerr << "Ошибка: стек пуст" << endl;
        return T();
    }
    
    StackNode<T>* temp = stack.top;
    T popped_value = temp->data;
    stack.top = stack.top->next;
    delete temp;
    stack.size--;
    
    return popped_value;
}

template<typename T>
T peek_stack(const Stack<T>& stack) {
    if (stack.top == nullptr) {
        cerr << "Ошибка: стек пуст" << endl;
        return T();
    }
    return stack.top->data;
}

template<typename T>
bool is_empty_stack(const Stack<T>& stack) {
    return stack.top == nullptr;
}

template<typename T>
void print_stack(const Stack<T>& stack) {
    StackNode<T>* current = stack.top;
    while (current != nullptr) {
        cout << current->data << " ";
        current = current->next;
    }
    cout << endl;
}

template<typename T>
void clear_stack(Stack<T>& stack) {
    while (stack.top != nullptr) {
        StackNode<T>* temp = stack.top;
        stack.top = stack.top->next;
        delete temp;
    }
    stack.size = 0;
}

template<typename T>
void copy_stack(const Stack<T>& source, Stack<T>& destination) {
    clear_stack(destination);
    
    // Создаем временный стек для сохранения порядка
    Stack<T> temp_stack = create_stack<T>();
    StackNode<T>* current = source.top;
    
    // Копируем элементы во временный стек (обратный порядок)
    while (current != nullptr) {
        push_stack(temp_stack, current->data);
        current = current->next;
    }
    
    // Копируем из временного стека в destination (правильный порядок)
    current = temp_stack.top;
    while (current != nullptr) {
        push_stack(destination, current->data);
        current = current->next;
    }
    
    clear_stack(temp_stack);
}