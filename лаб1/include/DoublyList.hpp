#pragma once

#include <iostream>
using namespace std;

template<typename T>
struct Node_Dl {
    T* data = nullptr;
    Node_Dl<T>* next = nullptr;
    Node_Dl<T>* prev = nullptr;
};

template<typename T>
struct DoublyList {
    Node_Dl<T>* head = nullptr;
    Node_Dl<T>* tail = nullptr;
};

template<typename T>
DoublyList<T> create_doublyList() {
    DoublyList<T> new_dl;
    new_dl.head = nullptr;
    new_dl.tail = nullptr;
    return new_dl;
} 

template<typename T>
void add_element_front_dl(T new_element, DoublyList<T>& dl) {
    Node_Dl<T>* new_node = new Node_Dl<T>;
    new_node->data = new T(new_element);
    new_node->next = dl.head;
    new_node->prev = nullptr;
    if (dl.head != nullptr) {
        dl.head->prev = new_node;
    }
    dl.head = new_node;
    if (dl.tail == nullptr) {
        dl.tail = new_node;
    }
}

template<typename T>
void add_element_back_dl(T new_element, DoublyList<T>& dl) {
    Node_Dl<T>* new_node = new Node_Dl<T>;
    new_node->data = new T(new_element);
    new_node->next = nullptr;
    new_node->prev = dl.tail;
    if (dl.head == nullptr) {
        dl.head = new_node;
        dl.tail = new_node;
    } else {
        dl.tail->next = new_node;
        dl.tail = new_node;
    }
}

template<typename T>
void add_element_before_dl(T new_element, DoublyList<T>& dl, Node_Dl<T>* node) {
    if (node == nullptr) {
        cerr << "Ошибка: передан нулевой указатель на узел" << endl;
        return;
    }
    if (dl.head == nullptr) {
        cerr << "Ошибка: список пуст" << endl;
        return;
    }
    Node_Dl<T>* current = dl.head;
    bool found = false;
    while (current != nullptr) {
        if (current == node) {
            found = true;
            break;
        }
        current = current->next;
    }
    if (!found) {
        cerr << "Ошибка: узел не найден в списке" << endl;
        return;
    }
    Node_Dl<T>* new_node = new Node_Dl<T>;
    new_node->data = new T(new_element);
    new_node->next = node;
    new_node->prev = node->prev;
    if (node->prev != nullptr) {
        node->prev->next = new_node;
    } else {
        dl.head = new_node;
    }
    node->prev = new_node;
}

template<typename T>
void add_element_after_dl(T new_element, DoublyList<T>& dl, Node_Dl<T>* node) {
    if (node == nullptr) {
        cerr << "Ошибка: передан нулевой указатель на узел" << endl;
        return;
    }
    if (dl.head == nullptr) {
        cerr << "Ошибка: список пуст" << endl;
        return;
    }
    Node_Dl<T>* current = dl.head;
    bool found = false;
    while (current != nullptr) {
        if (current == node) {
            found = true;
            break;
        }
        current = current->next;
    }
    if (!found) {
        cerr << "Ошибка: узел не найден в списке" << endl;
        return;
    }
    Node_Dl<T>* new_node = new Node_Dl<T>;
    new_node->data = new T(new_element);
    new_node->next = node->next;
    new_node->prev = node;
    if (node->next != nullptr) {
        node->next->prev = new_node;
    } else {
        dl.tail = new_node;
    }
    node->next = new_node;
}

// Удаление элемента с начала списка
template<typename T>
void delete_front_dl(DoublyList<T>& dl) {
    if (dl.head == nullptr) {
        cerr << "Ошибка: список пуст" << endl;
        return;
    }

    Node_Dl<T>* to_delete = dl.head;
    dl.head = dl.head->next;
    if (dl.head != nullptr) {
        dl.head->prev = nullptr;
    } else {
        dl.tail = nullptr; // если был один элемент
    }

    delete to_delete->data;
    delete to_delete;
}

