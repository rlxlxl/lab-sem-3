#include "set.h"
#include <iostream>
#include <cstdlib>
#include <cstring>
#include <cstdio>
using namespace std;

#define MAX_QUERY_LENGTH 1024

// Хэш-функция для строк (простой полиномиальный хэш)
unsigned int hashFunction(const char* key, int capacity) {
    unsigned int hash = 0;
    while (*key) {
        hash = (hash * 31) + *key;  // 31 - простое число для лучшего распределения
        key++;
    }
    return hash % capacity;  // Приводим к диапазону [0, capacity-1]
}

// Инициализация хэш-множества
void initSet(HashSet* set, const char* name, int initialCapacity) {
    set->buckets = (SetNode**)calloc(initialCapacity, sizeof(SetNode*));  // Нулевая инициализация
    set->capacity = initialCapacity;
    set->size = 0;
    strcpy(set->name, name);
}

// Освобождение памяти множества
void freeSet(HashSet* set) {
    for (int i = 0; i < set->capacity; i++) {
        SetNode* current = set->buckets[i];
        while (current != nullptr) {
            SetNode* next = current->next;
            free(current->key);
            free(current);
            current = next;
        }
    }
    free(set->buckets);
    set->buckets = nullptr;
    set->capacity = 0;
    set->size = 0;
}

// Увеличение размера хэш-таблицы
void resizeSet(HashSet* set) {
    int newCapacity = set->capacity * 2;
    SetNode** newBuckets = (SetNode**)calloc(newCapacity, sizeof(SetNode*));
    
    // Перераспределение всех элементов
    for (int i = 0; i < set->capacity; i++) {
        SetNode* current = set->buckets[i];
        while (current != nullptr) {
            SetNode* next = current->next;
            
            // Пересчет индекса для нового размера
            unsigned int newIndex = hashFunction(current->key, newCapacity);
            
            // Вставка в новую таблицу (в начало цепочки)
            current->next = newBuckets[newIndex];
            newBuckets[newIndex] = current;
            
            current = next;
        }
    }
    
    free(set->buckets);
    set->buckets = newBuckets;
    set->capacity = newCapacity;
}

// Добавление элемента в множество
void setAdd(HashSet* set, const char* key) {
    // Автоматическое увеличение при заполнении на 75%
    if (set->size >= set->capacity * 0.75) {
        resizeSet(set);
    }
    
    unsigned int index = hashFunction(key, set->capacity);
    
    // Проверка на дубликат
    SetNode* current = set->buckets[index];
    while (current != nullptr) {
        if (strcmp(current->key, key) == 0) {
            return;  // Элемент уже существует
        }
        current = current->next;
    }
    
    // Создание нового узла
    SetNode* newNode = (SetNode*)malloc(sizeof(SetNode));
    newNode->key = (char*)malloc(strlen(key) + 1);
    strcpy(newNode->key, key);
    // Вставка в начало цепочки
    newNode->next = set->buckets[index];
    set->buckets[index] = newNode;
    set->size++;
}

// Удаление элемента из множества
void setRemove(HashSet* set, const char* key) {
    unsigned int index = hashFunction(key, set->capacity);
    
    SetNode* current = set->buckets[index];
    SetNode* prev = nullptr;
    
    while (current != nullptr) {
        if (strcmp(current->key, key) == 0) {
            // Удаление из цепочки
            if (prev == nullptr) {
                set->buckets[index] = current->next;  // Удаление первого элемента
            } else {
                prev->next = current->next;  // Удаление из середины/конца
            }
            
            free(current->key);
            free(current);
            set->size--;
            return;
        }
        prev = current;
        current = current->next;
    }
}

// Проверка наличия элемента в множестве
bool setContains(HashSet* set, const char* key) {
    unsigned int index = hashFunction(key, set->capacity);
    
    SetNode* current = set->buckets[index];
    while (current != nullptr) {
        if (strcmp(current->key, key) == 0) {
            return true;
        }
        current = current->next;
    }
    return false;
}

