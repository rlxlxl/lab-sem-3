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
    // build value from tokens[2..]
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
    if (id < 1) { cerr << "Неправильный индекс\n"; return; }
    int idx = find_name_index(names_M, name);
    if (idx == -1) { cerr << "Массив не найден\n"; return; }
    Array<string>* arr = data_M.data[idx];
    if (arr->max_index < id || !arr->is_set[id-1]) { cerr << "Индекс вне диапазона\n"; return; }
    cout << arr->data[id-1] << "\n";
}

void cmd_Mdel(const Array<string>& toks) {
    if (toks.max_index < 3) { cerr << "MDEL требует имя и индекс\n"; return; }
    string name = toks.data[1];
    int id = parse_index(toks.data[2]);
    if (id < 1) { cerr << "Неправильный индекс\n"; return; }
    int idx = find_name_index(names_M, name);
    if (idx == -1) { cerr << "Массив не найден\n"; return; }
    Array<string>* arr = data_M.data[idx];
    if (arr->max_index < id || !arr->is_set[id-1]) { cerr << "Индекс вне диапазона\n"; return; }
    delete_element_index_ar<string>(*arr, id);
    cout << "OK\n";
    save_db(g_file_path);
}

// FPUSH (back)
void cmd_Fpush(const Array<string>& toks) {
    if (toks.max_index < 3) { cerr << "FPUSH требует имя и значение\n"; return; }
    string name = toks.data[1];
    string value = toks.data[2];
    for (int i = 3; i < toks.max_index; ++i) value += " " + toks.data[i];
    int idx = find_name_index(names_F, name);
    ForwardList<string>* fl;
    if (idx == -1) {
        fl = new ForwardList<string>(create_forwardList<string>());
        add_named_container<ForwardList<string>*>(names_F, data_F, name, fl);
    } else fl = data_F.data[idx];
    add_element_back_fl<string>(value, *fl);
    cout << "OK\n";
    save_db(g_file_path);
}

void cmd_Fget(const Array<string>& toks) {
    if (toks.max_index < 3) { cerr << "FGET требует имя и индекс\n"; return; }
    string name = toks.data[1];
    int id = parse_index(toks.data[2]);
    if (id < 1) { cerr << "Неправильный индекс\n"; return; }
    int idx = find_name_index(names_F, name);
    if (idx == -1) { cerr << "Список не найден\n"; return; }
    ForwardList<string>* fl = data_F.data[idx];
    Node_Fl<string>* cur = fl->head;
    int pos = 1;
    while (cur != nullptr && pos < id) { cur = cur->next; ++pos; }
    if (cur == nullptr) { cerr << "Индекс вне диапазона\n"; return; }
    cout << *(cur->data) << "\n";
}

void cmd_Fdel(const Array<string>& toks) {
    if (toks.max_index < 3) { cerr << "FDEL требует имя и индекс\n"; return; }
    string name = toks.data[1];
    int id = parse_index(toks.data[2]);
    if (id < 1) { cerr << "Неправильный индекс\n"; return; }
    int idx = find_name_index(names_F, name);
    if (idx == -1) { cerr << "Список не найден\n"; return; }
    ForwardList<string>* fl = data_F.data[idx];
    if (fl->head == nullptr) { cerr << "Список пуст\n"; return; }
    if (id == 1) {
        Node_Fl<string>* todel = fl->head;
        fl->head = todel->next;
        delete todel->data;
        delete todel;
        cout << "OK\n";
        save_db(g_file_path);
        return;
    }
    Node_Fl<string>* cur = fl->head;
    int pos = 1;
    while (cur != nullptr && pos < id-1) { cur = cur->next; ++pos; }
    if (cur == nullptr || cur->next == nullptr) { cerr << "Индекс вне диапазона\n"; return; }
    Node_Fl<string>* todel = cur->next;
    cur->next = todel->next;
    delete todel->data;
    delete todel;
    cout << "OK\n";
    save_db(g_file_path);
}

// LPUSH (back)
void cmd_Lpush(const Array<string>& toks) {
    if (toks.max_index < 3) { cerr << "LPUSH требует имя и значение\n"; return; }
    string name = toks.data[1];
    string value = toks.data[2];
    for (int i = 3; i < toks.max_index; ++i) value += " " + toks.data[i];
    int idx = find_name_index(names_L, name);
    DoublyList<string>* dl;
    if (idx == -1) {
        dl = new DoublyList<string>(create_doublyList<string>());
        add_named_container<DoublyList<string>*>(names_L, data_L, name, dl);
    } else dl = data_L.data[idx];
    add_element_back_dl<string>(value, *dl);
    cout << "OK\n";
    save_db(g_file_path);
}

