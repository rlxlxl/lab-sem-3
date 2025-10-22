#pragma once

#include "Queue.hpp"
#include "Stack.hpp"
#include "Array.hpp"
#include "DoublyList.hpp"
#include "ForwardList.hpp"
#include "SystemWork.hpp"
#include "FullBinaryTree.hpp"
using namespace std;


void cmd_Mpush(const Array<string>& toks) {
    if (toks.max_index < 3) { cerr << "MPUSH требует имя и значение\n"; return; }
    string name = toks.data[1];
    string value = toks.data[2];
    for (int i = 3; i < toks.max_index; ++i) value += " " + toks.data[i];

    int idx = find_name_index(names_M, name);
    Array<string>* arr;
    if (idx == -1) {
        arr = new Array<string>(create_array_ar<string>(4));
        add_named_container<Array<string>*>(names_M, data_M, name, arr);
    } else arr = data_M.data[idx];
    add_element_end_ar<string>(*arr, value);
    cout << "OK\n";
    save_db(g_file_path);
}

void cmd_Mget(const Array<string>& toks) {
    if (toks.max_index < 3) { cerr << "MGET требует имя и индекс\n"; return; }
    string name = toks.data[1];
    int id = parse_index(toks.data[2]);
    if (id < 0) { cerr << "Неправильный индекс\n"; return; } // Изменено: 0 вместо 1
    int idx = find_name_index(names_M, name);
    if (idx == -1) { cerr << "Массив не найден\n"; return; }
    Array<string>* arr = data_M.data[idx];
    if (arr->max_index <= id || !arr->is_set[id]) { cerr << "Индекс вне диапазона\n"; return; } // Изменено: id вместо id-1
    cout << arr->data[id] << "\n"; // Изменено: id вместо id-1
}

// MDEL - индексация с 0
void cmd_Mdel(const Array<string>& toks) {
    if (toks.max_index < 3) { cerr << "MDEL требует имя и индекс\n"; return; }
    string name = toks.data[1];
    int id = parse_index(toks.data[2]);
    if (id < 0) { cerr << "Неправильный индекс\n"; return; } // Изменено: 0 вместо 1
    int idx = find_name_index(names_M, name);
    if (idx == -1) { cerr << "Массив не найден\n"; return; }
    Array<string>* arr = data_M.data[idx];
    if (arr->max_index <= id || !arr->is_set[id]) { cerr << "Индекс вне диапазона\n"; return; } // Изменено: id вместо id-1
    delete_element_index_ar<string>(*arr, id + 1); // Изменено: +1 т.к. функция ожидает 1-based
    cout << "OK\n";
    save_db(g_file_path);
}

void cmd_Mset(const Array<string>& toks) {
    if (toks.max_index < 4) {
        cerr << "MSET требует имя, индекс и новое значение\n";
        return;
    }

    string name = toks.data[1];
    int id = parse_index(toks.data[2]);
    if (id < 0) {
        cerr << "Неправильный индекс\n";
        return;
    }

    string new_value = toks.data[3];
    for (int i = 4; i < toks.max_index; ++i) {
        new_value += " " + toks.data[i];
    }

    int idx = find_name_index(names_M, name);
    if (idx == -1) {
        cerr << "Массив не найден\n";
        return;
    }

    Array<string>* arr = data_M.data[idx];
    if (id >= arr->max_index || !arr->is_set[id]) {
        cerr << "Индекс вне диапазона или элемент не установлен\n";
        return;
    }

    change_element_index_ar<string>(*arr, id + 1, new_value); // +1 — т.к. функция 1-based
    cout << "OK\n";
    save_db(g_file_path);
}

