#include "flist.h"
#include <iostream>
#include <cstdlib>
#include <cstring>
#include <cstdio>
using namespace std;

#define MAX_QUERY_LENGTH 1024

void initFList(FLinkedList* list, const char* name) {
    list->head = nullptr;
    list->tail = nullptr;
    list->size = 0;
    strcpy(list->name, name);
}

void freeFList(FLinkedList* list) {
    FListNode* current = list->head;
    while (current != nullptr) {
        FListNode* next = current->next;
        free(current->data);
        free(current);
        current = next;
    }
    list->head = nullptr;
    list->tail = nullptr;
    list->size = 0;
}

void addToFrontF(FLinkedList* list, const char* value) {
    FListNode* newNode = (FListNode*)malloc(sizeof(FListNode));
    newNode->data = (char*)malloc(strlen(value) + 1);
    strcpy(newNode->data, value);
    newNode->next = list->head;
    
    list->head = newNode;

    if (list->tail == nullptr) {
        list->tail = newNode;
    }
    list->size++;
}

void addToBackF(FLinkedList* list, const char* value) {
    FListNode* newNode = (FListNode*)malloc(sizeof(FListNode));
    newNode->data = (char*)malloc(strlen(value) + 1);
    strcpy(newNode->data, value);
    newNode->next = nullptr;
    
    if (list->tail == nullptr) {
        list->head = newNode;
        list->tail = newNode;
    } else {
        list->tail->next = newNode;
        list->tail = newNode;
    }
    list->size++;
}

void insertAfterF(FLinkedList* list, const char* afterValue, const char* newValue) {
    FListNode* current = list->head;
    while (current != nullptr) {
        if (strcmp(current->data, afterValue) == 0) {
            FListNode* newNode = (FListNode*)malloc(sizeof(FListNode));
            newNode->data = (char*)malloc(strlen(newValue) + 1);
            strcpy(newNode->data, newValue);

            newNode->next = current->next;
            current->next = newNode;
            
            if (current == list->tail) {
                list->tail = newNode;
            }
            list->size++;
            return;
        }
        current = current->next;
    }
    cout << "Значение '" << afterValue << "' не найдено" << endl;
}

void insertBeforeF(FLinkedList* list, const char* beforeValue, const char* newValue) {
    if (list->head == nullptr) {
        cout << "Список пуст" << endl;
        return;
    }

    if (strcmp(list->head->data, beforeValue) == 0) {
        addToFrontF(list, newValue);
        return;
    }
    
    FListNode* current = list->head;
    while (current->next != nullptr) {
        if (strcmp(current->next->data, beforeValue) == 0) {
            FListNode* newNode = (FListNode*)malloc(sizeof(FListNode));
            newNode->data = (char*)malloc(strlen(newValue) + 1);
            strcpy(newNode->data, newValue);

            newNode->next = current->next;
            current->next = newNode;
            list->size++;
            return;
        }
        current = current->next;
    }
    cout << "Значение '" << beforeValue << "' не найдено" << endl;
}

void removeFromFrontF(FLinkedList* list) {
    if (list->head == nullptr) {
        cout << "Список пуст" << endl;
        return;
    }
    
    FListNode* temp = list->head;
    list->head = list->head->next;
    
    if (list->head == nullptr) {
        list->tail = nullptr;
    }
    
    free(temp->data);
    free(temp);
    list->size--;
}

void removeFromBackF(FLinkedList* list) {
    if (list->head == nullptr) {
        cout << "Список пуст" << endl;
        return;
    }
    
    if (list->head == list->tail) {
        free(list->head->data);
        free(list->head);
        list->head = nullptr;
        list->tail = nullptr;
    } else {
        FListNode* current = list->head;
        while (current->next != list->tail) {
            current = current->next;
        }
        
        free(list->tail->data);
        free(list->tail);
        current->next = nullptr;
        list->tail = current;
    }
    list->size--;
}

void removeByValueF(FLinkedList* list, const char* value) {
    if (list->head == nullptr) {
        cout << "Список пуст" << endl;
        return;
    }
    
    if (strcmp(list->head->data, value) == 0) {
        removeFromFrontF(list);
        return;
    }
    
    FListNode* current = list->head;
    while (current->next != nullptr) {
        if (strcmp(current->next->data, value) == 0) {
            FListNode* temp = current->next;
            current->next = temp->next;
            
            if (temp == list->tail) {
                list->tail = current;
            }
            
            free(temp->data);
            free(temp);
            list->size--;
            return;
        }
        current = current->next;
    }
    cout << "Значение '" << value << "' не найдено" << endl;
}

