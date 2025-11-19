#ifndef FLIST_H
#define FLIST_H

struct FListNode {
    char* data;
    FListNode* next;
};

struct FLinkedList {
    FListNode* head;
    FListNode* tail;
    int size;
    char name[50];
};

void initFList(FLinkedList* list, const char* name);
void freeFList(FLinkedList* list);
void addToFrontF(FLinkedList* list, const char* value);
void addToBackF(FLinkedList* list, const char* value);
void insertAfterF(FLinkedList* list, const char* afterValue, const char* newValue);
void insertBeforeF(FLinkedList* list, const char* beforeValue, const char* newValue);
void removeFromFrontF(FLinkedList* list);
void removeFromBackF(FLinkedList* list);
void removeByValueF(FLinkedList* list, const char* value);
const char* findByValueF(FLinkedList* list, const char* value);
int getFListSize(FLinkedList* list);
void removeBeforeF(FLinkedList* list, const char* beforeValue);
void removeAfterF(FLinkedList* list, const char* afterValue);

void executeFListCommand(FLinkedList* lists, int* listCount, const char* command, const char* filename);
void saveFListsToFile(const char* filename, FLinkedList* lists, int listCount);
void loadFListsFromFile(const char* filename, FLinkedList* lists, int* listCount);
FLinkedList* findFListByName(FLinkedList* lists, int* listCount, const char* name);
FLinkedList* createFList(FLinkedList* lists, int* listCount, const char* name);

void printFList(FLinkedList* lists, int* listCount, const char* listName);

#endif
