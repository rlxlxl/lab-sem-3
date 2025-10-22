#pragma once
#include "Queue.hpp"
#include <iostream>
using namespace std;

// Узел BST
struct NodeBST {
    int data;
    NodeBST* left;
    NodeBST* right;
};

// BST
struct FullBinaryTree {
    NodeBST* root;
};

// Создать пустое дерево
FullBinaryTree create_bst() {
    return {nullptr};
}

// Создать узел
NodeBST* create_node_bst(int value) {
    NodeBST* node = new NodeBST;
    node->data = value;
    node->left = node->right = nullptr;
    return node;
}

void add_element_bst(FullBinaryTree& tree, int value) {
    NodeBST** cur = &tree.root;
    while (*cur) {
        if (value < (*cur)->data)
            cur = &(*cur)->left;
        else
            cur = &(*cur)->right;
    }
    *cur = create_node_bst(value);
}

struct PrintLine {
    string line;
    PrintLine* next;
};

PrintLine* build_tree_lines(NodeBST* node, bool isLeft, string prefix) {
    if (!node) return nullptr;

    PrintLine* current = new PrintLine;
    current->line = prefix + (isLeft ? "├── " : "└── ") + to_string(node->data);
    current->next = nullptr;

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

bool find_element_bst(NodeBST* root, int value) {
    NodeBST* cur = root;
    while (cur) {
        if (cur->data == value) return true;
        else if (value < cur->data) cur = cur->left;
        else cur = cur->right;
    }
    return false;
}

void print_tree_pretty(NodeBST* root) {
    if (!root) { cout << "Дерево пустое\n"; return; }

    PrintLine* lines = build_tree_lines(root, false, "");
    PrintLine* current = lines;
    while (current) {
        cout << current->line << "\n";
        PrintLine* temp = current;
        current = current->next;
        delete temp;
    }
}
