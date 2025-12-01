#pragma once
#include <iostream>
using namespace std;

template<typename T>
class DoublyList {
private:

    struct Node {
        T data;
        Node* next;
        Node* prev;

        Node(const T& value) : data(value), next(nullptr), prev(nullptr) {}
    };

    Node* head;
    Node* tail;

    bool contains(Node* node) const {
        Node* cur = head;
        while (cur) {
            if (cur == node) return true;
            cur = cur->next;
        }
        return false;
    }

public:

    DoublyList() : head(nullptr), tail(nullptr) {}

    ~DoublyList() {
        Node* cur = head;
        while (cur) {
            Node* next = cur->next;
            delete cur;
            cur = next;
        }
    }

    // ------------------------
    //  ДОБАВЛЕНИЕ
    // ------------------------

    void push_front(const T& value) {
        Node* node = new Node(value);
        node->next = head;

        if (head)
            head->prev = node;
        else
            tail = node;  // список был пуст

        head = node;
    }

    void push_back(const T& value) {
        Node* node = new Node(value);
        node->prev = tail;

        if (tail)
            tail->next = node;
        else
            head = node;  // список был пуст

        tail = node;
    }

    void insert_before(Node* where, const T& value) {
        if (!where) {
            cerr << "Ошибка: null-узел\n";
            return;
        }
        if (!contains(where)) {
            cerr << "Ошибка: узел не принадлежит списку\n";
            return;
        }

        Node* node = new Node(value);
        node->next = where;
        node->prev = where->prev;

        if (where->prev)
            where->prev->next = node;
        else
            head = node;

        where->prev = node;
    }

    void insert_after(Node* where, const T& value) {
        if (!where) {
            cerr << "Ошибка: null-узел\n";
            return;
        }
        if (!contains(where)) {
            cerr << "Ошибка: узел не принадлежит списку\n";
            return;
        }

        Node* node = new Node(value);
        node->prev = where;
        node->next = where->next;

        if (where->next)
            where->next->prev = node;
        else
            tail = node;

        where->next = node;
    }

    // ------------------------
    //  УДАЛЕНИЕ
    // ------------------------

    void pop_front() {
        if (!head) {
            cerr << "Ошибка: список пуст\n";
            return;
        }

        Node* to_delete = head;
        head = head->next;

        if (head)
            head->prev = nullptr;
        else
            tail = nullptr;

        delete to_delete;
    }

    void pop_back() {
        if (!tail) {
            cerr << "Ошибка: список пуст\n";
            return;
        }

        Node* to_delete = tail;
        tail = tail->prev;

        if (tail)
            tail->next = nullptr;
        else
            head = nullptr;

        delete to_delete;
    }

    void erase(Node* node) {
        if (!node) {
            cerr << "Ошибка: null-узел\n";
            return;
        }
        if (!contains(node)) {
            cerr << "Ошибка: узел не принадлежит списку\n";
            return;
        }

        if (node->prev)
            node->prev->next = node->next;
        else
            head = node->next;

        if (node->next)
            node->next->prev = node->prev;
        else
            tail = node->prev;

        delete node;
    }

    void delete_value(const T& value) {
        Node* cur = head;
        while (cur) {
            if (cur->data == value) {
                erase(cur);
                cout << "OK\n";
                return;
            }
            cur = cur->next;
        }
        cerr << "Ошибка: элемент не найден\n";
    }

    void delete_before_value(const T& value) {
        Node* cur = head;
        while (cur) {
            if (cur->data == value) {
                if (!cur->prev) {
                    cerr << "Ошибка: перед элементом ничего нет\n";
                    return;
                }
                erase(cur->prev);
                return;
            }
            cur = cur->next;
        }
        cerr << "Ошибка: элемент не найден\n";
    }

    void delete_after_value(const T& value) {
        Node* cur = head;
        while (cur) {
            if (cur->data == value) {
                if (!cur->next) {
                    cerr << "Ошибка: после элемента ничего нет\n";
                    return;
                }
                erase(cur->next);
                return;
            }
            cur = cur->next;
        }
        cerr << "Ошибка: элемент не найден\n";
    }

    // ------------------------
    //  ПОИСК
    // ------------------------

    Node* find(const T& value) const {
        Node* cur = head;
        while (cur) {
            if (cur->data == value)
                return cur;
            cur = cur->next;
        }
        cerr << "Ошибка: узел не найден\n";
        return nullptr;
    }

    // ------------------------
    //  ПЕЧАТЬ
    // ------------------------

    void print() const {
        if (!head) {
            cout << "Список пуст\n";
            return;
        }

        Node* cur = head;
        while (cur) {
            cout << "Значение: " << cur->data
                 << "; адрес: " << cur
                 << "; next: " << cur->next
                 << "; prev: " << cur->prev << endl;
            cur = cur->next;
        }
    }

    // ------------------------
    //  Возврат головы/хвоста (например для insert_before)
    // ------------------------

    Node* get_head() const { return head; }
    Node* get_tail() const { return tail; }
};