// Получение размера множества
int getSetSize(HashSet* set) {
    return set->size;
}

// Объединение двух множеств
void setUnion(HashSet* result, HashSet* set1, HashSet* set2) {
    // Добавляем все элементы из первого множества
    for (int i = 0; i < set1->capacity; i++) {
        SetNode* current = set1->buckets[i];
        while (current != nullptr) {
            setAdd(result, current->key);
            current = current->next;
        }
    }
    
    // Добавляем все элементы из второго множества
    for (int i = 0; i < set2->capacity; i++) {
        SetNode* current = set2->buckets[i];
        while (current != nullptr) {
            setAdd(result, current->key);
            current = current->next;
        }
    }
}

// Пересечение двух множеств
void setIntersection(HashSet* result, HashSet* set1, HashSet* set2) {
    // Добавляем только элементы, присутствующие в обоих множествах
    for (int i = 0; i < set1->capacity; i++) {
        SetNode* current = set1->buckets[i];
        while (current != nullptr) {
            if (setContains(set2, current->key)) {
                setAdd(result, current->key);
            }
            current = current->next;
        }
    }
}

// Разность множеств (set1 - set2)
void setDifference(HashSet* result, HashSet* set1, HashSet* set2) {
    // Добавляем элементы из set1, которых нет в set2
    for (int i = 0; i < set1->capacity; i++) {
        SetNode* current = set1->buckets[i];
        while (current != nullptr) {
            if (!setContains(set2, current->key)) {
                setAdd(result, current->key);
            }
            current = current->next;
        }
    }
}

// Поиск множества по имени
HashSet* findSetByName(HashSet* sets, int* setCount, const char* name) {
    for (int i = 0; i < *setCount; i++) {
        if (strcmp(sets[i].name, name) == 0) {
            return &sets[i];
        }
    }
    return nullptr;
}

// Создание нового множества
HashSet* createSet(HashSet* sets, int* setCount, const char* name) {
    HashSet* newSet = &sets[*setCount];
    initSet(newSet, name);
    (*setCount)++;
    return newSet;
}

// Сохранение множеств в файл
void saveSetsToFile(const char* filename, HashSet* sets, int setCount) {
    FILE* file = fopen(filename, "w");
    if (!file) {
        cout << "Ошибка открытия файла для записи" << endl;
        return;
    }
    
    for (int i = 0; i < setCount; i++) {
        fprintf(file, "SET %s\n", sets[i].name);
        
        // Сохраняем все элементы (обход всех бакетов)
        for (int j = 0; j < sets[i].capacity; j++) {
            SetNode* current = sets[i].buckets[j];
            while (current != nullptr) {
                fprintf(file, "  SETDATA %s\n", current->key);
                current = current->next;
            }
        }
        fprintf(file, "END_SET\n");
    }
    
    fclose(file);
}

// Загрузка множеств из файла
void loadSetsFromFile(const char* filename, HashSet* sets, int* setCount) {
    FILE* file = fopen(filename, "r");
    if (!file) return;
    
    char line[MAX_QUERY_LENGTH];
    HashSet* currentSet = nullptr;
    
    while (fgets(line, sizeof(line), file)) {
        line[strcspn(line, "\n")] = 0;
        
        if (strncmp(line, "SET ", 4) == 0) {
            char setName[256];
            sscanf(line + 4, "%255s", setName);
            currentSet = findSetByName(sets, setCount, setName);
            if (!currentSet) {
                currentSet = createSet(sets, setCount, setName);
            }
        } 
        else if (strncmp(line, "  SETDATA ", 10) == 0 && currentSet) {
            setAdd(currentSet, line + 10);
        }
        else if (strcmp(line, "END_SET") == 0) {
            currentSet = nullptr;
        }
    }
    
    fclose(file);
}