// MINDEX - вставка по индексу (0-based)
void cmd_Mindex(const Array<string>& toks) {
    if (toks.max_index < 4) {
        cerr << "MINDEX требует имя, индекс и значение\n";
        return;
    }

    string name = toks.data[1];
    int id = parse_index(toks.data[2]);
    if (id < 0) {
        cerr << "Неправильный индекс\n";
        return;
    }

    string value = toks.data[3];
    for (int i = 4; i < toks.max_index; ++i) {
        value += " " + toks.data[i];
    }

    int idx = find_name_index(names_M, name);
    Array<string>* arr;
    if (idx == -1) {
        arr = new Array<string>(create_array_ar<string>(id + 1));
        add_named_container<Array<string>*>(names_M, data_M, name, arr);
    } else {
        arr = data_M.data[idx];
    }

    add_element_index_ar<string>(*arr, id + 1, value); // +1 — 1-based
    cout << "OK\n";
    save_db(g_file_path);
}

// MSIZE - размер массива
void cmd_Msize(const Array<string>& toks) {
    if (toks.max_index < 2) {
        cerr << "MSIZE требует имя\n";
        return;
    }

    string name = toks.data[1];
    int idx = find_name_index(names_M, name);
    if (idx == -1) {
        cerr << "Массив не найден\n";
        return;
    }

    Array<string>* arr = data_M.data[idx];
    cout << arr->max_index << "\n";
}

// MCAP - вместимость массива
void cmd_Mcap(const Array<string>& toks) {
    if (toks.max_index < 2) {
        cerr << "MCAP требует имя\n";
        return;
    }

    string name = toks.data[1];
    int idx = find_name_index(names_M, name);
    if (idx == -1) {
        cerr << "Массив не найден\n";
        return;
    }

    Array<string>* arr = data_M.data[idx];
    cout << arr->capacity << "\n";
}

void cmd_Fpush(const Array<string>& toks) {
    if (toks.max_index < 3) { cerr << "FPUSH требует имя и значение\n"; return; }
    string name = toks.data[1];
    string value = toks.data[2];
    for (int i = 3; i < toks.max_index; ++i) value += " " + toks.data[i];

    bool front = false;
    if (toks.max_index > 3 && toks.data[3] == "front") front = true;

    int idx = find_name_index(names_F, name);
    ForwardList<string>* fl;
    if (idx == -1) {
        fl = new ForwardList<string>(create_forwardList<string>());
        add_named_container<ForwardList<string>*>(names_F, data_F, name, fl);
    } else fl = data_F.data[idx];

    if (front) add_element_front_fl(value, *fl);
    else add_element_back_fl(value, *fl);

    cout << "OK\n";
    save_db(g_file_path);
}

void cmd_Fget(const Array<string>& toks) {
    if (toks.max_index < 3) { cerr << "FGET требует имя и индекс\n"; return; }
    string name = toks.data[1];
    int id = parse_index(toks.data[2]);
    if (id < 0) { cerr << "Неправильный индекс\n"; return; }

    int idx = find_name_index(names_F, name);
    if (idx == -1) { cerr << "Список не найден\n"; return; }

    ForwardList<string>* fl = data_F.data[idx];
    Node_Fl<string>* cur = fl->head;
    int pos = 0;
    while (cur != nullptr && pos < id) { cur = cur->next; ++pos; }

    if (cur == nullptr) { cerr << "Индекс вне диапазона\n"; return; }
    cout << *(cur->data) << "\n";
}

void cmd_Fdel(const Array<string>& toks) {
    if (toks.max_index < 3) { cerr << "FDEL требует имя и индекс\n"; return; }
    string name = toks.data[1];
    int id = parse_index(toks.data[2]);
    if (id < 0) { cerr << "Неправильный индекс\n"; return; }

    int idx = find_name_index(names_F, name);
    if (idx == -1) { cerr << "Список не найден\n"; return; }

    ForwardList<string>* fl = data_F.data[idx];
    if (fl->head == nullptr) { cerr << "Список пуст\n"; return; }

    Node_Fl<string>* cur = fl->head;
    if (id == 0) {
        Node_Fl<string>* todel = fl->head;
        fl->head = todel->next;
        delete todel->data;
        delete todel;
        cout << "OK\n";
        save_db(g_file_path);
        return;
    }

    int pos = 0;
    while (cur != nullptr && pos < id-1) { cur = cur->next; ++pos; }
    if (cur == nullptr || cur->next == nullptr) { cerr << "Индекс вне диапазона\n"; return; }

    Node_Fl<string>* todel = cur->next;
    cur->next = todel->next;
    delete todel->data;
    delete todel;
    cout << "OK\n";
    save_db(g_file_path);
}

