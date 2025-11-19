#include "lfu_cache.h"
using namespace std;

// Глобальные переменные для управления LFU кэшами
static LFUCache lfuCaches[MAX_LFU_CACHES];
static int lfuCacheCount = 0;

// Инициализация системы LFU кэшей
void initLFUCacheSystem() {
    lfuCacheCount = 0;
    for (int i = 0; i < MAX_LFU_CACHES; i++) {
        lfuCaches[i] = LFUCache();
    }
}

// Создание нового узла LFU
LFUNode* createLFUNode(const char* key, const char* value) {
    LFUNode* node = new LFUNode();
    strncpy(node->key, key, MAX_KEY_LENGTH - 1);
    node->key[MAX_KEY_LENGTH - 1] = '\0';
    strncpy(node->value, value, MAX_VALUE_LENGTH - 1);
    node->value[MAX_VALUE_LENGTH - 1] = '\0';
    node->frequency = 1;  // Начальная частота = 1
    return node;
}

// Освобождение узла LFU
void freeLFUNode(LFUNode* node) {
    delete node;
}

// Создание нового частотного списка
FrequencyList* createFrequencyList(int freq) {
    return new FrequencyList(freq);
}

// Освобождение частотного списка и всех его узлов
void freeFrequencyList(FrequencyList* fl) {
    if (fl) {
        // Освобождаем все узлы в списке
        LFUNode* current = fl->head;
        while (current != nullptr) {
            LFUNode* next = current->next;
            freeLFUNode(current);
            current = next;
        }
        delete fl;
    }
}

// Добавление узла в начало частотного списка
void addNodeToFrequencyList(FrequencyList* fl, LFUNode* node) {
    if (fl->head == nullptr) {
        fl->head = node;
        fl->tail = node;
        node->prev = nullptr;
        node->next = nullptr;
    } else {
        node->next = fl->head;
        node->prev = nullptr;
        fl->head->prev = node;
        fl->head = node;
    }
}

// Удаление узла из частотного списка
void removeNodeFromFrequencyList(FrequencyList* fl, LFUNode* node) {
    if (node->prev) {
        node->prev->next = node->next;
    } else {
        fl->head = node->next;
    }
    
    if (node->next) {
        node->next->prev = node->prev;
    } else {
        fl->tail = node->prev;
    }
    
    node->prev = nullptr;
    node->next = nullptr;
}

// Проверка, пуст ли частотный список
bool isFrequencyListEmpty(FrequencyList* fl) {
    return fl->head == nullptr;
}

// Поиск узла по ключу во всех частотных списках
LFUNode* findNodeByKey(LFUCache* cache, const char* key) {
    FrequencyList* currentFreq = cache->freqHead;
    while (currentFreq != nullptr) {
        LFUNode* current = currentFreq->head;
        while (current != nullptr) {
            if (strcmp(current->key, key) == 0) {
                return current;
            }
            current = current->next;
        }
        currentFreq = currentFreq->next;
    }
    return nullptr;
}

// Обновление частоты узла (основная логика LFU)
void updateFrequency(LFUCache* cache, LFUNode* node) {
    int oldFreq = node->frequency;
    int newFreq = oldFreq + 1;
    
    // Находим текущий частотный список
    FrequencyList* currentFreqList = cache->freqHead;
    while (currentFreqList != nullptr && currentFreqList->frequency != oldFreq) {
        currentFreqList = currentFreqList->next;
    }
    
    if (currentFreqList == nullptr) {
        return; // Не должно происходить
    }
    
    // Удаляем узел из текущего частотного списка
    removeNodeFromFrequencyList(currentFreqList, node);
    
    // Находим или создаем частотный список с новой частотой
    FrequencyList* newFreqList = nullptr;
    
    // Ищем существующий список с новой частотой
    FrequencyList* temp = cache->freqHead;
    while (temp != nullptr) {
        if (temp->frequency == newFreq) {
            newFreqList = temp;
            break;
        }
        temp = temp->next;
    }
    
    // Если не нашли, создаем новый список
    if (newFreqList == nullptr) {
        newFreqList = createFrequencyList(newFreq);
        
        // Вставляем в правильную позицию (сортировка по возрастанию частоты)
        if (cache->freqHead == nullptr) {
            cache->freqHead = newFreqList;
            cache->freqTail = newFreqList;
        } else {
            FrequencyList* prev = nullptr;
            FrequencyList* curr = cache->freqHead;
            
            while (curr != nullptr && curr->frequency < newFreq) {
                prev = curr;
                curr = curr->next;
            }
            
            if (prev == nullptr) {
                // Вставляем в начало
                newFreqList->next = cache->freqHead;
                cache->freqHead->prev = newFreqList;
                cache->freqHead = newFreqList;
            } else if (curr == nullptr) {
                // Вставляем в конец
                prev->next = newFreqList;
                newFreqList->prev = prev;
                cache->freqTail = newFreqList;
            } else {
                // Вставляем между prev и curr
                prev->next = newFreqList;
                newFreqList->prev = prev;
                newFreqList->next = curr;
                curr->prev = newFreqList;
            }
        }
    }
    
    // Добавляем узел в новый частотный список
    addNodeToFrequencyList(newFreqList, node);
    node->frequency = newFreq;
    
    // Если старый частотный список пуст, удаляем его
    if (isFrequencyListEmpty(currentFreqList)) {
        if (currentFreqList->prev) {
            currentFreqList->prev->next = currentFreqList->next;
        } else {
            cache->freqHead = currentFreqList->next;
        }
        
        if (currentFreqList->next) {
            currentFreqList->next->prev = currentFreqList->prev;
        } else {
            cache->freqTail = currentFreqList->prev;
        }
        
        freeFrequencyList(currentFreqList);
    }
}

