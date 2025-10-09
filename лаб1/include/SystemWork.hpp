#pragma once
#include "Stack.hpp"
#include "Array.hpp"
#include "DoublyList.hpp"
#include "ForwardList.hpp"
#include "FullBinaryTree.hpp"
#include "Globals.cpp"
#include "Queue.hpp"
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
using namespace std;

// Все контейнеры
extern Array<string> names_M;
extern Array<Array<string>*> data_M;

extern Array<string> names_F;
extern Array<ForwardList<string>*> data_F;

extern Array<string> names_L;
extern Array<DoublyList<string>*> data_L;

extern Array<string> names_Q;
extern Array<Queue<string>*> data_Q;

extern Array<string> names_S;
extern Array<Stack<string>*> data_S;

extern Array<string> names_T;
extern Array<FullBinaryTree*> data_T;

extern string g_file_path;

// Добавление контейнера по имени
template<typename PtrT>
void add_named_container(Array<string>& names_arr, Array<PtrT>& data_arr, const string& name, PtrT ptr) {
    add_element_end_ar<string>(names_arr, name);
    add_element_end_ar<PtrT>(data_arr, ptr);
}

// Токенизация строки
Array<string> tokenize_to_array(const string& line) {
    Array<string> toks = create_array_ar<string>(4);
    string token;
    stringstream ss(line);
    while (ss >> token) add_element_end_ar<string>(toks, token);
    return toks;
}

// --- Загрузка базы ---
void load_db(const string& path) {
    ifstream in(path);
    if (!in.is_open()) return;
    string line;
    while (getline(in, line)) {
        if (line.empty()) continue;
        Array<string> toks = tokenize_to_array(line);
        if (toks.max_index < 2) continue;
        string type = toks.data[0];
        string name = toks.data[1];

        if (type == "M") {
            Array<string>* arr = new Array<string>(create_array_ar<string>(max(4, toks.max_index-2)));
            for (int i=2; i<toks.max_index; ++i) add_element_end_ar<string>(*arr, toks.data[i]);
            add_named_container(names_M, data_M, name, arr);
        } else if (type == "F") {
            ForwardList<string>* fl = new ForwardList<string>(create_forwardList<string>());
            for (int i=2; i<toks.max_index; ++i) add_element_back_fl(toks.data[i], *fl);
            add_named_container(names_F, data_F, name, fl);
        } else if (type == "L") {
            DoublyList<string>* dl = new DoublyList<string>(create_doublyList<string>());
            for (int i=2; i<toks.max_index; ++i) add_element_back_dl(toks.data[i], *dl);
            add_named_container(names_L, data_L, name, dl);
        } else if (type == "Q") {
            Queue<string>* q = new Queue<string>(create_queue<string>());
            for (int i=2; i<toks.max_index; ++i) push_queue(*q, toks.data[i]);
            add_named_container(names_Q, data_Q, name, q);
        } else if (type == "S") {
            // ИЗМЕНЕНО: создаем стек с новой реализацией
            Stack<string>* s = new Stack<string>(create_stack<string>());
            for (int i=2; i<toks.max_index; ++i) push_stack(*s, toks.data[i]);
            add_named_container(names_S, data_S, name, s);
        } else if (type == "T") {
            FullBinaryTree* tree = new FullBinaryTree(create_fbt());
            for (int i=2; i<toks.max_index; ++i) {
                try {
                    int value = stoi(toks.data[i]);
                    add_element_fbt(*tree, value);
                } catch (...) {
                    continue;
                }
            }
            add_named_container(names_T, data_T, name, tree);
        }
    }
    in.close();
}

// --- Сохранение базы ---
void save_db(const string& path) {
    string tmp = path + ".tmp";
    ofstream out(tmp);
    if (!out.is_open()) { cerr << "Не удалось открыть файл\n"; return; }

    // Массивы
    for (int i=0; i<names_M.max_index; ++i) {
        if (!names_M.is_set[i]) continue;
        out << "M " << names_M.data[i];
        Array<string>* arr = data_M.data[i];
        for (int j=0; j<arr->max_index; ++j) if (arr->is_set[j]) out << " " << arr->data[j];
        out << "\n";
    }

    // Односвязные списки
    for (int i=0; i<names_F.max_index; ++i) {
        if (!names_F.is_set[i]) continue;
        out << "F " << names_F.data[i];
        ForwardList<string>* fl = data_F.data[i];
        Node_Fl<string>* cur = fl->head;
        while(cur) { out << " " << *(cur->data); cur = cur->next; }
        out << "\n";
    }

    // Двусвязные списки
    for (int i=0; i<names_L.max_index; ++i) {
        if (!names_L.is_set[i]) continue;
        out << "L " << names_L.data[i];
        DoublyList<string>* dl = data_L.data[i];
        Node_Dl<string>* cur = dl->head;
        while(cur) { out << " " << *(cur->data); cur = cur->next; }
        out << "\n";
    }

    // Очереди
    for (int i=0; i<names_Q.max_index; ++i) {
        if (!names_Q.is_set[i]) continue;
        out << "Q " << names_Q.data[i];
        Queue<string>* q = data_Q.data[i];
        for (int k=0; k<q->size; ++k) {
            int at = (q->front + k) % q->capacity;
            out << " " << q->data[at];
        }
        out << "\n";
    }

    // Стеки - ИЗМЕНЕНО для новой реализации
    for (int i=0; i<names_S.max_index; ++i) {
        if (!names_S.is_set[i]) continue;
        out << "S " << names_S.data[i];
        Stack<string>* s = data_S.data[i];
        // Обход связного списка стека
        StackNode<string>* current = s->top;
        while (current != nullptr) {
            out << " " << current->data;
            current = current->next;
        }
        out << "\n";
    }

    // Деревья (FullBinaryTree)
    for (int i = 0; i < names_T.max_index; ++i) {
        if (!names_T.is_set[i]) continue;
        out << "T " << names_T.data[i];
        FullBinaryTree* tree = data_T.data[i];
        if (!tree->root) { out << "\n"; continue; }
        
        // BFS обход с пользовательской очередью
        Queue<NodeFBT*> q = create_queue<NodeFBT*>();
        push_queue(q, tree->root);
        while (q.size > 0) {
            NodeFBT* cur = pop_queue(q);
            out << " " << cur->data;
            if (cur->left) push_queue(q, cur->left);
            if (cur->right) push_queue(q, cur->right);
        }
        out << "\n";
    }

    out.close();
    remove(path.c_str());
    rename(tmp.c_str(), path.c_str());
}

int parse_index(const string& s) {
    try {
        long x = stol(s);
        if (x < 1) return -1;
        return (int)x;
    } catch (...) {
        return -1;
    }
}

int find_name_index(Array<string>& names, const string& name) {
    for (int i = 0; i < names.max_index; ++i) {
        if (names.is_set[i] && names.data[i] == name) return i;
    }
    return -1;
}