#pragma once
#include <iostream>
#include <vector>
using namespace std;

// Класс узла для цепочек
template<typename Key, typename Value>
class HashNode {
public:
    Key key;
    Value value;
    HashNode* next;

    HashNode(const Key& k, const Value& v) : key(k), value(v), next(nullptr) {}
};

// Класс хеш-таблицы
template<typename Key, typename Value>
class HashTable {
private:
    vector<HashNode<Key, Value>*> table;
    int capacity;
    int size;

    // Хеш-функция
    int hash(const Key& key) const {
        return std::hash<Key>{}(key) % capacity;
    }

public:
    HashTable(int cap = 16) : capacity(cap), size(0) {
        table.resize(capacity, nullptr);
    }

    ~HashTable() {
        for (int i = 0; i < capacity; i++) {
            HashNode<Key, Value>* current = table[i];
            while (current) {
                HashNode<Key, Value>* temp = current;
                current = current->next;
                delete temp;
            }
        }
    }

    // Вставка элемента
    void insert(const Key& key, const Value& value) {
        int idx = hash(key);
        HashNode<Key, Value>* node = table[idx];
        while (node) {
            if (node->key == key) {
                node->value = value; // обновление значения
                return;
            }
            node = node->next;
        }
        HashNode<Key, Value>* newNode = new HashNode<Key, Value>(key, value);
        newNode->next = table[idx];
        table[idx] = newNode;
        size++;
    }

    // Поиск элемента
    bool find(const Key& key, Value& value) const {
        int idx = hash(key);
        HashNode<Key, Value>* node = table[idx];
        while (node) {
            if (node->key == key) {
                value = node->value;
                return true;
            }
            node = node->next;
        }
        return false;
    }

    // Удаление элемента
    void remove(const Key& key) {
        int idx = hash(key);
        HashNode<Key, Value>* node = table[idx];
        HashNode<Key, Value>* prev = nullptr;

        while (node) {
            if (node->key == key) {
                if (prev) prev->next = node->next;
                else table[idx] = node->next;
                delete node;
                size--;
                return;
            }
            prev = node;
            node = node->next;
        }
        cerr << "Элемент с ключом " << key << " не найден\n";
    }

    // Вывод таблицы
    void print() const {
        for (int i = 0; i < capacity; i++) {
            cout << "[" << i << "]: ";
            HashNode<Key, Value>* node = table[i];
            while (node) {
                cout << "(" << node->key << " -> " << node->value << ") ";
                node = node->next;
            }
            cout << "\n";
        }
    }

    int get_size() const { return size; }
    int get_capacity() const { return capacity; }
};
