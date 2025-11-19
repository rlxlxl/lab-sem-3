#ifndef HASH_TABLE_H
#define HASH_TABLE_H

#include <cstddef>

#define MAX_HASH_TABLES 10
#define MAX_KEY_LENGTH 256
#define MAX_VALUE_LENGTH 512

// Типы хэш-таблиц
enum HashTableType {
    CHAINING,
    OPEN_ADDRESSING
};

// Структура хэш-таблицы
struct HashTable {
    char name[50];
    HashTableType type;
    size_t capacity;
    size_t size;
    void* table;  // Указатель на реализацию
};

// Прототипы функций
void initHashTableSystem();
HashTable* createHashTable(const char* name, HashTableType type, size_t capacity);
HashTable* findHashTable(const char* name);
void removeHashTable(const char* name);
void hashTableInsert(HashTable* ht, const char* key, const char* value);
bool hashTableFind(HashTable* ht, const char* key, char* value);
const char* hashTableRemove(HashTable* ht, const char* key);
void hashTablePrint(HashTable* ht);
size_t hashTableSize(HashTable* ht);
void saveHashTablesToFile(const char* filename);
void loadHashTablesFromFile(const char* filename);
void convertToRoman(int number, char* result, size_t resultSize);
void executeHashTableCommand(const char* command);

#endif