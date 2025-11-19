#include "hash_table.h"
#include "array.h"
#include "flist.h"
#include <cstring>
#include <cstdio>
#include <iostream>

using namespace std;

// Глобальные переменные для управления хэш-таблицами
static HashTable hashTables[MAX_HASH_TABLES];
static int hashTableCount = 0;

// Функция для создания копии строки
char* strdup(const char* s) {
    if (s == nullptr) return nullptr;
    size_t len = strlen(s) + 1;
    char* result = (char*)malloc(len);
    if (result) memcpy(result, s, len);
    return result;
}

// Хэш-таблица с методом цепочек
struct ChainHashTableImpl {
public:
    FLinkedList* buckets;
    size_t capacity;
    size_t size;
    double loadFactorThreshold;
    
    ChainHashTableImpl(size_t cap, double threshold = 0.75) 
        : capacity(cap), size(0), loadFactorThreshold(threshold) {
        buckets = (FLinkedList*)malloc(capacity * sizeof(FLinkedList));
        for (size_t i = 0; i < capacity; i++) {
            char name[20];
            snprintf(name, sizeof(name), "bucket_%zu", i);
            initFList(&buckets[i], name);
        }
    }
    
    ~ChainHashTableImpl() {
        for (size_t i = 0; i < capacity; i++) {
            freeFList(&buckets[i]);
        }
        free(buckets);
    }
    
    size_t hash(const char* key, size_t tableSize) const {
        size_t hash = 5381;
        for (const char* p = key; *p != '\0'; p++) {
            hash = ((hash << 5) + hash) + *p;
        }
        return hash % tableSize;
    }
    
    void resize() {
        size_t newCapacity = capacity * 2;
        FLinkedList* newBuckets = (FLinkedList*)malloc(newCapacity * sizeof(FLinkedList));
        
        for (size_t i = 0; i < newCapacity; i++) {
            char name[20];
            snprintf(name, sizeof(name), "bucket_%zu", i);
            initFList(&newBuckets[i], name);
        }
        
        for (size_t i = 0; i < capacity; i++) {
            FListNode* current = buckets[i].head;
            while (current != nullptr) {
                char* dataCopy = strdup(current->data);
                char* colonPos = strchr(dataCopy, ':');
                if (colonPos != nullptr) {
                    *colonPos = '\0';
                    const char* key = dataCopy;
                    const char* value = colonPos + 1;
                    
                    size_t newIndex = hash(key, newCapacity);
                    char newEntry[MAX_KEY_LENGTH + MAX_VALUE_LENGTH + 2];
                    snprintf(newEntry, sizeof(newEntry), "%s:%s", key, value);
                    addToBackF(&newBuckets[newIndex], newEntry);
                }
                free(dataCopy);
                current = current->next;
            }
        }
        
        for (size_t i = 0; i < capacity; i++) {
            freeFList(&buckets[i]);
        }
        free(buckets);
        
        buckets = newBuckets;
        capacity = newCapacity;
        cout << "Таблица увеличена до " << capacity << endl;
    }
    
    void insert(const char* key, const char* value) {
        if (static_cast<double>(size) / capacity >= loadFactorThreshold) {
            resize();
        }
        
        size_t index = hash(key, capacity);
        
        FListNode* current = buckets[index].head;
        while (current != nullptr) {
            char* dataCopy = strdup(current->data);
            char* colonPos = strchr(dataCopy, ':');
            if (colonPos != nullptr) {
                *colonPos = '\0';
                bool keyExists = (strcmp(dataCopy, key) == 0);
                free(dataCopy);
                
                if (keyExists) {
                    free(current->data);
                    char newEntry[MAX_KEY_LENGTH + MAX_VALUE_LENGTH + 2];
                    snprintf(newEntry, sizeof(newEntry), "%s:%s", key, value);
                    current->data = strdup(newEntry);
                    return;
                }
            } else {
                free(dataCopy);
            }
            current = current->next;
        }
        
        char newEntry[MAX_KEY_LENGTH + MAX_VALUE_LENGTH + 2];
        snprintf(newEntry, sizeof(newEntry), "%s:%s", key, value);
        addToFrontF(&buckets[index], newEntry);
        size++;
    }
    
