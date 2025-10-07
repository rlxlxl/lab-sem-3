#pragma once

#include "Queue.hpp"
#include "Stack.hpp"
#include "Array.hpp"
#include "DoublyList.hpp"
#include "ForwardList.hpp"
#include "FullBinaryTree.hpp"
#include <fstream>
#include <iostream>
#include <string>
#include <sstream>
using namespace std;

Array<string> names_M = create_array_ar<string>(4);
Array<Array<string>*> data_M = create_array_ar<Array<string>*>(4);

Array<string> names_F = create_array_ar<string>(4);
Array<ForwardList<string>*> data_F = create_array_ar<ForwardList<string>*>(4);

Array<string> names_L = create_array_ar<string>(4);
Array<DoublyList<string>*> data_L = create_array_ar<DoublyList<string>*>(4);

Array<string> names_Q = create_array_ar<string>(4);
Array<Queue<string>*> data_Q = create_array_ar<Queue<string>*>(4);

Array<string> names_S = create_array_ar<string>(4);
Array<Stack<string>*> data_S = create_array_ar<Stack<string>*>(4);

Array<string> names_T = create_array_ar<string>(4);
Array<FullBinaryTree<string>*> data_T = create_array_ar<FullBinaryTree<string>*>(4);

string g_file_path;

template<typename PtrT>
void add_named_container(Array<string>& names_arr, Array<PtrT>& data_arr, const string& name, PtrT ptr) {
    add_element_end_ar<string>(names_arr, name);
    add_element_end_ar<PtrT>(data_arr, ptr);
}

Array<string> tokenize_to_array(const string& line) {
    Array<string> toks = create_array_ar<string>(4);
    string token;
    stringstream ss(line);
    while (ss >> token) add_element_end_ar<string>(toks, token);
    return toks;
}

void load_db(const string& path) {
    ifstream in(path);
    if (!in.is_open()) {
        cerr << "Не удалось открыть файл " << path << endl;
        return;
    }
    string line;
    while (getline(in, line)) {
        if (line.empty()) continue;
        Array<string> toks = tokenize_to_array(line);
        if (toks.max_index < 2) continue;
        string type = toks.data[0];
        string name = toks.data[1];
        if (type == "M") {
            Array<string>* arr = new Array<string>(create_array_ar<string>(max(4, toks.max_index - 2)));
            for (int i = 2; i < toks.max_index; ++i) add_element_end_ar<string>(*arr, toks.data[i]);
            add_named_container<Array<string>*>(names_M, data_M, name, arr);
        } else if (type == "F") {
            ForwardList<string>* fl = new ForwardList<string>(create_forwardList<string>());
            for (int i = 2; i < toks.max_index; ++i) add_element_back_fl<string>(toks.data[i], *fl);
            add_named_container<ForwardList<string>*>(names_F, data_F, name, fl);
        } else if (type == "L") {
            DoublyList<string>* dl = new DoublyList<string>(create_doublyList<string>());
            for (int i = 2; i < toks.max_index; ++i) add_element_back_dl<string>(toks.data[i], *dl);
            add_named_container<DoublyList<string>*>(names_L, data_L, name, dl);
        } else if (type == "Q") {
            Queue<string>* q = new Queue<string>(create_queue<string>());
            // push elements into queue
            for (int i = 2; i < toks.max_index; ++i) push_queue<string>(*q, toks.data[i]);
            add_named_container<Queue<string>*>(names_Q, data_Q, name, q);
        } else if (type == "S") {
            Stack<string>* s = new Stack<string>(create_stack<string>());
            for (int i = 2; i < toks.max_index; ++i) push_stack<string>(*s, toks.data[i]);
            add_named_container<Stack<string>*>(names_S, data_S, name, s);
        } else if (type == "T") {
            FullBinaryTree<string>* tree = new FullBinaryTree<string>(create_fbt<string>());
            for (int i = 2; i < toks.max_index; ++i) {
                add_element_fbt(*tree, toks.data[i]);
            }
            add_named_container<FullBinaryTree<string>*>(names_T, data_T, name, tree);
        }
    in.close();
}
}

void save_db(const string& path) {
    string tmp = path + ".tmp";
    ofstream out(tmp);
    if (!out.is_open()) {
        cerr << "Не удалось открыть временный файл для записи: " << tmp << endl;
        return;
    }
    // M
    for (int i = 0; i < names_M.max_index; ++i) {
        if (!names_M.is_set[i]) continue;
        out << "M " << names_M.data[i];
        Array<string>* arr = data_M.data[i];
        for (int j = 0; j < arr->max_index; ++j) {
            if (arr->is_set[j]) out << " " << arr->data[j];
        }
        out << "\n";
    }
    // F
    for (int i = 0; i < names_F.max_index; ++i) {
        if (!names_F.is_set[i]) continue;
        out << "F " << names_F.data[i];
        ForwardList<string>* fl = data_F.data[i];
        Node_Fl<string>* cur = fl->head;
        while (cur != nullptr) {
            out << " " << *(cur->data);
            cur = cur->next;
        }
        out << "\n";
    }
    // L
    for (int i = 0; i < names_L.max_index; ++i) {
        if (!names_L.is_set[i]) continue;
        out << "L " << names_L.data[i];
        DoublyList<string>* dl = data_L.data[i];
        Node_Dl<string>* cur = dl->head;
        while (cur != nullptr) {
            out << " " << *(cur->data);
            cur = cur->next;
        }
        out << "\n";
    }
    // Q
    for (int i = 0; i < names_Q.max_index; ++i) {
        if (!names_Q.is_set[i]) continue;
        out << "Q " << names_Q.data[i];
        Queue<string>* q = data_Q.data[i];
        for (int k = 0; k < q->size; ++k) {
            int idx = (q->front + k) % q->capacity;
            out << " " << q->data[idx];
        }
        out << "\n";
    }
    // S
    for (int i = 0; i < names_S.max_index; ++i) {
        if (!names_S.is_set[i]) continue;
        out << "S " << names_S.data[i];
        Stack<string>* s = data_S.data[i];
        for (int k = 0; k < s->top_index; ++k) out << " " << s->data[k];
        out << "\n";
    }
    // T (FullBinaryTree)
    for (int i = 0; i < names_T.max_index; ++i) {
        if (!names_T.is_set[i]) continue;
        out << "T " << names_T.data[i];
        FullBinaryTree<string>* tree = data_T.data[i];
        if (tree->root == nullptr) {
            out << "\n";
            continue;
        }

        // Обход по уровням для сохранения узлов
        NodeQueue<string> q = create_queue_fbt<string>(128);
        push_queue_fbt(q, tree->root);
        while (!is_empty_queue(q)) {
            NodeFBT<string>* cur = pop_queue_fbt(q);
            out << " " << cur->data;
            if (cur->left != nullptr) push_queue_fbt(q, cur->left);
            if (cur->right != nullptr) push_queue_fbt(q, cur->right);
        }
        destroy_queue_fbt(q);
        out << "\n";
    }

    
    out.close();
    // replace
    std::remove(path.c_str());
    std::rename(tmp.c_str(), path.c_str());
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

