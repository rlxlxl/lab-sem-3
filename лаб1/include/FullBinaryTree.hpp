#pragma once

#include "Queue.hpp"

// Структура узла и дерева
struct NodeFBT {
    int data;
    NodeFBT* left;
    NodeFBT* right;
};

struct FullBinaryTree {
    NodeFBT* root;
};

FullBinaryTree create_fbt() {
    FullBinaryTree tree;
    tree.root = nullptr;
    return tree;
}

NodeFBT* create_node_fbt(int value) {
    NodeFBT* node = new NodeFBT;
    node->data = value;
    node->left = node->right = nullptr;
    return node;
}

void add_element_fbt(FullBinaryTree& tree, int value) {
    NodeFBT* newNode = create_node_fbt(value);
    if (!tree.root) {
        tree.root = newNode;
        return;
    }
    Queue<NodeFBT*> q = create_queue<NodeFBT*>();
    push_queue(q, tree.root);
    while (q.size > 0) {
        NodeFBT* cur = pop_queue(q);
        if (!cur->left) {
            cur->left = newNode;
            return;
        } else {
            push_queue(q, cur->left);
        }
        if (!cur->right) {
            cur->right = newNode;
            return;
        } else {
            push_queue(q, cur->right);
        }
    }
}

bool is_full_fbt(const FullBinaryTree& tree) {
    if (!tree.root) return true;

    Queue<NodeFBT*> q = create_queue<NodeFBT*>();
    push_queue(q, tree.root);
    bool mustBeLeaf = false;

    while (q.size > 0) {
        NodeFBT* cur = pop_queue(q);

        if (mustBeLeaf) {
            if (cur->left || cur->right) return false;
        }

        if (cur->left && cur->right) {
            push_queue(q, cur->left);
            push_queue(q, cur->right);
        } else if (cur->left && !cur->right) {
            return false;
        } else if (!cur->left && cur->right) {
            return false;
        } else {
            mustBeLeaf = true;
        }
    }

    return true;
}

bool find_element_fbt(NodeFBT* root, int value) {
    if (!root) return false;
    Queue<NodeFBT*> q = create_queue<NodeFBT*>();
    push_queue(q, root);
    while (q.size > 0) {
        NodeFBT* cur = pop_queue(q);
        if (cur->data == value) return true;
        if (cur->left) push_queue(q, cur->left);
        if (cur->right) push_queue(q, cur->right);
    }
    return false;
}

void print_fbt(NodeFBT* root) {
    if (!root) return;
    Queue<NodeFBT*> q = create_queue<NodeFBT*>();
    push_queue(q, root);
    while (q.size > 0) {
        NodeFBT* cur = pop_queue(q);
        cout << cur->data << " ";
        if (cur->left) push_queue(q, cur->left);
        if (cur->right) push_queue(q, cur->right);
    }
    cout << "\n";
}

struct PrintLine {
    string line;
    PrintLine* next;
};

// Рекурсивная функция для построения строк вывода
PrintLine* build_tree_lines(NodeFBT* node, bool isLeft, string prefix) {
    if (!node) return nullptr;

    // Создаем строку для текущего узла
    PrintLine* current = new PrintLine;
    current->line = prefix + (isLeft ? "├── " : "└── ") + to_string(node->data);
    current->next = nullptr;

    // Рекурсивно обрабатываем потомков
    string newPrefix = prefix + (isLeft ? "│   " : "    ");
    
    PrintLine* last = current;
    
    if (node->left && node->right) {
        // Есть оба потомка
        PrintLine* leftLines = build_tree_lines(node->left, true, newPrefix);
        PrintLine* rightLines = build_tree_lines(node->right, false, newPrefix);
        
        if (leftLines) {
            last->next = leftLines;
            while (last->next) last = last->next;
        }
        if (rightLines) {
            last->next = rightLines;
        }
    } else if (node->left) {
        // Только левый потомок
        PrintLine* leftLines = build_tree_lines(node->left, false, newPrefix);
        if (leftLines) {
            last->next = leftLines;
        }
    } else if (node->right) {
        // Только правый потомок
        PrintLine* rightLines = build_tree_lines(node->right, false, newPrefix);
        if (rightLines) {
            last->next = rightLines;
        }
    }

    return current;
}

// Функция для красивого вывода дерева (вертикальный вывод)
void print_tree_pretty(NodeFBT* root) {
    if (!root) {
        cout << "Дерево пустое\n";
        return;
    }

    // Получаем строки для вывода
    PrintLine* lines = build_tree_lines(root, false, "");
    
    // Выводим результат и освобождаем память
    PrintLine* current = lines;
    while (current) {
        cout << current->line << "\n";
        PrintLine* temp = current;
        current = current->next;
        delete temp;
    }
}

