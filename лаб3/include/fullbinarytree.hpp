#pragma once
#include <iostream>
#include <string>
using namespace std;

class BST {
private:
    struct Node {
        int data;
        Node* left;
        Node* right;
        Node(int value) : data(value), left(nullptr), right(nullptr) {}
    };

    Node* root;

    // Вспомогательная структура для вывода дерева
    struct PrintLine {
        string line;
        PrintLine* next;
        PrintLine(const string& s) : line(s), next(nullptr) {}
    };

    // Рекурсивная функция построения линий для красивого вывода
    PrintLine* build_tree_lines(Node* node, bool isLeft, const string& prefix) const {
        if (!node) return nullptr;

        PrintLine* current = new PrintLine(prefix + (isLeft ? "├── " : "└── ") + to_string(node->data));
        string newPrefix = prefix + (isLeft ? "│   " : "    ");
        PrintLine* last = current;

        if (node->left && node->right) {
            PrintLine* leftLines = build_tree_lines(node->left, true, newPrefix);
            PrintLine* rightLines = build_tree_lines(node->right, false, newPrefix);
            if (leftLines) { last->next = leftLines; while (last->next) last = last->next; }
            if (rightLines) last->next = rightLines;
        } else if (node->left) {
            PrintLine* leftLines = build_tree_lines(node->left, false, newPrefix);
            if (leftLines) last->next = leftLines;
        } else if (node->right) {
            PrintLine* rightLines = build_tree_lines(node->right, false, newPrefix);
            if (rightLines) last->next = rightLines;
        }

        return current;
    }

    // Рекурсивное удаление всех узлов
    void clear(Node* node) {
        if (!node) return;
        clear(node->left);
        clear(node->right);
        delete node;
    }

public:
    BST() : root(nullptr) {}
    ~BST() { clear(root); }

    void insert(int value) {
        Node** cur = &root;
        while (*cur) {
            if (value < (*cur)->data)
                cur = &(*cur)->left;
            else
                cur = &(*cur)->right;
        }
        *cur = new Node(value);
    }

    bool find(int value) const {
        Node* cur = root;
        while (cur) {
            if (cur->data == value) return true;
            else if (value < cur->data) cur = cur->left;
            else cur = cur->right;
        }
        return false;
    }

    void print_pretty() const {
        if (!root) {
            cout << "Дерево пустое\n";
            return;
        }

        PrintLine* lines = build_tree_lines(root, false, "");
        PrintLine* current = lines;
        while (current) {
            cout << current->line << "\n";
            PrintLine* temp = current;
            current = current->next;
            delete temp;
        }
    }
};