void cmd_Finsert_before(const Array<string>& toks) {
    if (toks.max_index < 4) { cerr << "FININSERT_BEFORE требует имя, индекс и значение\n"; return; }
    string name = toks.data[1];
    int id = parse_index(toks.data[2]);
    if (id < 0) { cerr << "Неправильный индекс\n"; return; }

    string value = toks.data[3];
    for (int i=4; i<toks.max_index; ++i) value += " " + toks.data[i];

    int idx = find_name_index(names_F, name);
    if (idx == -1) { cerr << "Список не найден\n"; return; }

    ForwardList<string>* fl = data_F.data[idx];
    Node_Fl<string>* cur = fl->head;
    int pos = 0;
    while (cur != nullptr && pos < id) { cur = cur->next; ++pos; }
    if (cur == nullptr) { cerr << "Индекс вне диапазона\n"; return; }

    add_element_before_fl(value, *fl, cur);
    cout << "OK\n";
    save_db(g_file_path);
}

void cmd_Finsert_after(const Array<string>& toks) {
    if (toks.max_index < 4) { cerr << "FININSERT_AFTER требует имя, индекс и значение\n"; return; }
    string name = toks.data[1];
    int id = parse_index(toks.data[2]);
    if (id < 0) { cerr << "Неправильный индекс\n"; return; }

    string value = toks.data[3];
    for (int i=4; i<toks.max_index; ++i) value += " " + toks.data[i];

    int idx = find_name_index(names_F, name);
    if (idx == -1) { cerr << "Список не найден\n"; return; }

    ForwardList<string>* fl = data_F.data[idx];
    Node_Fl<string>* cur = fl->head;
    int pos = 0;
    while (cur != nullptr && pos < id) { cur = cur->next; ++pos; }
    if (cur == nullptr) { cerr << "Индекс вне диапазона\n"; return; }

    add_element_after_fl(value, *fl, cur);
    cout << "OK\n";
    save_db(g_file_path);
}

void cmd_PRINT_F(const Array<string>& toks) {
    if (toks.max_index < 2) { cerr << "PRINT требует имя\n"; return; }

    string name = toks.data[1];
    int idx = find_name_index(names_F, name);
    if (idx == -1) { cerr << "Список не найден\n"; return; }

    ForwardList<string>* fl = data_F.data[idx];
    Node_Fl<string>* cur = fl->head;
    while (cur != nullptr) {
        cout << *(cur->data) << " ";
        cur = cur->next;
    }
    cout << "\n";
}


void cmd_Lget(const Array<string>& toks) {
    if (toks.max_index < 3) { cerr << "LGET требует имя и индекс\n"; return; }
    string name = toks.data[1];
    int id = parse_index(toks.data[2]);
    if (id < 0) { cerr << "Неправильный индекс\n"; return; } // Изменено: 0 вместо 1
    int idx = find_name_index(names_L, name);
    if (idx == -1) { cerr << "Список не найден\n"; return; }
    DoublyList<string>* dl = data_L.data[idx];
    Node_Dl<string>* cur = dl->head;
    int pos = 0; // Изменено: 0 вместо 1
    while (cur != nullptr && pos < id) { cur = cur->next; ++pos; }
    if (cur == nullptr) { cerr << "Индекс вне диапазона\n"; return; }
    cout << *(cur->data) << "\n";
}