void cmd_Lget(const Array<string>& toks) {
    if (toks.max_index < 3) { cerr << "LGET требует имя и индекс\n"; return; }
    string name = toks.data[1];
    int id = parse_index(toks.data[2]);
    if (id < 1) { cerr << "Неправильный индекс\n"; return; }
    int idx = find_name_index(names_L, name);
    if (idx == -1) { cerr << "Список не найден\n"; return; }
    DoublyList<string>* dl = data_L.data[idx];
    Node_Dl<string>* cur = dl->head;
    int pos = 1;
    while (cur != nullptr && pos < id) { cur = cur->next; ++pos; }
    if (cur == nullptr) { cerr << "Индекс вне диапазона\n"; return; }
    cout << *(cur->data) << "\n";
}

void cmd_Ldel(const Array<string>& toks) {
    if (toks.max_index < 3) { cerr << "LDEL требует имя и индекс\n"; return; }
    string name = toks.data[1];
    int id = parse_index(toks.data[2]);
    if (id < 1) { cerr << "Неправильный индекс\n"; return; }
    int idx = find_name_index(names_L, name);
    if (idx == -1) { cerr << "Список не найден\n"; return; }
    DoublyList<string>* dl = data_L.data[idx];
    if (dl->head == nullptr) { cerr << "Список пуст\n"; return; }
    Node_Dl<string>* cur = dl->head;
    int pos = 1;
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
    if (id < 1) { cerr << "Неправильный индекс\n"; return; }
    int idx = find_name_index(names_Q, name);
    if (idx == -1) { cerr << "Очередь не найдена\n"; return; }
    Queue<string>* q = data_Q.data[idx];
    if (q->size < id) { cerr << "Индекс вне диапазона\n"; return; }
    int at = (q->front + (id - 1)) % q->capacity;
    cout << q->data[at] << "\n";
}

// SPUSH / SPOP / SGET
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
    if (s->top_index == 0) { 
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
    if (id < 1) { cerr << "Неправильный индекс\n"; return; }
    int idx = find_name_index(names_S, name);
    if (idx == -1) { cerr << "Стек не найден\n"; return; }
    Stack<string>* s = data_S.data[idx];
    if (s->top_index < id) { cerr << "Индекс вне диапазона\n"; return; }
    // определяем: индекс 1 == первый элемент (нижний). Чтобы вернуть по такому порядку:
    cout << s->data[id-1] << "\n";
}

// === ХРАНИЛИЩЕ ДЕРЕВЬЕВ ===
const int MAX_TREES = 16;
string tree_names[MAX_TREES];
FullBinaryTree<int>* trees[MAX_TREES];
int tree_count = 0;

// === ВСПОМОГАТЕЛЬНАЯ ФУНКЦИЯ ===
int find_tree_index(const string& name) {
    for (int i = 0; i < tree_count; ++i)
        if (tree_names[i] == name)
            return i;
    return -1;
}

// === TINSERT ===
void cmd_Tinsert(const Array<string>& toks) {
    if (toks.max_index < 3) { 
        cerr << "TINSERT требует имя и значение\n"; 
        return; 
    }

    string name = toks.data[1];
    string value = toks.data[2];

    // Если значение состоит из нескольких слов (TINSERT tree "hello world")
    for (int i = 3; i < toks.max_index; ++i)
        value += " " + toks.data[i];

    int idx = find_name_index(names_T, name);
    FullBinaryTree<string>* tree;

    if (idx == -1) {
        tree = new FullBinaryTree<string>;
        *tree = create_fbt<string>(); // создаём пустое бинарное дерево
        add_named_container<FullBinaryTree<string>*>(names_T, data_T, name, tree);
    } else {
        tree = data_T.data[idx];
    }

    add_element_fbt(*tree, value);
    cout << "OK\n";
    save_db(g_file_path);
}

// === TDEL ===
void cmd_Tfull(const Array<string>& toks) {
    if (toks.max_index < 2) { 
        cerr << "TFULL требует имя\n"; 
        return; 
    }

    string name = toks.data[1];
    int idx = find_name_index(names_T, name);

    if (idx == -1) { 
        cerr << "Дерево не найдено\n"; 
        return; 
    }

    FullBinaryTree<string>* tree = data_T.data[idx];
    cout << "Full? " << (is_full_fbt(*tree) ? "Да" : "Нет") << "\n";
}

// === TGET ===
void cmd_Tget(const Array<string>& toks) {
    if (toks.max_index < 2) { 
        cerr << "TGET требует имя\n"; 
        return; 
    }

    string name = toks.data[1];
    int idx = find_tree_index(name);

    if (idx == -1) { 
        cerr << "Дерево не найдено\n"; 
        return; 
    }

    cout << "Дерево " << name << ": ";
    read_fbt(*trees[idx]);
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

    // стек
    idx = find_name_index(names_S, name);
    if (idx != -1) {
        Stack<string>* s = data_S.data[idx];
        for (int i = 0; i < s->top_index; i++) {
            cout << s->data[i] << " ";
        }
        cout << "\n";
        return;
    }

    cerr << "Контейнер с именем " << name << " не найден\n";
}

