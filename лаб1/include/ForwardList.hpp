#pragma once

#include <iostream>
using namespace std;

template<typename T>
struct Node_Fl {
    T* data = nullptr;
    Node_Fl* next = nullptr;
};

template<typename T>
struct ForwardList {
    Node_Fl<T>* head;
};

template<typename T>
ForwardList<T> create_forwardList() {
    ForwardList<T> new_fl;
    new_fl.head = nullptr;
    return new_fl;
}

template<typename T>
void add_element_front_fl(T new_element, ForwardList<T>& fl) {
    Node_Fl<T>* new_node = new Node_Fl<T>;
    new_node->data = new T(new_element);
    new_node->next = fl.head;
    fl.head = new_node;
}

template<typename T>
void add_element_back_fl(T new_element, ForwardList<T>& fl) {
    Node_Fl<T>* new_node = new Node_Fl<T>;
    new_node->data = new T(new_element);
    new_node->next = nullptr;
    if (fl.head == nullptr) {
        fl.head = new_node;
    } else {
        Node_Fl<T>* current = fl.head;
        while (current->next != nullptr) {
            current = current->next;
        }
        current->next = new_node;
    }
}

template<typename T>
void add_element_before_fl(T new_element, ForwardList<T>& fl, Node_Fl<T>* node) {
    if (node == nullptr) {
        cerr << "Ошибка: передан нулевой указатель на узел" << endl;
        return;
    }
    if (fl.head == nullptr) {
        cerr << "Ошибка: список пуст" << endl;
        return;
    }
    if (node == fl.head) {
        Node_Fl<T>* new_node = new Node_Fl<T>;
        new_node->data = new T(new_element);
        new_node->next = fl.head;
        fl.head = new_node;
        return;
    } else {
        Node_Fl<T>* current =fl.head;
        while (current != nullptr && current->next != node) {
            current = current->next;
        }
        if (current == nullptr) {
            cerr << "Ошибка: узел не найден" << endl;
            return;
        }
        Node_Fl<T>* new_node = new Node_Fl<T>;
        new_node->data = new T(new_element);
        new_node->next = current->next;
        current->next = new_node;
    }
}

template<typename T>
void add_element_after_fl(T new_element, ForwardList<T>& fl, Node_Fl<T>* node) {
    if (node == nullptr) {
        cerr << "Ошибка: передан нулевой указатель на узел" << endl;
        return;
    }
    if (fl.head == nullptr) {
        cerr << "Ошибка: список пуст" << endl;
        return;
    }
    Node_Fl<T>* new_node = new Node_Fl<T>;
    new_node->data = new T(new_element);
    new_node->next = node->next;
    node->next = new_node;
}

template<typename T>
Node_Fl<T>* find_element_fl(T element, ForwardList<T>& fl) {
    Node_Fl<T>* current = fl.head;
    while (current != nullptr) {
        if (*(current->data) == element) {
            return current;
        }
        current = current->next;
    }
    cerr << "Ошибка: узел не найден" << endl;
    return nullptr;
}

template<typename T>
void delete_element_fl(ForwardList<T>& fl, Node_Fl<T>* node) {
    if (node == nullptr) {
        cerr << "Ошибка: передан нулевой указатель на узел" << endl;
        return;
    }
    Node_Fl<T>* current = fl.head;
    while (current != nullptr && current->next != node) {
        current = current->next;
    }
    if (current == nullptr) {
        cerr << "Ошибка: узел не найден" << endl;
        return;
    }
    current->next = node->next;
    delete node->data;
    delete node;
}

template<typename T>
void print_flist_while_fl(ForwardList<T>& fl) {
    Node_Fl<T>* current = fl.head;
    if (current == nullptr) {
        cout << "Список пуст" << endl;
        return;
    }
    while (current != nullptr) {
        cout << "Значение: " << *(current->data) << "; адрес: " << current << "; адрес следующего узла: " << current->next << endl;
        current = current->next;
    }
}

template<typename T>
void print_flist_for_fl(ForwardList<T>& fl) {
    Node_Fl<T>* current = fl.head;
    if (current == nullptr) {
        cout << "Список пуст" << endl;
        return;
    }
    for (Node_Fl<T>* current = fl.head; current != nullptr; current = current->next) {
        cout << "Значение: " << *(current->data) << "; адрес: " << current << "; адрес следующего узла: " << current->next << endl;
    }
}