// LDEL - индексация с 0
void cmd_Ldel(const Array<string>& toks) {
    if (toks.max_index < 3) { cerr << "LDEL требует имя и индекс\n"; return; }
    string name = toks.data[1];
    int id = parse_index(toks.data[2]);
    if (id < 0) { cerr << "Неправильный индекс\n"; return; } // Изменено: 0 вместо 1
    int idx = find_name_index(names_L, name);
    if (idx == -1) { cerr << "Список не найден\n"; return; }
    DoublyList<string>* dl = data_L.data[idx];
    if (dl->head == nullptr) { cerr << "Список пуст\n"; return; }
    Node_Dl<string>* cur = dl->head;
    int pos = 0; // Изменено: 0 вместо 1
    while (cur != nullptr && pos < id) { cur = cur->next; ++pos; }
    if (cur == nullptr) { cerr << "Индекс вне диапазона\n"; return; }
    // unlink
    if (cur->prev != nullptr) cur->prev->next = cur->next; else dl->head = cur->next;
    if (cur->next != nullptr) cur->next->prev = cur->prev; else dl->tail = cur->prev;
    delete cur->data;
    delete cur;
    cout << "OK\n";
    save_db(g_file_path);
}

void cmd_Linsert_after(const Array<string>& toks) {
    if (toks.max_index < 4) { 
        cerr << "LINSERT_AFTER требует имя, индекс и значение\n"; 
        return; 
    }

    string name = toks.data[1];
    int idx_elem = parse_index(toks.data[2]);
    if (idx_elem < 0) { cerr << "Неправильный индекс\n"; return; }

    string value = toks.data[3];
    for (int i = 4; i < toks.max_index; ++i) value += " " + toks.data[i];

    int idx = find_name_index(names_L, name);
    if (idx == -1) { cerr << "Список не найден\n"; return; }

    DoublyList<string>* dl = data_L.data[idx];
    Node_Dl<string>* cur = dl->head;
    int pos = 0;
    while (cur != nullptr && pos < idx_elem) { cur = cur->next; ++pos; }
    if (cur == nullptr) { cerr << "Индекс вне диапазона\n"; return; }

    add_element_after_dl(value, *dl, cur);
    cout << "OK\n";
    save_db(g_file_path);
}

void cmd_Linsert_before(const Array<string>& toks) {
    if (toks.max_index < 4) { 
        cerr << "LINSERT_BEFORE требует имя, индекс и значение\n"; 
        return; 
    }

    string name = toks.data[1];
    int idx_elem = parse_index(toks.data[2]);
    if (idx_elem < 0) { cerr << "Неправильный индекс\n"; return; }

    string value = toks.data[3];
    for (int i = 4; i < toks.max_index; ++i) value += " " + toks.data[i];

    int idx = find_name_index(names_L, name);
    if (idx == -1) { cerr << "Список не найден\n"; return; }

    DoublyList<string>* dl = data_L.data[idx];
    Node_Dl<string>* cur = dl->head;
    int pos = 0;
    while (cur != nullptr && pos < idx_elem) { cur = cur->next; ++pos; }
    if (cur == nullptr) { cerr << "Индекс вне диапазона\n"; return; }

    add_element_before_dl(value, *dl, cur);
    cout << "OK\n";
    save_db(g_file_path);
}

void cmd_Lpush(const Array<string>& toks) {
    if (toks.max_index < 3) { cerr << "LPUSH требует имя и значение\n"; return; }

    string name = toks.data[1];
    string value = toks.data[2];

    int i = 3;
    // собираем значение из нескольких токенов, пока не встретили "front" или "back"
    string pos = "back"; // по умолчанию хвост
    for (; i < toks.max_index; ++i) {
        if (toks.data[i] == "front" || toks.data[i] == "back") {
            pos = toks.data[i];
            break;
        }
        value += " " + toks.data[i];
    }

    int idx = find_name_index(names_L, name);
    DoublyList<string>* dl;
    if (idx == -1) {
        dl = new DoublyList<string>(create_doublyList<string>());
        add_named_container<DoublyList<string>*>(names_L, data_L, name, dl);
    } else dl = data_L.data[idx];

    if (pos == "front") add_element_front_dl(value, *dl);
    else add_element_back_dl(value, *dl);

    cout << "OK\n";
    save_db(g_file_path);
}

