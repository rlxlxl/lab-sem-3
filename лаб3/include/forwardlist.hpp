#pragma once
#include <iostream>
using namespace std;

template<typename T>
class ForwardList {
private:
    struct Node {
        T data;
        Node* next;
        Node(const T& value) : data(value), next(nullptr) {}
    };

    Node* head;

public:
    ForwardList() : head(nullptr) {}

    ~ForwardList() {
        clear();
    }

    // Добавление
    void push_front(const T& value) {
        Node* new_node = new Node(value);
        new_node->next = head;
        head = new_node;
    }

    void push_back(const T& value) {
        Node* new_node = new Node(value);
        if (!head) {
            head = new_node;
            return;
        }
        Node* current = head;
        while (current->next)
            current = current->next;
        current->next = new_node;
    }

    void insert_before(Node* node, const T& value) {
        if (!node) {
            cerr << "Ошибка: передан нулевой указатель" << endl;
            return;
        }
        if (node == head) {
            push_front(value);
            return;
        }
        Node* current = head;
        while (current && current->next != node)
            current = current->next;
        if (!current) {
            cerr << "Ошибка: узел не найден" << endl;
            return;
        }
        Node* new_node = new Node(value);
        new_node->next = node;
        current->next = new_node;
    }

    void insert_after(Node* node, const T& value) {
        if (!node) {
            cerr << "Ошибка: передан нулевой указатель" << endl;
            return;
        }
        Node* new_node = new Node(value);
        new_node->next = node->next;
        node->next = new_node;
    }

    // Поиск
    Node* find(const T& value) const {
        Node* current = head;
        while (current) {
            if (current->data == value)
                return current;
            current = current->next;
        }
        return nullptr;
    }

    // Удаление
    void pop_front() {
        if (!head) return;
        Node* todel = head;
        head = head->next;
        delete todel;
    }

    void pop_back() {
        if (!head) return;
        if (!head->next) {
            delete head;
            head = nullptr;
            return;
        }
        Node* current = head;
        while (current->next->next)
            current = current->next;
        delete current->next;
        current->next = nullptr;
    }

    void erase(Node* node) {
        if (!head || !node) return;
        if (node == head) {
            pop_front();
            return;
        }
        Node* current = head;
        while (current && current->next != node)
            current = current->next;
        if (!current) return;
        current->next = node->next;
        delete node;
    }

    void erase_by_value(const T& value) {
        if (!head) return;
        if (head->data == value) {
            pop_front();
            return;
        }
        Node* current = head;
        while (current->next && current->next->data != value)
            current = current->next;
        if (current->next) {
            Node* todel = current->next;
            current->next = todel->next;
            delete todel;
        }
    }

    void erase_before(Node* node) {
        if (!head || !node || node == head) return;
        Node* prev = nullptr;
        Node* cur = head;
        while (cur && cur->next != node) {
            prev = cur;
            cur = cur->next;
        }
        if (!cur) return;
        if (!prev) { // удаляем голову
            head = node;
            delete cur;
        } else {
            prev->next = node;
            delete cur;
        }
    }

    void erase_after(Node* node) {
        if (!node || !node->next) return;
        Node* todel = node->next;
        node->next = todel->next;
        delete todel;
    }

    // Печать
    void print_while() const {
        Node* current = head;
        if (!current) {
            cout << "Список пуст" << endl;
            return;
        }
        while (current) {
            cout << "Значение: " << current->data 
                 << "; адрес: " << current 
                 << "; адрес следующего узла: " << current->next << endl;
            current = current->next;
        }
    }

    void print_for() const {
        for (Node* current = head; current; current = current->next) {
            cout << "Значение: " << current->data 
                 << "; адрес: " << current 
                 << "; адрес следующего узла: " << current->next << endl;
        }
    }

    void clear() {
        while (head) {
            Node* todel = head;
            head = head->next;
            delete todel;
        }
    }

    Node* get_head() const { return head; }
};