// Удаление элемента с конца списка
template<typename T>
void delete_back_dl(DoublyList<T>& dl) {
    if (dl.tail == nullptr) {
        cerr << "Ошибка: список пуст" << endl;
        return;
    }

    Node_Dl<T>* to_delete = dl.tail;
    dl.tail = dl.tail->prev;
    if (dl.tail != nullptr) {
        dl.tail->next = nullptr;
    } else {
        dl.head = nullptr; // если был один элемент
    }

    delete to_delete->data;
    delete to_delete;
}

template<typename T>
Node_Dl<T>* find_element_dl(T element, DoublyList<T>& dl) {
    Node_Dl<T>* current = dl.head;
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
void delete_element_dl(DoublyList<T>& dl, Node_Dl<T>* node) {
    if (node == nullptr) {
        cerr << "Ошибка: передан нулевой указатель на узел" << endl;
        return;
    }
    if (dl.head == nullptr) {
        cerr << "Ошибка: список пуст" << endl;
        return;
    }
    Node_Dl<T>* current = dl.head;
    bool found = false;
    while (current != nullptr) {
        if (current == node) {
            found = true;
            break;
        }
        current = current->next;
    }
    if (!found) {
        cerr << "Ошибка: узел не найден в списке" << endl;
        return;
    }
    if (node->prev != nullptr) {
        node->prev->next = node->next;
    } else {
        dl.head = node->next;
    }
    if (node->next != nullptr) {
        node->next->prev = node->prev;
    } else {
        dl.tail = node->prev;
    }
    delete node->data;
    delete node;
}

template<typename T>
void print_dlist_while_dl(DoublyList<T>& dl) {
    Node_Dl<T>* current = dl.head;
    if (current == nullptr) {
        cout << "Список пуст" << endl;
        return;
    }
    while (current != nullptr) {
        cout << "Значение: " << *(current->data) << "; адрес: " << current << "; адрес следующего узла: " << current->next << "; адрес предыдущего  узла: " << current->prev  << endl;
        current = current->next;
    }
}

template<typename T>
void delete_before_element_dl(T element, DoublyList<T>& dl) {
    if (dl.head == nullptr) {
        cerr << "Ошибка: список пуст" << endl;
        return;
    }

    Node_Dl<T>* current = dl.head;
    while (current != nullptr) {
        if (*(current->data) == element) {
            if (current->prev == nullptr) {
                cerr << "Ошибка: перед данным элементом ничего нет" << endl;
                return;
            }
            Node_Dl<T>* to_delete = current->prev;
            if (to_delete->prev != nullptr) {
                to_delete->prev->next = current;
                current->prev = to_delete->prev;
            } else {
                dl.head = current;
                current->prev = nullptr;
            }
            delete to_delete->data;
            delete to_delete;
            return;
        }
        current = current->next;
    }

    cerr << "Ошибка: элемент не найден в списке" << endl;
}

template<typename T>
void delete_after_element_dl(T element, DoublyList<T>& dl) {
    if (dl.head == nullptr) {
        cerr << "Ошибка: список пуст" << endl;
        return;
    }

    Node_Dl<T>* current = dl.head;
    while (current != nullptr) {
        if (*(current->data) == element) {
            if (current->next == nullptr) {
                cerr << "Ошибка: после данного элемента ничего нет" << endl;
                return;
            }
            Node_Dl<T>* to_delete = current->next;
            current->next = to_delete->next;
            if (to_delete->next != nullptr) {
                to_delete->next->prev = current;
            } else {
                dl.tail = current;
            }
            delete to_delete->data;
            delete to_delete;
            return;
        }
        current = current->next;
    }

    cerr << "Ошибка: элемент не найден в списке" << endl;
}

template<typename T>
void print_dlist_for_dl(DoublyList<T>& dl) {
    Node_Dl<T>* current = dl.head;
    if (current == nullptr) {
        cout << "Список пуст" << endl;
        return;
    }
    for (current; current != nullptr; current = current->next) {
        cout << "Значение: " << *(current->data) << "; адрес: " << current << "; адрес следующего узла: " << current->next << "; адрес предыдущего  узла: " << current->prev  << endl;
    }
}