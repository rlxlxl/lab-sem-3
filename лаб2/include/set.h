#ifndef SET_H
#define SET_H

struct SetNode {
    char* key;
    SetNode* next;
};

struct HashSet {
    SetNode** buckets;
    int capacity;
    int size;
    char name[50];
};

// Базовые операции
void initSet(HashSet* set, const char* name, int initialCapacity = 16);
void freeSet(HashSet* set);
void setAdd(HashSet* set, const char* key);
void setRemove(HashSet* set, const char* key);
bool setContains(HashSet* set, const char* key);
int getSetSize(HashSet* set);

// Дополнительные операции
void setUnion(HashSet* result, HashSet* set1, HashSet* set2);
void setIntersection(HashSet* result, HashSet* set1, HashSet* set2);
void setDifference(HashSet* result, HashSet* set1, HashSet* set2);

// Команды
void executeSetCommand(HashSet* sets, int* setCount, const char* command, const char* filename);
void saveSetsToFile(const char* filename, HashSet* sets, int setCount);
void loadSetsFromFile(const char* filename, HashSet* sets, int* setCount);
HashSet* findSetByName(HashSet* sets, int* setCount, const char* name);
HashSet* createSet(HashSet* sets, int* setCount, const char* name);

#endif