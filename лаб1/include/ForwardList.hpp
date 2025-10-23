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

// Удаляет первый элемент списка
template<typename T>
void delete_front_fl(ForwardList<T>& fl) {
    if (!fl.head) {
        cerr << "Ошибка: список пуст" << endl;
        return;
    }
    Node_Fl<T>* todel = fl.head;
    fl.head = fl.head->next;
    delete todel->data;
    delete todel;
}

template<typename T>
void delete_by_value_fl(ForwardList<T>& fl, const T& value) {
    if (!fl.head) {
        cerr << "Ошибка: список пуст" << endl;
        return;
    }

    // Если удаляем голову
    if (*(fl.head->data) == value) {
        Node_Fl<T>* todel = fl.head;
        fl.head = fl.head->next;
        delete todel->data;
        delete todel;
        return;
    }

    Node_Fl<T>* current = fl.head;
    while (current->next != nullptr && *(current->next->data) != value) {
        current = current->next;
    }

    if (current->next == nullptr) {
        cerr << "Ошибка: элемент не найден" << endl;
        return;
    }

    // Удаляем найденный узел
    Node_Fl<T>* todel = current->next;
    current->next = todel->next;
    delete todel->data;
    delete todel;
}

// Удаляет последний элемент списка
template<typename T>
void delete_back_fl(ForwardList<T>& fl) {
    if (!fl.head) {
        cerr << "Ошибка: список пуст" << endl;
        return;
    }
    if (!fl.head->next) { // если только один элемент
        delete fl.head->data;
        delete fl.head;
        fl.head = nullptr;
        return;
    }
    Node_Fl<T>* current = fl.head;
    while (current->next->next != nullptr) {
        current = current->next;
    }
    // current теперь предпоследний узел
    Node_Fl<T>* todel = current->next;
    current->next = nullptr;
    delete todel->data;
    delete todel;
}

template<typename T>
void delete_element_before_fl(ForwardList<T>& fl, Node_Fl<T>* node) {
    if (!fl.head || !node) {
        cerr << "Ошибка: список пуст или узел равен nullptr" << endl;
        return;
    }
    if (node == fl.head) {
        cerr << "Ошибка: перед головой ничего нет" << endl;
        return;
    }
    // если перед головой, удалить нельзя, ищем предыдущий
    Node_Fl<T>* prev = nullptr;
    Node_Fl<T>* cur = fl.head;
    while (cur != nullptr && cur->next != node) {
        prev = cur;
        cur = cur->next;
    }
    if (cur == nullptr) {
        cerr << "Ошибка: узел не найден в списке" << endl;
        return;
    }
    if (!prev) { // удаляем голову
        fl.head = node;
        delete cur->data;
        delete cur;
    } else {
        prev->next = node;
        delete cur->data;
        delete cur;
    }
}

// Удаляет узел после указанного node
template<typename T>
void delete_element_after_fl(ForwardList<T>& fl, Node_Fl<T>* node) {
    if (!node || !node->next) {
        cerr << "Ошибка: нет узла после указанного" << endl;
        return;
    }
    Node_Fl<T>* todel = node->next;
    node->next = todel->next;
    delete todel->data;
    delete todel;
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