    const char* find(const char* key) const {
        size_t index = hash(key, capacity);
        
        FListNode* current = buckets[index].head;
        while (current != nullptr) {
            char* dataCopy = strdup(current->data);
            char* colonPos = strchr(dataCopy, ':');
            if (colonPos != nullptr) {
                *colonPos = '\0';
                bool keyMatches = (strcmp(dataCopy, key) == 0);
                if (keyMatches) {
                    const char* value = colonPos + 1;
                    char* result = strdup(value);
                    free(dataCopy);
                    return result;
                }
            }
            free(dataCopy);
            current = current->next;
        }
        return nullptr;
    }
    

    const char* remove(const char* key) {
        size_t index = hash(key, capacity);
        
        FListNode* current = buckets[index].head;
        FListNode* prev = nullptr;
        
        while (current != nullptr) {
            char* dataCopy = strdup(current->data);
            char* colonPos = strchr(dataCopy, ':');
            if (colonPos != nullptr) {
                *colonPos = '\0';
                bool keyMatches = (strcmp(dataCopy, key) == 0);
                
                if (keyMatches) {
                    // Сохраняем значение перед удалением
                    const char* value = colonPos + 1;
                    char* removedValue = strdup(value);
                    
                    // Удаляем элемент из списка
                    if (prev == nullptr) {
                        buckets[index].head = current->next;
                    } else {
                        prev->next = current->next;
                    }
                    
                    // Освобождаем память узла
                    free(current->data);
                    free(current);
                    buckets[index].size--;
                    size--;
                    
                    free(dataCopy);
                    return removedValue;
                }
            }
            free(dataCopy);
            prev = current;
            current = current->next;
        }
        return nullptr;
    }
    
    void print() const {
        for (size_t i = 0; i < capacity; i++) {
            if (buckets[i].size > 0) {
                cout << "  Bucket " << i << " (" << buckets[i].size << "): ";
                FListNode* current = buckets[i].head;
                while (current != nullptr) {
                    cout << current->data;
                    if (current->next != nullptr) cout << " -> ";
                    current = current->next;
                }
                cout << endl;
            }
        }
    }
};

// Хэш-таблица с открытой адресацией
struct OpenAddressingHashTableImpl {
public:
    DynamicArray table;
    size_t capacity;
    size_t size;
    double loadFactorThreshold;
    
    enum CellState { EMPTY, OCCUPIED, DELETED };
    
    OpenAddressingHashTableImpl(size_t cap, double threshold = 0.7) 
        : capacity(cap), size(0), loadFactorThreshold(threshold) {
        initArray(&table, capacity);
        for (size_t i = 0; i < capacity; i++) {
            pushBack(&table, "EMPTY");
        }
    }
    
    ~OpenAddressingHashTableImpl() {
        freeArray(&table);
    }
    
    size_t hash(const char* key, size_t tableSize) const {
        size_t hash = 5381;
        for (const char* p = key; *p != '\0'; p++) {
            hash = ((hash << 5) + hash) + *p;
        }
        return hash % tableSize;
    }
    
    size_t probe(const char* key, size_t i, size_t tableSize) const {
        return (hash(key, tableSize) + i) % tableSize;
    }
    
    void resize() {
        size_t newCapacity = capacity * 2;
        DynamicArray newTable;
        initArray(&newTable, newCapacity);
        
        for (size_t i = 0; i < newCapacity; i++) {
            pushBack(&newTable, "EMPTY");
        }
        
        for (size_t i = 0; i < capacity; i++) {
            const char* cellData = getAt(&table, i);
            if (strncmp(cellData, "OCCUPIED:", 9) == 0) {
                const char* keyStart = cellData + 9;
                const char* valueStart = strchr(keyStart, ':');
                if (valueStart != nullptr) {
                    size_t keyLen = valueStart - keyStart;
                    char key[MAX_KEY_LENGTH];
                    strncpy(key, keyStart, keyLen);
                    key[keyLen] = '\0';
                    const char* value = valueStart + 1;
                    
                    size_t j = 0;
                    size_t newIndex;
                    do {
                        newIndex = probe(key, j, newCapacity);
                        j++;
                    } while (strcmp(getAt(&newTable, newIndex), "EMPTY") != 0);
                    
                    char newEntry[MAX_KEY_LENGTH + MAX_VALUE_LENGTH + 20];
                    snprintf(newEntry, sizeof(newEntry), "OCCUPIED:%s:%s", key, value);
                    replaceAt(&newTable, newIndex, newEntry);
                }
            }
        }
        
        freeArray(&table);
        table = newTable;
        capacity = newCapacity;
        cout << "Таблица увеличена до " << capacity << endl;
    }
    
