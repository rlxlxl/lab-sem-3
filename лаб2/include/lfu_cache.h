#ifndef LFU_CACHE_H
#define LFU_CACHE_H

#include <iostream>
#include <cstring>
#include <cstdlib>

#define MAX_KEY_LENGTH 256
#define MAX_VALUE_LENGTH 512
#define MAX_LFU_CACHES 50

// Структура для узла LFU кэша
struct LFUNode {
    char key[MAX_KEY_LENGTH];
    char value[MAX_VALUE_LENGTH];
    int frequency;
    LFUNode* prev;
    LFUNode* next;
    
    LFUNode() : frequency(1), prev(nullptr), next(nullptr) {
        key[0] = '\0';
        value[0] = '\0';
    }
};

// Структура для частотного списка
struct FrequencyList {
    int frequency;
    LFUNode* head;
    LFUNode* tail;
    FrequencyList* prev;
    FrequencyList* next;
    
    FrequencyList(int freq) : frequency(freq), head(nullptr), tail(nullptr), 
                             prev(nullptr), next(nullptr) {}
};

// Основная структура LFU кэша
struct LFUCache {
    char name[50];
    size_t capacity;
    size_t size;
    FrequencyList* freqHead;
    FrequencyList* freqTail;
    
    LFUCache() : capacity(0), size(0), freqHead(nullptr), freqTail(nullptr) {
        name[0] = '\0';
    }
};

// Инициализация системы LFU кэшей
void initLFUCacheSystem();

// Создание нового LFU кэша
LFUCache* createLFUCache(const char* name, size_t capacity);

// Поиск LFU кэша по имени
LFUCache* findLFUCache(const char* name);

// Удаление LFU кэша
void removeLFUCache(const char* name);

// Основные операции
void lfuCacheSet(LFUCache* cache, const char* key, const char* value);
const char* lfuCacheGet(LFUCache* cache, const char* key);
void lfuCachePrint(LFUCache* cache);
size_t lfuCacheSize(LFUCache* cache);

// Выполнение команд из терминала
void executeLFUCacheCommand(const char* command);

#endif