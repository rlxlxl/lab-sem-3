#pragma once

#include "Queue.hpp"

// Структура узла и дерева
template<typename T>
struct NodeFBT {
    T data;
    NodeFBT* left;
    NodeFBT* right;
};

template<typename T>
struct FullBinaryTree {
    NodeFBT<T>* root;
};

template<typename T>
FullBinaryTree<T> create_fbt() {
    FullBinaryTree<T> tree;
    tree.root = nullptr;
    return tree;
}

template<typename T>
NodeFBT<T>* create_node_fbt(const T& value) {
    NodeFBT<T>* node = new NodeFBT<T>;
    node->data = value;
    node->left = node->right = nullptr;
    return node;
}

template<typename T>
void add_element_fbt(FullBinaryTree<T>& tree, const T& value) {
    NodeFBT<T>* newNode = create_node_fbt(value);
    if (!tree.root) {
        tree.root = newNode;
        return;
    }
    Queue<NodeFBT<T>*> q = create_queue<NodeFBT<T>*>();
    push_queue(q, tree.root);
    while (q.size > 0) {
        NodeFBT<T>* cur = pop_queue(q);
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

template<typename T>
bool is_full_fbt(const FullBinaryTree<T>& tree) {
    if (!tree.root) return true;
    Queue<NodeFBT<T>*> q = create_queue<NodeFBT<T>*>();
    push_queue(q, tree.root);
    while (q.size > 0) {
        NodeFBT<T>* cur = pop_queue(q);
        if ((cur->left && !cur->right) || (!cur->left && cur->right)) {
            return false;
        }
        if (cur->left) push_queue(q, cur->left);
        if (cur->right) push_queue(q, cur->right);
    }
    return true;
}

template<typename T>
bool find_element_fbt(NodeFBT<T>* root, const T& value) {
    if (!root) return false;
    Queue<NodeFBT<T>*> q = create_queue<NodeFBT<T>*>();
    push_queue(q, root);
    while (q.size > 0) {
        NodeFBT<T>* cur = pop_queue(q);
        if (cur->data == value) return true;
        if (cur->left) push_queue(q, cur->left);
        if (cur->right) push_queue(q, cur->right);
    }
    return false;
}

template<typename T>
void print_fbt(NodeFBT<T>* root) {
    if (!root) return;
    Queue<NodeFBT<T>*> q = create_queue<NodeFBT<T>*>();
    push_queue(q, root);
    while (q.size > 0) {
        NodeFBT<T>* cur = pop_queue(q);
        cout << cur->data << " ";
        if (cur->left) push_queue(q, cur->left);
        if (cur->right) push_queue(q, cur->right);
    }
    cout << "\n";
}