    void insert(const char* key, const char* value) {
        if (static_cast<double>(size) / capacity >= loadFactorThreshold) {
            resize();
        }
        
        size_t i = 0;
        size_t index;
        size_t firstDeleted = capacity;
        
        do {
            index = probe(key, i, capacity);
            const char* cellData = getAt(&table, index);
            
            if (strcmp(cellData, "EMPTY") == 0) {
                break;
            }
            
            if (strncmp(cellData, "DELETED:", 8) == 0) {
                if (firstDeleted == capacity) {
                    firstDeleted = index;
                }
            } else if (strncmp(cellData, "OCCUPIED:", 9) == 0) {
                const char* keyStart = cellData + 9;
                const char* valueStart = strchr(keyStart, ':');
                if (valueStart != nullptr) {
                    size_t keyLen = valueStart - keyStart;
                    char storedKey[MAX_KEY_LENGTH];
                    strncpy(storedKey, keyStart, keyLen);
                    storedKey[keyLen] = '\0';
                    
                    if (strcmp(storedKey, key) == 0) {
                        char newEntry[MAX_KEY_LENGTH + MAX_VALUE_LENGTH + 20];
                        snprintf(newEntry, sizeof(newEntry), "OCCUPIED:%s:%s", key, value);
                        replaceAt(&table, index, newEntry);
                        return;
                    }
                }
            }
            
            i++;
        } while (i < capacity);
        
        if (firstDeleted != capacity) {
            index = firstDeleted;
        }
        
        char newEntry[MAX_KEY_LENGTH + MAX_VALUE_LENGTH + 20];
        snprintf(newEntry, sizeof(newEntry), "OCCUPIED:%s:%s", key, value);
        replaceAt(&table, index, newEntry);
        size++;
    }
    
    const char* find(const char* key) const {
        size_t i = 0;
        size_t index;
        
        do {
            index = probe(key, i, capacity);
            const char* cellData = getAt(&table, index);
            
            if (strcmp(cellData, "EMPTY") == 0) {
                break;
            }
            
            if (strncmp(cellData, "OCCUPIED:", 9) == 0) {
                const char* keyStart = cellData + 9;
                const char* valueStart = strchr(keyStart, ':');
                if (valueStart != nullptr) {
                    size_t keyLen = valueStart - keyStart;
                    char storedKey[MAX_KEY_LENGTH];
                    strncpy(storedKey, keyStart, keyLen);
                    storedKey[keyLen] = '\0';
                    
                    if (strcmp(storedKey, key) == 0) {
                        return valueStart + 1;
                    }
                }
            }
            
            i++;
        } while (i < capacity);
        
        return nullptr;
    }
    

    const char* remove(const char* key) {
        size_t i = 0;
        size_t index;
        
        do {
            index = probe(key, i, capacity);
            const char* cellData = getAt(&table, index);
            
            if (strcmp(cellData, "EMPTY") == 0) {
                break;
            }
            
            if (strncmp(cellData, "OCCUPIED:", 9) == 0) {
                const char* keyStart = cellData + 9;
                const char* valueStart = strchr(keyStart, ':');
                if (valueStart != nullptr) {
                    size_t keyLen = valueStart - keyStart;
                    char storedKey[MAX_KEY_LENGTH];
                    strncpy(storedKey, keyStart, keyLen);
                    storedKey[keyLen] = '\0';
                    
                    if (strcmp(storedKey, key) == 0) {
                        // Сохраняем значение перед пометкой как удаленное
                        const char* value = valueStart + 1;
                        char* removedValue = strdup(value);
                        
                        // Помечаем ячейку как удаленную
                        char deletedEntry[MAX_KEY_LENGTH + MAX_VALUE_LENGTH + 20];
                        snprintf(deletedEntry, sizeof(deletedEntry), "DELETED:%s:%s", key, value);
                        replaceAt(&table, index, deletedEntry);
                        size--;
                        
                        return removedValue;
                    }
                }
            }
            
            i++;
        } while (i < capacity);
        
        return nullptr;
    }
    