// Удаление элемента перед указанным значением
void removeBeforeF(FLinkedList* list, const char* beforeValue) {
    if (list->head == nullptr || list->head->next == nullptr) {
        cout << "Недостаточно элементов для удаления перед '" << beforeValue << "'" << endl;
        return;
    }
    
    // Особый случай - удаление перед первым элементом (невозможно)
    if (strcmp(list->head->data, beforeValue) == 0) {
        cout << "Нет элемента перед '" << beforeValue << "'" << endl;
        return;
    }
    
    // Если удаляем элемент перед вторым элементом
    if (strcmp(list->head->next->data, beforeValue) == 0) {
        removeFromFrontF(list);
        cout << "Успешно!" << endl;
        return;
    }
    
    // Ищем элемент, следующий за которым - элемент перед искомым
    FListNode* current = list->head;
    while (current->next != nullptr && current->next->next != nullptr) {
        if (strcmp(current->next->next->data, beforeValue) == 0) {
            FListNode* temp = current->next;
            current->next = temp->next;
            
            free(temp->data);
            free(temp);
            list->size--;
            cout << "Успешно!" << endl;
            return;
        }
        current = current->next;
    }
    cout << "Значение '" << beforeValue << "' не найдено или нет элемента перед ним" << endl;
}

// Удаление элемента после указанного значения
void removeAfterF(FLinkedList* list, const char* afterValue) {
    if (list->head == nullptr) {
        cout << "Список пуст" << endl;
        return;
    }
    
    FListNode* current = list->head;
    while (current != nullptr) {
        if (strcmp(current->data, afterValue) == 0) {
            if (current->next == nullptr) {
                cout << "Нет элемента после '" << afterValue << "'" << endl;
                return;
            }
            
            FListNode* temp = current->next;
            current->next = temp->next;
            
            if (temp == list->tail) {
                list->tail = current;
            }
            
            free(temp->data);
            free(temp);
            list->size--;
            cout << "Успешно!" << endl;
            return;
        }
        current = current->next;
    }
    cout << "Значение '" << afterValue << "' не найдено" << endl;
}

const char* findByValueF(FLinkedList* list, const char* value) {
    FListNode* current = list->head;
    while (current != nullptr) {
        if (strcmp(current->data, value) == 0) {
            return current->data;
        }
        current = current->next;
    }
    return nullptr;
}

int getFListSize(FLinkedList* list) {
    return list->size;
}

FLinkedList* findFListByName(FLinkedList* lists, int* listCount, const char* name) {
    for (int i = 0; i < *listCount; i++) {
        if (strcmp(lists[i].name, name) == 0) {
            return &lists[i];
        }
    }
    return nullptr;
}

FLinkedList* createFList(FLinkedList* lists, int* listCount, const char* name) {
    FLinkedList* newList = &lists[*listCount];
    initFList(newList, name);
    (*listCount)++;
    return newList;
}

void saveFListsToFile(const char* filename, FLinkedList* lists, int listCount) {
    FILE* file = fopen(filename, "w");
    if (!file) {
        cout << "Ошибка открытия файла для записи" << endl;
        return;
    }
    
    for (int i = 0; i < listCount; i++) {
        fprintf(file, "FLIST %s\n", lists[i].name);
        
        FListNode* current = lists[i].head;
        while (current != nullptr) {
            fprintf(file, "  FDATA %s\n", current->data);
            current = current->next;
        }
        fprintf(file, "END_FLIST\n");
    }
    
    fclose(file);
}

void loadFListsFromFile(const char* filename, FLinkedList* lists, int* listCount) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        return;
    }
    
    char line[MAX_QUERY_LENGTH];
    FLinkedList* currentList = nullptr;
    
    while (fgets(line, sizeof(line), file)) {
        line[strcspn(line, "\n")] = 0;
        
        if (strncmp(line, "FLIST ", 6) == 0) {
            char listName[256];
            sscanf(line + 6, "%255s", listName);
            currentList = findFListByName(lists, listCount, listName);
            if (!currentList) {
                currentList = createFList(lists, listCount, listName);
            }
        } else if (strncmp(line, "  FDATA ", 8) == 0 && currentList) {
            addToBackF(currentList, line + 8);
        } else if (strcmp(line, "END_FLIST") == 0) {
            currentList = nullptr;
        }
    }
    
    fclose(file);
}