// Удаление наименее часто используемого узла (при переполнении)
void removeLFUNode(LFUCache* cache) {
    if (cache->size == 0) return;
    
    // Находим список с минимальной частотой (первый в списке)
    FrequencyList* minFreqList = cache->freqHead;
    if (minFreqList == nullptr) return;
    
    // Удаляем последний узел из этого списка (LRU в пределах одной частоты)
    LFUNode* nodeToRemove = minFreqList->tail;
    if (nodeToRemove == nullptr) return;
    
    // Удаляем из частотного списка
    removeNodeFromFrequencyList(minFreqList, nodeToRemove);
    
    // Если частотный список стал пустым, удаляем его
    if (isFrequencyListEmpty(minFreqList)) {
        if (minFreqList->prev) {
            minFreqList->prev->next = minFreqList->next;
        } else {
            cache->freqHead = minFreqList->next;
        }
        
        if (minFreqList->next) {
            minFreqList->next->prev = minFreqList->prev;
        } else {
            cache->freqTail = minFreqList->prev;
        }
        
        freeFrequencyList(minFreqList);
    }
    
    // Освобождаем узел
    freeLFUNode(nodeToRemove);
    cache->size--;
}

// Создание нового LFU кэша
LFUCache* createLFUCache(const char* name, size_t capacity) {
    if (lfuCacheCount >= MAX_LFU_CACHES) {
        cout << "Достигнуто максимальное количество LFU кэшей" << endl;
        return nullptr;
    }
    
    LFUCache* cache = &lfuCaches[lfuCacheCount];
    strncpy(cache->name, name, sizeof(cache->name) - 1);
    cache->name[sizeof(cache->name) - 1] = '\0';
    cache->capacity = capacity;
    cache->size = 0;
    cache->freqHead = nullptr;
    cache->freqTail = nullptr;
    
    lfuCacheCount++;
    cout << "LFU кэш '" << name << "' создан с емкостью " << capacity << endl;
    return cache;
}

// Поиск кэша по имени
LFUCache* findLFUCache(const char* name) {
    for (int i = 0; i < lfuCacheCount; i++) {
        if (strcmp(lfuCaches[i].name, name) == 0) {
            return &lfuCaches[i];
        }
    }
    return nullptr;
}

// Удаление кэша
void removeLFUCache(const char* name) {
    for (int i = 0; i < lfuCacheCount; i++) {
        if (strcmp(lfuCaches[i].name, name) == 0) {
            LFUCache* cache = &lfuCaches[i];
            
            // Освобождаем все частотные списки
            FrequencyList* current = cache->freqHead;
            while (current != nullptr) {
                FrequencyList* next = current->next;
                freeFrequencyList(current);
                current = next;
            }
            
            // Сдвигаем оставшиеся кэши
            for (int j = i; j < lfuCacheCount - 1; j++) {
                lfuCaches[j] = lfuCaches[j + 1];
            }
            lfuCacheCount--;
            
            cout << "LFU кэш '" << name << "' удален" << endl;
            return;
        }
    }
    cout << "LFU кэш '" << name << "' не найден" << endl;
}

// Установка значения в кэш
void lfuCacheSet(LFUCache* cache, const char* key, const char* value) {
    if (cache->capacity == 0) return;
    
    // Проверяем, существует ли уже ключ
    LFUNode* existingNode = findNodeByKey(cache, key);
    if (existingNode != nullptr) {
        // Ключ существует, обновляем значение
        strncpy(existingNode->value, value, MAX_VALUE_LENGTH - 1);
        existingNode->value[MAX_VALUE_LENGTH - 1] = '\0';
        
        // Обновляем частоту
        updateFrequency(cache, existingNode);
        return;
    }
    
    // Если кэш полный, удаляем наименее часто используемый элемент
    if (cache->size >= cache->capacity) {
        removeLFUNode(cache);
    }
    
    // Создаем новый узел
    LFUNode* newNode = createLFUNode(key, value);
    
    // Находим или создаем частотный список с частотой 1
    FrequencyList* freqList1 = nullptr;
    if (cache->freqHead != nullptr && cache->freqHead->frequency == 1) {
        freqList1 = cache->freqHead;
    } else {
        freqList1 = createFrequencyList(1);
        
        // Вставляем в начало
        freqList1->next = cache->freqHead;
        if (cache->freqHead != nullptr) {
            cache->freqHead->prev = freqList1;
        }
        cache->freqHead = freqList1;
        
        if (cache->freqTail == nullptr) {
            cache->freqTail = freqList1;
        }
    }
    
    // Добавляем узел в частотный список
    addNodeToFrequencyList(freqList1, newNode);
    cache->size++;
}