    void print() const {
        for (size_t i = 0; i < capacity; i++) {
            const char* cellData = getAt(&table, i);
            if (strncmp(cellData, "OCCUPIED:", 9) == 0) {
                cout << "  [" << i << "]: " << cellData << endl;
            }
        }
    }
};

// ===== ФУНКЦИИ УПРАВЛЕНИЯ ХЭШ-ТАБЛИЦАМИ =====

void initHashTableSystem() {
    hashTableCount = 0;
}

HashTable* createHashTable(const char* name, HashTableType type, size_t capacity) {
    if (hashTableCount >= MAX_HASH_TABLES) {
        cout << "Достигнуто максимальное количество хэш-таблиц" << endl;
        return nullptr;
    }
    
    HashTable* ht = &hashTables[hashTableCount];
    strncpy(ht->name, name, sizeof(ht->name) - 1);
    ht->name[sizeof(ht->name) - 1] = '\0';
    ht->type = type;
    ht->capacity = capacity;
    ht->size = 0;
    
    if (type == CHAINING) {
        ht->table = new ChainHashTableImpl(capacity, 0.75);
    } else {
        ht->table = new OpenAddressingHashTableImpl(capacity, 0.7);
    }
    
    hashTableCount++;
    return ht;
}

HashTable* findHashTable(const char* name) {
    for (int i = 0; i < hashTableCount; i++) {
        if (strcmp(hashTables[i].name, name) == 0) {
            return &hashTables[i];
        }
    }
    return nullptr;
}

void removeHashTable(const char* name) {
    for (int i = 0; i < hashTableCount; i++) {
        if (strcmp(hashTables[i].name, name) == 0) {
            if (hashTables[i].type == CHAINING) {
                delete static_cast<ChainHashTableImpl*>(hashTables[i].table);
            } else {
                delete static_cast<OpenAddressingHashTableImpl*>(hashTables[i].table);
            }
            
            for (int j = i; j < hashTableCount - 1; j++) {
                hashTables[j] = hashTables[j + 1];
            }
            hashTableCount--;
            cout << "Хэш-таблица '" << name << "' удалена" << endl;
            return;
        }
    }
    cout << "Хэш-таблица '" << name << "' не найдена" << endl;
}

void hashTableInsert(HashTable* ht, const char* key, const char* value) {
    if (ht->type == CHAINING) {
        ChainHashTableImpl* impl = static_cast<ChainHashTableImpl*>(ht->table);
        impl->insert(key, value);
        ht->size = impl->size;
    } else {
        OpenAddressingHashTableImpl* impl = static_cast<OpenAddressingHashTableImpl*>(ht->table);
        impl->insert(key, value);
        ht->size = impl->size;
    }
}

const char* hashTableFind(HashTable* ht, const char* key) {
    if (ht->type == CHAINING) {
        ChainHashTableImpl* impl = static_cast<ChainHashTableImpl*>(ht->table);
        return impl->find(key);
    } else {
        OpenAddressingHashTableImpl* impl = static_cast<OpenAddressingHashTableImpl*>(ht->table);
        return impl->find(key);
    }
}


const char* hashTableRemove(HashTable* ht, const char* key) {
    if (ht->type == CHAINING) {
        ChainHashTableImpl* impl = static_cast<ChainHashTableImpl*>(ht->table);
        const char* removedValue = impl->remove(key);
        if (removedValue != nullptr) {
            ht->size = impl->size;
        }
        return removedValue;
    } else {
        OpenAddressingHashTableImpl* impl = static_cast<OpenAddressingHashTableImpl*>(ht->table);
        const char* removedValue = impl->remove(key);
        if (removedValue != nullptr) {
            ht->size = impl->size;
        }
        return removedValue;
    }
}

