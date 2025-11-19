#ifndef LIST_H
#define LIST_H

struct ListNode {
    char* data;
    ListNode* next;
    ListNode* prev;
};

struct LinkedList {
    ListNode* head;
    ListNode* tail;
    int size;
    char name[50];
};

void initList(LinkedList* list, const char* name);
void freeList(LinkedList* list);
void addToFront(LinkedList* list, const char* value);
void addToBack(LinkedList* list, const char* value);
void insertAfter(LinkedList* list, const char* afterValue, const char* newValue);
void insertBefore(LinkedList* list, const char* beforeValue, const char* newValue);
void removeFromFront(LinkedList* list);
void removeFromBack(LinkedList* list);
void removeByValue(LinkedList* list, const char* value);
int findByValue(LinkedList* list, const char* value);
int getListSize(LinkedList* list);
void removeBefore(LinkedList* list, const char* beforeValue);
void removeAfter(LinkedList* list, const char* afterValue);


void executeListCommand(LinkedList* lists, int* listCount, const char* command, const char* filename);
void saveListsToFile(const char* filename, LinkedList* lists, int listCount);
void loadListsFromFile(const char* filename, LinkedList* lists, int* listCount);
LinkedList* findListByName(LinkedList* lists, int* listCount, const char* name);
LinkedList* createList(LinkedList* lists, int* listCount, const char* name);

#endif