void cmd_Lfind(const Array<string>& toks) {
    if (toks.max_index < 3) { cerr << "LFIND требует имя и значение\n"; return; }
    string name = toks.data[1];
    string value = toks.data[2];
    for (int i = 3; i < toks.max_index; ++i) value += " " + toks.data[i];

    int idx = find_name_index(names_L, name);
    if (idx == -1) { cerr << "Список не найден\n"; return; }

    DoublyList<string>* dl = data_L.data[idx];
    Node_Dl<string>* found = find_element_dl(value, *dl);
    if (found) {
        cout << "Элемент найден по адресу: " << found << "\n";
    } else {
        cout << "Элемент не найден\n";
    }
}

void cmd_Lprint(const Array<string>& toks) {
    if (toks.max_index < 2) { cerr << "LPRINT требует имя\n"; return; }
    string name = toks.data[1];
    int idx = find_name_index(names_L, name);
    if (idx == -1) { cerr << "Список не найден\n"; return; }

    DoublyList<string>* dl = data_L.data[idx];
    Node_Dl<string>* cur = dl->head;
    while (cur != nullptr) {
        cout << *(cur->data) << " ";
        cur = cur->next;
    }
    cout << "\n";
}

// QPUSH / QPOP / QGET
void cmd_Qpush(const Array<string>& toks) {
    if (toks.max_index < 3) { cerr << "QPUSH требует имя и значение\n"; return; }
    string name = toks.data[1];
    string value = toks.data[2];
    for (int i = 3; i < toks.max_index; ++i) value += " " + toks.data[i];
    int idx = find_name_index(names_Q, name);
    Queue<string>* q;
    if (idx == -1) {
        q = new Queue<string>(create_queue<string>());
        add_named_container<Queue<string>*>(names_Q, data_Q, name, q);
    } else q = data_Q.data[idx];
    push_queue<string>(*q, value);
    cout << "OK\n";
    save_db(g_file_path);
}

void cmd_Qpop(const Array<string>& toks) {
    if (toks.max_index < 2) { cerr << "QPOP требует имя\n"; return; }
    string name = toks.data[1];
    int idx = find_name_index(names_Q, name);
    if (idx == -1) { cout << "-> (queue empty)\n"; return; }
    Queue<string>* q = data_Q.data[idx];
    if (q->size == 0) { cout << "-> (queue empty)\n"; return; }
    string v = pop_queue<string>(*q);
    cout << "-> " << v << "\n";
    save_db(g_file_path);
}

void cmd_Qget(const Array<string>& toks) {
    if (toks.max_index < 3) { cerr << "QGET требует имя и индекс\n"; return; }
    string name = toks.data[1];
    int id = parse_index(toks.data[2]);
    if (id < 0) { cerr << "Неправильный индекс\n"; return; } // Изменено: 0 вместо 1
    int idx = find_name_index(names_Q, name);
    if (idx == -1) { cerr << "Очередь не найдена\n"; return; }
    Queue<string>* q = data_Q.data[idx];
    if (q->size <= id) { cerr << "Индекс вне диапазона\n"; return; } // Изменено: <= вместо <
    int at = (q->front + id) % q->capacity; // Изменено: id вместо id-1
    cout << q->data[at] << "\n";
}