void hashTablePrint(HashTable* ht) {
    cout << "Хэш-таблица: " << ht->name << endl;
    cout << "Тип: " << (ht->type == CHAINING ? "Метод цепочек" : "Открытая адресация") << endl;
    cout << "Размер: " << ht->size << "/" << ht->capacity << endl;
    cout << "Содержимое:" << endl;
    
    if (ht->type == CHAINING) {
        ChainHashTableImpl* impl = static_cast<ChainHashTableImpl*>(ht->table);
        impl->print();
    } else {
        OpenAddressingHashTableImpl* impl = static_cast<OpenAddressingHashTableImpl*>(ht->table);
        impl->print();
    }
    cout << "--------------------" << endl;
}

size_t hashTableSize(HashTable* ht) {
    return ht->size;
}

void saveHashTablesToFile(const char* filename) {
    FILE* file = fopen(filename, "w");
    if (!file) {
        cout << "Ошибка открытия файла" << endl;
        return;
    }
    
    for (int i = 0; i < hashTableCount; i++) {
        HashTable* ht = &hashTables[i];
        fprintf(file, "HASH_TABLE %s %d %zu\n", ht->name, ht->type, ht->capacity);
        fprintf(file, "END_HASH_TABLE\n");
    }
    
    fclose(file);
    cout << "Хэш-таблицы сохранены в '" << filename << "'" << endl;
}

void loadHashTablesFromFile(const char* filename) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        cout << "Ошибка открытия файла" << endl;
        return;
    }
    
    char line[1024];
    while (fgets(line, sizeof(line), file)) {
        line[strcspn(line, "\n")] = 0;
        
        if (strncmp(line, "HASH_TABLE ", 11) == 0) {
            char name[50];
            int type;
            size_t capacity;
            sscanf(line + 11, "%49s %d %zu", name, &type, &capacity);
            createHashTable(name, static_cast<HashTableType>(type), capacity);
        }
    }
    
    fclose(file);
    cout << "Хэш-таблицы загружены из '" << filename << "'" << endl;
}

void convertToRoman(int number, char* result, size_t resultSize) {
    if (number <= 0 || number > 3999) {
        strncpy(result, "ERROR", resultSize - 1);
        result[resultSize - 1] = '\0';
        return;
    }
    
    struct RomanDigit {
        int value;
        const char* numeral;
    };
    
    const RomanDigit romanNumerals[] = {
        {1000, "M"}, {900, "CM"}, {500, "D"}, {400, "CD"},
        {100, "C"}, {90, "XC"}, {50, "L"}, {40, "XL"},
        {10, "X"}, {9, "IX"}, {5, "V"}, {4, "IV"}, {1, "I"}
    };
    
    const int numNumerals = sizeof(romanNumerals) / sizeof(romanNumerals[0]);
    
    char temp[50] = "";
    int num = number;
    
    for (int i = 0; i < numNumerals; i++) {
        while (num >= romanNumerals[i].value) {
            strcat(temp, romanNumerals[i].numeral);
            num -= romanNumerals[i].value;
        }
    }
    
    strncpy(result, temp, resultSize - 1);
    result[resultSize - 1] = '\0';
}