// Обработка команд для множеств
void executeSetCommand(HashSet* sets, int* setCount, const char* command, const char* filename) {
    char cmd[50], setName[256], value[512], setName2[256], resultName[256];
    
    // HADD - добавление элемента
    if (sscanf(command, "HADD %255s %511[^\n]", setName, value) == 2) {
        HashSet* set = findSetByName(sets, setCount, setName);
        if (!set) {
            set = createSet(sets, setCount, setName);
        }
        setAdd(set, value);
        saveSetsToFile(filename, sets, *setCount);
        cout << "Успешно!" << endl;
    }
    // HDEL - удаление элемента
    else if (sscanf(command, "HDEL %255s %511[^\n]", setName, value) == 2) {
        HashSet* set = findSetByName(sets, setCount, setName);
        if (set) {
            setRemove(set, value);
            saveSetsToFile(filename, sets, *setCount);
            cout << "Успешно!" << endl;
        } else {
            cout << "Множество не найдено" << endl;
        }
    }
    // H_AT - проверка наличия элемента
    else if (sscanf(command, "H_AT %255s %511[^\n]", setName, value) == 2) {
        HashSet* set = findSetByName(sets, setCount, setName);
        if (set) {
            if (setContains(set, value)) {
                cout << "Элемент '" << value << "' присутствует в множестве" << endl;
            } else {
                cout << "Элемент '" << value << "' отсутствует в множестве" << endl;
            }
        } else {
            cout << "Множество не найдено" << endl;
        }
    }
    // HSIZE - получение размера множества
    else if (sscanf(command, "HSIZE %255s", setName) == 1) {
        HashSet* set = findSetByName(sets, setCount, setName);
        if (set) {
            cout << set->size << endl;
        } else {
            cout << "Множество не найдено" << endl;
        }
    }
    // HUNION - объединение множеств
    else if (sscanf(command, "HUNION %255s %255s %255s", resultName, setName, setName2) == 3) {
        HashSet* set1 = findSetByName(sets, setCount, setName);
        HashSet* set2 = findSetByName(sets, setCount, setName2);
        
        if (set1 && set2) {
            HashSet* result = findSetByName(sets, setCount, resultName);
            if (!result) {
                result = createSet(sets, setCount, resultName);
            } else {
                freeSet(result);
                initSet(result, resultName);
            }
            
            setUnion(result, set1, set2);
            saveSetsToFile(filename, sets, *setCount);
            cout << "Объединение выполнено успешно!" << endl;
        } else {
            cout << "Одно из множеств не найдено" << endl;
        }
    }
    // HINTER - пересечение множеств
    else if (sscanf(command, "HINTER %255s %255s %255s", resultName, setName, setName2) == 3) {
        HashSet* set1 = findSetByName(sets, setCount, setName);
        HashSet* set2 = findSetByName(sets, setCount, setName2);
        
        if (set1 && set2) {
            HashSet* result = findSetByName(sets, setCount, resultName);
            if (!result) {
                result = createSet(sets, setCount, resultName);
            } else {
                freeSet(result);
                initSet(result, resultName);
            }
            
            setIntersection(result, set1, set2);
            saveSetsToFile(filename, sets, *setCount);
            cout << "Пересечение выполнено успешно!" << endl;
        } else {
            cout << "Одно из множеств не найдено" << endl;
        }
    }
    // HDIFF - разность множеств
    else if (sscanf(command, "HDIFF %255s %255s %255s", resultName, setName, setName2) == 3) {
        HashSet* set1 = findSetByName(sets, setCount, setName);
        HashSet* set2 = findSetByName(sets, setCount, setName2);
        
        if (set1 && set2) {
            HashSet* result = findSetByName(sets, setCount, resultName);
            if (!result) {
                result = createSet(sets, setCount, resultName);
            } else {
                freeSet(result);
                initSet(result, resultName);
            }
            
            setDifference(result, set1, set2);
            saveSetsToFile(filename, sets, *setCount);
            cout << "Разность выполнена успешно!" << endl;
        } else {
            cout << "Одно из множеств не найдено" << endl;
        }
    }
    else {
        cout << "Неизвестная команда множества" << endl;
    }
}