// SPUSH / SPOP / SGET - ПЕРЕРАБОТАНО ДЛЯ НОВОЙ РЕАЛИЗАЦИИ СТЕКА
void cmd_Spush(const Array<string>& toks) {
    if (toks.max_index < 3) { cerr << "SPUSH требует имя и значение\n"; return; }
    string name = toks.data[1];
    string value = toks.data[2];
    for (int i = 3; i < toks.max_index; ++i) value += " " + toks.data[i];
    int idx = find_name_index(names_S, name);
    Stack<string>* s;
    if (idx == -1) {
        s = new Stack<string>(create_stack<string>());
        add_named_container<Stack<string>*>(names_S, data_S, name, s);
    } else s = data_S.data[idx];
    push_stack<string>(*s, value);
    cout << "OK\n";
    save_db(g_file_path);
}

void cmd_Spop(const Array<string>& toks) {
    if (toks.max_index < 2) { 
        cerr << "SPOP требует имя\n"; 
        return; 
    }
    string name = toks.data[1];
    int idx = find_name_index(names_S, name);
    if (idx == -1) { 
        cout << "-> (stack empty)\n"; 
        return; 
    }

    Stack<string>* s = data_S.data[idx];
    if (s->top == nullptr) {  // ИЗМЕНЕНО: проверка на nullptr вместо top_index
        cout << "-> (stack empty)\n"; 
        return; 
    }

    string v = pop_stack(*s);
    cout << "-> " << v << "\n";
    save_db(g_file_path);
}

void cmd_Sget(const Array<string>& toks) {
    if (toks.max_index < 3) { cerr << "SGET требует имя и индекс\n"; return; }
    string name = toks.data[1];
    int id = parse_index(toks.data[2]);
    if (id < 0) { cerr << "Неправильный индекс\n"; return; } // Изменено: 0 вместо 1
    int idx = find_name_index(names_S, name);
    if (idx == -1) { cerr << "Стек не найден\n"; return; }
    Stack<string>* s = data_S.data[idx];
    
    if (s->top == nullptr) {
        cerr << "Стек пуст\n";
        return;
    }
    
    // Индекс 0 = верхний элемент стека
    StackNode<string>* current = s->top;
    int pos = 0; // Изменено: 0 вместо 1
    
    // Переходим к нужному элементу (индексы считаются от вершины)
    while (current != nullptr && pos < id) {
        current = current->next;
        pos++;
    }
    
    if (current == nullptr) {
        cerr << "Индекс вне диапазона\n";
        return;
    }
    
    cout << current->data << "\n";
}

// === ВСПОМОГАТЕЛЬНАЯ ФУНКЦИЯ ===
int find_tree_index(const string& name) {
    for (int i = 0; i < names_T.max_index; ++i)
        if (names_T.is_set[i] && names_T.data[i] == name)
            return i;
    return -1;
}

// === TINSERT ===
void cmd_Tinsert(const Array<string>& toks) {
    if (toks.max_index < 3) { cerr << "TINSERT требует имя и значение\n"; return; }

    string name = toks.data[1];
    
    int idx = find_name_index(names_T, name);
    FullBinaryTree* tree;

    if (idx == -1) {
        tree = new FullBinaryTree(create_fbt());
        add_named_container<FullBinaryTree*>(names_T, data_T, name, tree);
    } else {
        tree = data_T.data[idx];
    }

    // Обрабатываем все значения начиная с индекса 2
    for (int i = 2; i < toks.max_index; ++i) {
        try {
            int value = stoi(toks.data[i]);
            add_element_fbt(*tree, value);
        } catch (...) {
            cerr << "Ошибка: значение '" << toks.data[i] << "' должно быть целым числом\n";
        }
    }
    
    cout << "OK\n";
    save_db(g_file_path);
}

// TGET tree_name
void cmd_Tget(const Array<string>& toks) {
    if (toks.max_index < 2) { cerr << "TGET требует имя\n"; return; }

    string name = toks.data[1];
    int idx = find_name_index(names_T, name);
    if (idx == -1) { cerr << "Дерево не найдено\n"; return; }

    FullBinaryTree* tree = data_T.data[idx];
    cout << "TREE " << name << ": ";
    print_fbt(tree->root);
}