void executeHashTableCommand(const char* command) {
    char cmd[50], tableName[50], key[MAX_KEY_LENGTH], value[MAX_VALUE_LENGTH];
    int number;
    
    if (sscanf(command, "HCREATE %49s %49s", tableName, cmd) == 2) {
        HashTableType type;
        if (strcmp(cmd, "CHAINING") == 0) {
            type = CHAINING;
        } else if (strcmp(cmd, "OPEN_ADDRESSING") == 0) {
            type = OPEN_ADDRESSING;
        } else {
            cout << "Неизвестный тип. Используйте CHAINING или OPEN_ADDRESSING" << endl;
            return;
        }
        
        if (createHashTable(tableName, type, 16)) {
            cout << "Хэш-таблица '" << tableName << "' создана" << endl;
        }
    }
    else if (sscanf(command, "HINSERT %49s \"%255[^\"]\" \"%511[^\"]\"", tableName, key, value) == 3 ||
             sscanf(command, "HINSERT %49s %255s %511s", tableName, key, value) == 3) {
        HashTable* ht = findHashTable(tableName);
        if (ht) {
            hashTableInsert(ht, key, value);
            cout << "Успешно!" << endl;
        } else {
            cout << "Хэш-таблица '" << tableName << "' не найдена" << endl;
        }
    }
    else if (sscanf(command, "HGET %49s \"%255[^\"]\"", tableName, key) == 2 ||
             sscanf(command, "HGET %49s %255s", tableName, key) == 2) {
        HashTable* ht = findHashTable(tableName);
        if (ht) {
            const char* foundValue = hashTableFind(ht, key);
            if (foundValue != nullptr) {
                cout << "Найдено: " << foundValue << endl;
                if (ht->type == CHAINING) {
                    free((void*)foundValue);
                }
            } else {
                cout << "Ключ '" << key << "' не найден" << endl;
            }
        } else {
            cout << "Хэш-таблица '" << tableName << "' не найдена" << endl;
        }
    }
    
    else if (sscanf(command, "HREMOVE %49s \"%255[^\"]\"", tableName, key) == 2 ||
             sscanf(command, "HREMOVE %49s %255s", tableName, key) == 2) {
        HashTable* ht = findHashTable(tableName);
        if (ht) {
            const char* removedValue = hashTableRemove(ht, key);
            if (removedValue != nullptr) {
                cout << "Успешно! Удалено: " << key << " -> " << removedValue << endl;
                free((void*)removedValue); // Освобождаем память значения
            } else {
                cout << "Ключ '" << key << "' не найден" << endl;
            }
        } else {
            cout << "Хэш-таблица '" << tableName << "' не найдена" << endl;
        }
    }
    else if (sscanf(command, "HPRINT %49s", tableName) == 1) {
        HashTable* ht = findHashTable(tableName);
        if (ht) {
            hashTablePrint(ht);
        } else {
            cout << "Хэш-таблица '" << tableName << "' не найдена" << endl;
        }
    }
    else if (sscanf(command, "HDELETE %49s", tableName) == 1) {
        removeHashTable(tableName);
    }
    else if (sscanf(command, "HSIZE %49s", tableName) == 1) {
        HashTable* ht = findHashTable(tableName);
        if (ht) {
            cout << "Размер: " << hashTableSize(ht) << endl;
        } else {
            cout << "Хэш-таблица '" << tableName << "' не найдена" << endl;
        }
    }
    else if (strncmp(command, "HSAVE ", 6) == 0) {
        saveHashTablesToFile(command + 6);
    }
    else if (strncmp(command, "HLOAD ", 6) == 0) {
        loadHashTablesFromFile(command + 6);
    }
    else if (sscanf(command, "ROMAN %d", &number) == 1) {
        char romanResult[50];
        convertToRoman(number, romanResult, sizeof(romanResult));
        cout << "Число " << number << " в римской системе: " << romanResult << endl;
    }
    else if (sscanf(command, "ROMAN_INSERT %49s \"%255[^\"]\" %d", tableName, key, &number) == 3 ||
             sscanf(command, "ROMAN_INSERT %49s %255s %d", tableName, key, &number) == 3) {
        HashTable* ht = findHashTable(tableName);
        if (ht) {
            char romanValue[50];
            convertToRoman(number, romanValue, sizeof(romanValue));
            hashTableInsert(ht, key, romanValue);
            cout << "Успешно! Добавлено: " << key << " -> " << romanValue << endl;
        } else {
            cout << "Хэш-таблица '" << tableName << "' не найдена" << endl;
        }
    }
    else {
        cout << "Неизвестная команда" << endl;
        cout << "Доступные команды:" << endl;
        cout << "  HCREATE <name> CHAINING|OPEN_ADDRESSING" << endl;
        cout << "  HINSERT <table> <key> <value>" << endl;
        cout << "  HGET <table> <key>" << endl;
        cout << "  HREMOVE <table> <key>" << endl;
        cout << "  HPRINT <table>" << endl;
        cout << "  HDELETE <table>" << endl;
        cout << "  HSIZE <table>" << endl;
        cout << "  HSAVE <filename>" << endl;
        cout << "  HLOAD <filename>" << endl;
        cout << "  ROMAN <number> - преобразовать число в римскую цифру" << endl;
        cout << "  ROMAN_INSERT <table> <key> <number> - преобразовать и добавить в таблицу" << endl;
    }
}