void executeFListCommand(FLinkedList* lists, int* listCount, const char* command, const char* filename) {
    char cmd[50], listName[256], value[512], value2[512];
    
    if (sscanf(command, "FPUSH_FRONT %255s \"%511[^\"]\"", listName, value) == 2 ||
        sscanf(command, "FPUSH_FRONT %255s %511s", listName, value) == 2) {
        FLinkedList* list = findFListByName(lists, listCount, listName);
        if (!list) {
            list = createFList(lists, listCount, listName);
        }
        addToFrontF(list, value);
        saveFListsToFile(filename, lists, *listCount);
        cout << "Успешно!" << endl;
    }
    else if (sscanf(command, "FPUSH_BACK %255s \"%511[^\"]\"", listName, value) == 2 ||
             sscanf(command, "FPUSH_BACK %255s %511s", listName, value) == 2) {
        FLinkedList* list = findFListByName(lists, listCount, listName);
        if (!list) {
            list = createFList(lists, listCount, listName);
        }
        addToBackF(list, value);
        saveFListsToFile(filename, lists, *listCount);
        cout << "Успешно!" << endl;
    }
    else if (sscanf(command, "FINSERT_AFTER %255s \"%511[^\"]\" \"%511[^\"]\"", listName, value, value2) == 3 ||
             sscanf(command, "FINSERT_AFTER %255s %511s %511s", listName, value, value2) == 3) {
        FLinkedList* list = findFListByName(lists, listCount, listName);
        if (list) {
            insertAfterF(list, value, value2);
            saveFListsToFile(filename, lists, *listCount);
            cout << "Успешно!" << endl;
        } else {
            cout << "Список '" << listName << "' не найден" << endl;
        }
    }
    else if (sscanf(command, "FINSERT_BEFORE %255s \"%511[^\"]\" \"%511[^\"]\"", listName, value, value2) == 3 ||
             sscanf(command, "FINSERT_BEFORE %255s %511s %511s", listName, value, value2) == 3) {
        FLinkedList* list = findFListByName(lists, listCount, listName);
        if (list) {
            insertBeforeF(list, value, value2);
            saveFListsToFile(filename, lists, *listCount);
            cout << "Успешно!" << endl;
        } else {
            cout << "Список '" << listName << "' не найден" << endl;
        }
    }
    else if (sscanf(command, "FPOP_FRONT %255s", listName) == 1) {
        FLinkedList* list = findFListByName(lists, listCount, listName);
        if (list) {
            removeFromFrontF(list);
            saveFListsToFile(filename, lists, *listCount);
            cout << "Успешно!" << endl;
        } else {
            cout << "Список не найден" << endl;
        }
    }
    else if (sscanf(command, "FPOP_BACK %255s", listName) == 1) {
        FLinkedList* list = findFListByName(lists, listCount, listName);
        if (list) {
            removeFromBackF(list);
            saveFListsToFile(filename, lists, *listCount);
            cout << "Успешно!" << endl;
        } else {
            cout << "Список не найден" << endl;
        }
    }
    else if (sscanf(command, "FREMOVE %255s \"%511[^\"]\"", listName, value) == 2 ||
             sscanf(command, "FREMOVE %255s %511s", listName, value) == 2) {
        FLinkedList* list = findFListByName(lists, listCount, listName);
        if (list) {
            removeByValueF(list, value);
            saveFListsToFile(filename, lists, *listCount);
            cout << "Успешно!" << endl;
        } else {
            cout << "Список не найден" << endl;
        }
    }
    // Обработка команды FREMOVE_BEFORE - удаление элемента перед указанным значением
    else if (sscanf(command, "FREMOVE_BEFORE %255s \"%511[^\"]\"", listName, value) == 2 ||
             sscanf(command, "FREMOVE_BEFORE %255s %511s", listName, value) == 2) {
        FLinkedList* list = findFListByName(lists, listCount, listName);
        if (list) {
            removeBeforeF(list, value);
            saveFListsToFile(filename, lists, *listCount);
        } else {
            cout << "Список не найден" << endl;
        }
    }
    // Обработка команды FREMOVE_AFTER - удаление элемента после указанного значения
    else if (sscanf(command, "FREMOVE_AFTER %255s \"%511[^\"]\"", listName, value) == 2 ||
             sscanf(command, "FREMOVE_AFTER %255s %511s", listName, value) == 2) {
        FLinkedList* list = findFListByName(lists, listCount, listName);
        if (list) {
            removeAfterF(list, value);
            saveFListsToFile(filename, lists, *listCount);
        } else {
            cout << "Список не найден" << endl;
        }
    }
    else if (sscanf(command, "FFIND %255s \"%511[^\"]\"", listName, value) == 2 ||
             sscanf(command, "FFIND %255s %511s", listName, value) == 2) {
        FLinkedList* list = findFListByName(lists, listCount, listName);
        if (list) {
            FListNode* current = list->head;
            int index = 0;
            bool found = false;
            
            while (current != nullptr) {
                if (strcmp(current->data, value) == 0) {
                    cout << "Найдено на позиции: " << index << endl;
                    found = true;
                    break;
                }
                current = current->next;
                index++;
            }
            
            if (!found) {
                cout << "Значение '" << value << "' не найдено" << endl;
            }
        } else {
            cout << "Список не найден" << endl;
        }
    } 
    else if (sscanf(command, "FSIZE %255s", listName) == 1) {
        FLinkedList* list = findFListByName(lists, listCount, listName);
        if (list) {
            cout << list->size << endl;
        } else {
            cout << "Список не найден" << endl;
        }
    }
    else {
        cout << "Неизвестная команда списка" << endl;
    }
}