// TFULL tree_name
void cmd_Tfull(const Array<string>& toks) {
    if (toks.max_index < 2) { cerr << "TFULL требует имя\n"; return; }

    string name = toks.data[1];
    int idx = find_name_index(names_T, name);
    if (idx == -1) { cerr << "Дерево не найдено\n"; return; }

    FullBinaryTree* tree = data_T.data[idx];
    cout << "Full? " << (is_full_fbt(*tree) ? "Да" : "Нет") << "\n";
}

// ISMEMBER tree_name value
void cmd_Tismember(const Array<string>& toks) {
    if (toks.max_index < 3) { cerr << "ISMEMBER требует имя и значение\n"; return; }

    string name = toks.data[1];
    string value_str = toks.data[2];
    for (int i = 3; i < toks.max_index; ++i) value_str += " " + toks.data[i];

    int idx = find_name_index(names_T, name);
    if (idx == -1) { cerr << "Дерево не найдено\n"; return; }

    FullBinaryTree* tree = data_T.data[idx];
    
    try {
        int value = stoi(value_str);
        cout << (find_element_fbt(tree->root, value) ? "1\n" : "0\n");
    } catch (...) {
        cerr << "Ошибка: значение должно быть целым числом\n";
    }
}

void cmd_Tpretty(const Array<string>& toks) {
    if (toks.max_index < 2) { 
        cerr << "TPRETTY требует имя\n"; 
        return; 
    }

    string name = toks.data[1];
    int idx = find_name_index(names_T, name);
    if (idx == -1) { 
        cerr << "Дерево не найдено\n"; 
        return; 
    }

    FullBinaryTree* tree = data_T.data[idx];
    cout << "Дерево '" << name << "':\n";
    
    cout << "=== Красивый вывод ===\n";
    print_tree_pretty(tree->root);
}

void cmd_PRINT(const Array<string>& toks) {
    if (toks.max_index < 2) { 
        cerr << "PRINT требует имя\n"; 
        return; 
    }
    string name = toks.data[1];

    // массив
    int idx = find_name_index(names_M, name);
    if (idx != -1) {
        print_array_ar(*data_M.data[idx]);
        return;
    }

    // односвязный список
    idx = find_name_index(names_F, name);
    if (idx != -1) {
        ForwardList<string>* fl = data_F.data[idx];
        Node_Fl<string>* cur = fl->head;
        while (cur != nullptr) {
            cout << *(cur->data) << " ";
            cur = cur->next;
        }
        cout << "\n";
        return;
    }

    // двусвязный список
    idx = find_name_index(names_L, name);
    if (idx != -1) {
        DoublyList<string>* dl = data_L.data[idx];
        Node_Dl<string>* cur = dl->head;
        while (cur != nullptr) {
            cout << *(cur->data) << " ";
            cur = cur->next;
        }
        cout << "\n";
        return;
    }

    // очередь
    idx = find_name_index(names_Q, name);
    if (idx != -1) {
        Queue<string>* q = data_Q.data[idx];
        for (int i = 0; i < q->size; i++) {
            int at = (q->front + i) % q->capacity;
            cout << q->data[at] << " ";
        }
        cout << "\n";
        return;
    }

    // стек - ИЗМЕНЕНО для новой реализации
    idx = find_name_index(names_S, name);
    if (idx != -1) {
        Stack<string>* s = data_S.data[idx];
        StackNode<string>* current = s->top;
        while (current != nullptr) {
            cout << current->data << " ";  // ИЗМЕНЕНО: прямой доступ к data
            current = current->next;
        }
        cout << "\n";
        return;
    }

    cerr << "Контейнер с именем " << name << " не найден\n";
}