// Получение значения из кэша
const char* lfuCacheGet(LFUCache* cache, const char* key) {
    if (cache->capacity == 0) return nullptr;
    
    LFUNode* node = findNodeByKey(cache, key);
    if (node == nullptr) {
        return nullptr; // Ключ не найден
    }
    
    // Обновляем частоту использования
    updateFrequency(cache, node);
    
    return node->value;
}

// Вывод содержимого кэша
void lfuCachePrint(LFUCache* cache) {
    cout << "LFU кэш: " << cache->name << endl;
    cout << "Емкость: " << cache->capacity << ", Размер: " << cache->size << endl;
    cout << "Содержимое (по возрастанию частоты):" << endl;
    
    FrequencyList* currentFreq = cache->freqHead;
    while (currentFreq != nullptr) {
        cout << "  Частота " << currentFreq->frequency << ":" << endl;
        
        LFUNode* current = currentFreq->head;
        while (current != nullptr) {
            cout << "    " << current->key << " -> " << current->value;
            if (current->next != nullptr) cout << ", ";
            cout << endl;
            current = current->next;
        }
        
        currentFreq = currentFreq->next;
    }
    
    if (cache->size == 0) {
        cout << "  (пусто)" << endl;
    }
    cout << "--------------------" << endl;
}

// Получение текущего размера кэша
size_t lfuCacheSize(LFUCache* cache) {
    return cache->size;
}

// Обработка команд LFU кэша
void executeLFUCacheCommand(const char* command) {
    char cmd[50], cacheName[50], key[MAX_KEY_LENGTH], value[MAX_VALUE_LENGTH];
    size_t capacity;
    
    // LCREATE - создание LFU кэша
    if (sscanf(command, "LCREATE %49s %zu", cacheName, &capacity) == 2) {
        if (createLFUCache(cacheName, capacity)) {
            cout << "LFU кэш '" << cacheName << "' создан" << endl;
        }
    }
    // LSET - установка значения
    else if (sscanf(command, "LSET %49s \"%255[^\"]\" \"%511[^\"]\"", cacheName, key, value) == 3 ||
             sscanf(command, "LSET %49s %255s %511s", cacheName, key, value) == 3) {
        LFUCache* cache = findLFUCache(cacheName);
        if (cache) {
            lfuCacheSet(cache, key, value);
            cout << "Успешно!" << endl;
        } else {
            cout << "LFU кэш '" << cacheName << "' не найден" << endl;
        }
    }
    // LGET - получение значения
    else if (sscanf(command, "LGET %49s \"%255[^\"]\"", cacheName, key) == 2 ||
             sscanf(command, "LGET %49s %255s", cacheName, key) == 2) {
        LFUCache* cache = findLFUCache(cacheName);
        if (cache) {
            const char* result = lfuCacheGet(cache, key);
            if (result != nullptr) {
                cout << "Найдено: " << result << endl;
            } else {
                cout << "-1" << endl;
            }
        } else {
            cout << "LFU кэш '" << cacheName << "' не найден" << endl;
        }
    }
    // LPRINT - вывод кэша
    else if (sscanf(command, "LPRINT %49s", cacheName) == 1) {
        LFUCache* cache = findLFUCache(cacheName);
        if (cache) {
            lfuCachePrint(cache);
        } else {
            cout << "LFU кэш '" << cacheName << "' не найден" << endl;
        }
    }
    // LDELETE - удаление кэша
    else if (sscanf(command, "LDELETE %49s", cacheName) == 1) {
        removeLFUCache(cacheName);
    }
    // LSIZE - размер кэша
    else if (sscanf(command, "LSIZE %49s", cacheName) == 1) {
        LFUCache* cache = findLFUCache(cacheName);
        if (cache) {
            cout << "Размер: " << lfuCacheSize(cache) << "/" << cache->capacity << endl;
        } else {
            cout << "LFU кэш '" << cacheName << "' не найден" << endl;
        }
    }
    else {
        cout << "Неизвестная команда LFU кэша" << endl;
        cout << "Доступные команды:" << endl;
        cout << "  LCREATE <name> <capacity>" << endl;
        cout << "  LSET <cache> <key> <value>" << endl;
        cout << "  LGET <cache> <key>" << endl;
        cout << "  LPRINT <cache>" << endl;
        cout << "  LDELETE <cache>" << endl;
        cout << "  LSIZE <cache>" << endl;
    }
}