#pragma once
#include <iostream>
using namespace std;

template<typename T>
class Stack {
private:
    struct StackNode {
        T data;
        StackNode* next;

        StackNode(T value) : data(value), next(nullptr) {}
    };

    StackNode* top_node;
    int sz;

public:
    Stack() : top_node(nullptr), sz(0) {}

    ~Stack() {
        clear();
    }

    void push(const T& value) {
        StackNode* new_node = new StackNode(value);
        new_node->next = top_node;
        top_node = new_node;
        sz++;
    }

    T pop() {
        if (is_empty()) {
            cerr << "Ошибка: стек пуст" << endl;
            return T();
        }
        StackNode* temp = top_node;
        T value = temp->data;
        top_node = top_node->next;
        delete temp;
        sz--;
        return value;
    }

    T peek() const {
        if (is_empty()) {
            cerr << "Ошибка: стек пуст" << endl;
            return T();
        }
        return top_node->data;
    }

    bool is_empty() const {
        return top_node == nullptr;
    }

    int size() const {
        return sz;
    }

    void print() const {
        StackNode* current = top_node;
        while (current != nullptr) {
            cout << current->data << " ";
            current = current->next;
        }
        cout << endl;
    }

    void clear() {
        while (top_node != nullptr) {
            StackNode* temp = top_node;
            top_node = top_node->next;
            delete temp;
        }
        sz = 0;
    }

    // Копирование стека
    void copy_from(const Stack<T>& source) {
        clear();

        // Временный стек для сохранения порядка
        Stack<T> temp_stack;
        StackNode* current = source.top_node;

        while (current != nullptr) {
            temp_stack.push(current->data);
            current = current->next;
        }

        current = temp_stack.top_node;
        while (current != nullptr) {
            push(current->data);
            current = current->next;
        }
    }
};
