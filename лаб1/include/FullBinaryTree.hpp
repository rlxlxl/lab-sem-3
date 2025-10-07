#pragma once
#include <iostream>
using namespace std;

// Узел полного бинарного дерева
template<typename T>
struct NodeFBT {
    T data;
    NodeFBT* left;
    NodeFBT* right;
};

// Само дерево
template<typename T>
struct FullBinaryTree {
    NodeFBT<T>* root;
};

// Создание пустого дерева
template<typename T>
FullBinaryTree<T> create_fbt() {
    FullBinaryTree<T> tree;
    tree.root = nullptr;
    return tree;
}

// ------------------- ОЧЕРЕДЬ (вручную) -------------------
template<typename T>
struct NodeQueue {
    NodeFBT<T>** data;
    int front;
    int rear;
    int size;
    int capacity;
};

template<typename T>
NodeQueue<T> create_queue_fbt(int cap = 64) {
    NodeQueue<T> q;
    q.data = new NodeFBT<T>*[cap];
    q.front = q.rear = q.size = 0;
    q.capacity = cap;
    return q;
}

template<typename T>
bool is_empty_queue(NodeQueue<T>& q) {
    return q.size == 0;
}

template<typename T>
void push_queue_fbt(NodeQueue<T>& q, NodeFBT<T>* node) {
    if (q.size == q.capacity) return;
    q.data[q.rear] = node;
    q.rear = (q.rear + 1) % q.capacity;
    q.size++;
}

template<typename T>
NodeFBT<T>* pop_queue_fbt(NodeQueue<T>& q) {
    if (is_empty_queue(q)) return nullptr;
    NodeFBT<T>* node = q.data[q.front];
    q.front = (q.front + 1) % q.capacity;
    q.size--;
    return node;
}

template<typename T>
void destroy_queue_fbt(NodeQueue<T>& q) {
    delete[] q.data;
    q.data = nullptr;
    q.front = q.rear = q.size = q.capacity = 0;
}

// ------------------- ДОБАВЛЕНИЕ -------------------
template<typename T>
void add_element_fbt(FullBinaryTree<T>& tree, const T& value) {
    NodeFBT<T>* new_node = new NodeFBT<T>;
    new_node->data = value;
    new_node->left = nullptr;
    new_node->right = nullptr;

    if (tree.root == nullptr) {
        tree.root = new_node;
        return;
    }

    NodeQueue<T> q = create_queue_fbt<T>(128);
    push_queue_fbt(q, tree.root);

    while (!is_empty_queue(q)) {
        NodeFBT<T>* cur = pop_queue_fbt(q);
        if (cur->left == nullptr) {
            cur->left = new_node;
            destroy_queue_fbt(q);
            return;
        } else if (cur->right == nullptr) {
            cur->right = new_node;
            destroy_queue_fbt(q);
            return;
        } else {
            push_queue_fbt(q, cur->left);
            push_queue_fbt(q, cur->right);
        }
    }

    destroy_queue_fbt(q);
}

// ------------------- ПОИСК -------------------
template<typename T>
NodeFBT<T>* find_element_fbt(NodeFBT<T>* root, const T& value) {
    if (root == nullptr) return nullptr;
    if (root->data == value) return root;
    NodeFBT<T>* left = find_element_fbt(root->left, value);
    if (left != nullptr) return left;
    return find_element_fbt(root->right, value);
}

template<typename T>
NodeFBT<T>* find_element_fbt(FullBinaryTree<T>& tree, const T& value) {
    return find_element_fbt(tree.root, value);
}

// ------------------- ПРОВЕРКА FULL -------------------
template<typename T>
bool is_full_fbt(NodeFBT<T>* root) {
    if (root == nullptr) return true;
    if ((root->left == nullptr && root->right != nullptr) ||
        (root->left != nullptr && root->right == nullptr))
        return false;
    return is_full_fbt(root->left) && is_full_fbt(root->right);
}

template<typename T>
bool is_full_fbt(FullBinaryTree<T>& tree) {
    return is_full_fbt(tree.root);
}

// ------------------- ПРОВЕРКА COMPLETE -------------------
template<typename T>
bool is_complete_fbt(FullBinaryTree<T>& tree) {
    if (tree.root == nullptr) return true;

    NodeQueue<T> q = create_queue_fbt<T>(128);
    push_queue_fbt(q, tree.root);

    bool foundNull = false;

    while (!is_empty_queue(q)) {
        NodeFBT<T>* cur = pop_queue_fbt(q);
        if (cur->left != nullptr) {
            if (foundNull) {
                destroy_queue_fbt(q);
                return false; // после null не должно быть потомков
            }
            push_queue_fbt(q, cur->left);
        } else {
            foundNull = true;
        }

        if (cur->right != nullptr) {
            if (foundNull) {destroy_queue_fbt(q);
                return false;
            }
            push_queue_fbt(q, cur->right);
        } else {
            foundNull = true;
        }
    }

    destroy_queue_fbt(q);
    return true;
}

// ------------------- ОБХОД (ЧТЕНИЕ) -------------------
template<typename T>
void read_fbt(NodeFBT<T>* root) {
    if (root == nullptr) return;
    read_fbt(root->left);
    cout << root->data << " ";
    read_fbt(root->right);
}

template<typename T>
void read_fbt(FullBinaryTree<T>& tree) {
    read_fbt(tree.root);
    cout << endl;
}