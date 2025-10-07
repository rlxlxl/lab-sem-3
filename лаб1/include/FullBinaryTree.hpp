#pragma once
#include <iostream>
#include <queue>
#include <string>
using namespace std;

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
    queue<NodeFBT<T>*> q;
    q.push(tree.root);
    while (!q.empty()) {
        NodeFBT<T>* cur = q.front(); q.pop();
        if (!cur->left) { cur->left = newNode; return; } else q.push(cur->left);
        if (!cur->right) { cur->right = newNode; return; } else q.push(cur->right);
    }
}

template<typename T>
bool is_full_fbt(const FullBinaryTree<T>& tree) {
    if (!tree.root) return true;
    queue<NodeFBT<T>*> q;
    q.push(tree.root);
    bool shouldHaveNoChild = false;
    while (!q.empty()) {
        NodeFBT<T>* cur = q.front(); q.pop();
        if (cur->left) {
            if (shouldHaveNoChild) return false;
            q.push(cur->left);
        } else shouldHaveNoChild = true;

        if (cur->right) {
            if (shouldHaveNoChild) return false;
            q.push(cur->right);
        } else shouldHaveNoChild = true;
    }
    return true;
}

template<typename T>
bool find_element_fbt(NodeFBT<T>* root, const T& value) {
    if (!root) return false;
    queue<NodeFBT<T>*> q;
    q.push(root);
    while (!q.empty()) {
        NodeFBT<T>* cur = q.front(); q.pop();
        if (cur->data == value) return true;
        if (cur->left) q.push(cur->left);
        if (cur->right) q.push(cur->right);
    }
    return false;
}

template<typename T>
void print_fbt(NodeFBT<T>* root) {
    if (!root) return;
    queue<NodeFBT<T>*> q;
    q.push(root);
    while (!q.empty()) {
        NodeFBT<T>* cur = q.front(); q.pop();
        cout << cur->data << " ";
        if (cur->left) q.push(cur->left);
        if (cur->right) q.push(cur->right);
    }
    cout << "\n";
}