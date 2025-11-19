#include "list.h"
#include <iostream>
#include <cstdlib>
#include <cstring>
#include <cstdio>
using namespace std;

#define MAX_QUERY_LENGTH 1024  // Максимальная длина команды

// Инициализация двусвязного списка
void initList(LinkedList* list, const char* name) {
    list->head = nullptr;      
    list->tail = nullptr;      
    list->size = 0;            
    strcpy(list->name, name);  // Копируем имя списка
}

// Освобождение памяти, занятой списком
void freeList(LinkedList* list) {
    ListNode* current = list->head;
    // Проходим по всем узлам списка
    while (current != nullptr) {
        ListNode* next = current->next;  
        free(current->data);             
        free(current);                   
        current = next;                 
    }
    // Обнуляем указатели и размер
    list->head = nullptr;
    list->tail = nullptr;
    list->size = 0;
}

// Добавление элемента в начало списка
void addToFront(LinkedList* list, const char* value) {
    // Создаем новый узел
    ListNode* newNode = (ListNode*)malloc(sizeof(ListNode));
    newNode->data = (char*)malloc(strlen(value) + 1);  // Память для строки + терминатор
    strcpy(newNode->data, value);      // Копируем значение
    newNode->prev = nullptr;           // Предыдущий элемент для первого - nullptr
    newNode->next = list->head;        // Следующий элемент - бывший первый
    
    // Если список не пуст, обновляем prev бывшего первого элемента
    if (list->head != nullptr) {
        list->head->prev = newNode;
    }
    list->head = newNode;  // Новый узел становится первым
    
    // Если список был пуст, tail тоже указывает на новый узел
    if (list->tail == nullptr) {
        list->tail = newNode;
    }
    list->size++;  // Увеличиваем счетчик элементов
}

// Добавление элемента в конец списка
void addToBack(LinkedList* list, const char* value) {
    // Создаем новый узел
    ListNode* newNode = (ListNode*)malloc(sizeof(ListNode));
    newNode->data = (char*)malloc(strlen(value) + 1);
    strcpy(newNode->data, value);
    newNode->next = nullptr;           // Следующий элемент для последнего - nullptr
    newNode->prev = list->tail;        // Предыдущий элемент - бывший последний
    
    // Если список не пуст, обновляем next бывшего последнего элемента
    if (list->tail != nullptr) {
        list->tail->next = newNode;
    }
    list->tail = newNode;  // Новый узел становится последним
    
    // Если список был пуст, head тоже указывает на новый узел
    if (list->head == nullptr) {
        list->head = newNode;
    }
    list->size++;  // Увеличиваем счетчик элементов
}

// Вставка элемента после указанного значения
void insertAfter(LinkedList* list, const char* afterValue, const char* newValue) {
    ListNode* current = list->head;
    // Ищем узел с заданным значением
    while (current != nullptr) {
        if (strcmp(current->data, afterValue) == 0) {
            // Создаем новый узел
            ListNode* newNode = (ListNode*)malloc(sizeof(ListNode));
            newNode->data = (char*)malloc(strlen(newValue) + 1);
            strcpy(newNode->data, newValue);
            newNode->prev = current;           // Предыдущий - найденный узел
            newNode->next = current->next;     // Следующий - тот, что был после найденного
            
            // Если вставляем не в конец, обновляем prev следующего узла
            if (current->next != nullptr) {
                current->next->prev = newNode;
            }
            current->next = newNode;  // Найденный узел теперь указывает на новый
            
            // Если вставили после последнего элемента, обновляем tail
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

// Вставка элемента перед указанным значением
void insertBefore(LinkedList* list, const char* beforeValue, const char* newValue) {
    ListNode* current = list->head;
    // Ищем узел с заданным значением
    while (current != nullptr) {
        if (strcmp(current->data, beforeValue) == 0) {
            // Создаем новый узел
            ListNode* newNode = (ListNode*)malloc(sizeof(ListNode));
            newNode->data = (char*)malloc(strlen(newValue) + 1);
            strcpy(newNode->data, newValue);
            newNode->prev = current->prev;  // Предыдущий - тот, что был перед найденным
            newNode->next = current;        // Следующий - найденный узел
            
            // Если вставляем не в начало, обновляем next предыдущего узла
            if (current->prev != nullptr) {
                current->prev->next = newNode;
            } else {
                // Вставляем в начало - обновляем head
                list->head = newNode;
            }
            current->prev = newNode;  // Найденный узел теперь указывает назад на новый
            list->size++;
            return;
        }
        current = current->next;
    }
    cout << "Значение '" << beforeValue << "' не найдено" << endl;
}

// Удаление элемента из начала списка
void removeFromFront(LinkedList* list) {
    if (list->head == nullptr) {
        cout << "Список пуст" << endl;
        return;
    }
    
    ListNode* temp = list->head;        // Сохраняем указатель на удаляемый узел
    list->head = list->head->next;      // Сдвигаем head на следующий элемент
    
    // Если после удаления список не пуст, обнуляем prev нового head
    if (list->head != nullptr) {
        list->head->prev = nullptr;
    } else {
        // Если список стал пуст, обнуляем tail
        list->tail = nullptr;
    }
    
    free(temp->data);  // Освобождаем память строки
    free(temp);        // Освобождаем память узла
    list->size--;      // Уменьшаем счетчик элементов
}

// Удаление элемента из конца списка
void removeFromBack(LinkedList* list) {
    if (list->tail == nullptr) {
        cout << "Список пуст" << endl;
        return;
    }
    
    ListNode* temp = list->tail;        // Сохраняем указатель на удаляемый узел
    list->tail = list->tail->prev;      // Сдвигаем tail на предыдущий элемент
    
    // Если после удаления список не пуст, обнуляем next нового tail
    if (list->tail != nullptr) {
        list->tail->next = nullptr;
    } else {
        // Если список стал пуст, обнуляем head
        list->head = nullptr;
    }
    
    free(temp->data);  // Освобождаем память строки
    free(temp);        // Освобождаем память узла
    list->size--;      // Уменьшаем счетчик элементов
}

// Удаление элемента по значению
void removeByValue(LinkedList* list, const char* value) {
    ListNode* current = list->head;
    // Ищем узел с заданным значением
    while (current != nullptr) {
        if (strcmp(current->data, value) == 0) {
            // Обновляем связи соседних узлов
            if (current->prev != nullptr) {
                current->prev->next = current->next;  // Предыдущий указывает на следующий
            } else {
                list->head = current->next;  // Удаляем первый элемент
            }
            
            if (current->next != nullptr) {
                current->next->prev = current->prev;  // Следующий указывает на предыдущий
            } else {
                list->tail = current->prev;  // Удаляем последний элемент
            }
            
            free(current->data);  // Освобождаем память строки
            free(current);        // Освобождаем память узла
            list->size--;         // Уменьшаем счетчик элементов
            return;
        }
        current = current->next;
    }
    cout << "Значение '" << value << "' не найдено" << endl;
}

// Удаление элемента перед указанным значением
void removeBefore(LinkedList* list, const char* beforeValue) {
    if (list->head == nullptr) {
        cout << "Список пуст" << endl;
        return;
    }
    
    ListNode* current = list->head;
    // Ищем узел с заданным значением
    while (current != nullptr) {
        if (strcmp(current->data, beforeValue) == 0) {
            // Проверяем, есть ли элемент перед текущим
            if (current->prev == nullptr) {
                cout << "Нет элемента перед '" << beforeValue << "'" << endl;
                return;
            }
            
            ListNode* toRemove = current->prev;  // Узел для удаления
            
            // Обновляем связи
            current->prev = toRemove->prev;
            if (toRemove->prev != nullptr) {
                toRemove->prev->next = current;
            } else {
                // Удаляем первый элемент - обновляем head
                list->head = current;
            }
            
            free(toRemove->data);  // Освобождаем память строки
            free(toRemove);        // Освобождаем память узла
            list->size--;          // Уменьшаем счетчик элементов
            cout << "Успешно!" << endl;
            return;
        }
        current = current->next;
    }
    cout << "Значение '" << beforeValue << "' не найдено" << endl;
}

// Удаление элемента после указанного значения
void removeAfter(LinkedList* list, const char* afterValue) {
    if (list->head == nullptr) {
        cout << "Список пуст" << endl;
        return;
    }
    
    ListNode* current = list->head;
    // Ищем узел с заданным значением
    while (current != nullptr) {
        if (strcmp(current->data, afterValue) == 0) {
            // Проверяем, есть ли элемент после текущего
            if (current->next == nullptr) {
                cout << "Нет элемента после '" << afterValue << "'" << endl;
                return;
            }
            
            ListNode* toRemove = current->next;  // Узел для удаления
            
            // Обновляем связи
            current->next = toRemove->next;
            if (toRemove->next != nullptr) {
                toRemove->next->prev = current;
            } else {
                // Удаляем последний элемент - обновляем tail
                list->tail = current;
            }
            
            free(toRemove->data);  // Освобождаем память строки
            free(toRemove);        // Освобождаем память узла
            list->size--;          // Уменьшаем счетчик элементов
            cout << "Успешно!" << endl;
            return;
        }
        current = current->next;
    }
    cout << "Значение '" << afterValue << "' не найдено" << endl;
}

// Поиск элемента по значению и возврат его индекса
int findByValue(LinkedList* list, const char* value) {
    ListNode* current = list->head;
    int index = 0;
    // Линейный поиск по списку
    while (current != nullptr) {
        if (strcmp(current->data, value) == 0) {
            return index;  // Возвращаем индекс найденного элемента
        }
        current = current->next;
        index++;
    }
    return -1;  // Элемент не найден
}

// Получение размера списка
int getListSize(LinkedList* list) {
    return list->size;
}

// Поиск списка по имени в массиве списков
LinkedList* findListByName(LinkedList* lists, int* listCount, const char* name) {
    for (int i = 0; i < *listCount; i++) {
        if (strcmp(lists[i].name, name) == 0) {
            return &lists[i];  // Возвращаем указатель на найденный список
        }
    }
    return nullptr;  // Список не найден
}

// Создание нового списка с заданным именем
LinkedList* createList(LinkedList* lists, int* listCount, const char* name) {
    LinkedList* newList = &lists[*listCount];  // Берем следующий свободный слот
    initList(newList, name);                   // Инициализируем список
    (*listCount)++;                            // Увеличиваем счетчик списков
    return newList;                            // Возвращаем указатель на новый список
}

// Сохранение всех списков в файл
void saveListsToFile(const char* filename, LinkedList* lists, int listCount) {
    FILE* file = fopen(filename, "w");  // Открываем файл для записи
    if (!file) {
        cout << "Ошибка открытия файла для записи" << endl;
        return;
    }
    
    // Для каждого списка записываем его имя и все элементы
    for (int i = 0; i < listCount; i++) {
        fprintf(file, "LIST %s\n", lists[i].name);  // Запись имени списка
        
        // Запись всех элементов списка
        ListNode* current = lists[i].head;
        while (current != nullptr) {
            fprintf(file, "  LDATA %s\n", current->data);  // Запись данных элемента
            current = current->next;
        }
        fprintf(file, "END_LIST\n");  // Маркер конца списка
    }
    
    fclose(file);  // Закрываем файл
}

// Загрузка списков из файла
void loadListsFromFile(const char* filename, LinkedList* lists, int* listCount) {
    FILE* file = fopen(filename, "r");  // Открываем файл для чтения
    if (!file) {
        return;  // Файл не существует - выходим
    }
    
    char line[MAX_QUERY_LENGTH];
    LinkedList* currentList = nullptr;  // Текущий обрабатываемый список
    
    // Читаем файл построчно
    while (fgets(line, sizeof(line), file)) {
        line[strcspn(line, "\n")] = 0;  // Удаляем символ новой строки
        
        if (strncmp(line, "LIST ", 5) == 0) {
            // Найдена секция нового списка
            char listName[256];
            sscanf(line + 5, "%255s", listName);  // Извлекаем имя списка
            // Ищем или создаем список
            currentList = findListByName(lists, listCount, listName);
            if (!currentList) {
                currentList = createList(lists, listCount, listName);
            }
        } else if (strncmp(line, "  LDATA ", 8) == 0 && currentList) {
            // Найдены данные элемента - добавляем в конец текущего списка
            addToBack(currentList, line + 8);
        } else if (strcmp(line, "END_LIST") == 0) {
            // Конец текущего списка
            currentList = nullptr;
        }
    }
    
    fclose(file);  // Закрываем файл
}

// Обработка команд для двусвязных списков
// Обработка команд для двусвязных списков
// Обработка команд для двусвязных списков
void executeListCommand(LinkedList* lists, int* listCount, const char* command, const char* filename) {
    char cmd[50], listName[256], value[512], value2[512];
    
    // Обработка команды LPUSH_FRONT - добавление в начало
    if (sscanf(command, "LPUSH_FRONT %255s \"%511[^\"]\"", listName, value) == 2) {
        LinkedList* list = findListByName(lists, listCount, listName);
        if (!list) {
            list = createList(lists, listCount, listName);
        }
        addToFront(list, value);
        saveListsToFile(filename, lists, *listCount);
        cout << "Успешно!" << endl;
    }
    // Обработка команды LPUSH_BACK - добавление в конец
    else if (sscanf(command, "LPUSH_BACK %255s \"%511[^\"]\"", listName, value) == 2) {
        LinkedList* list = findListByName(lists, listCount, listName);
        if (!list) {
            list = createList(lists, listCount, listName);
        }
        addToBack(list, value);
        saveListsToFile(filename, lists, *listCount);
        cout << "Успешно!" << endl;
    }
    // Обработка команды LINSERT_AFTER - вставка после элемента
    else if (sscanf(command, "LINSERT_AFTER %255s \"%511[^\"]\" \"%511[^\"]\"", listName, value, value2) == 3) {
        LinkedList* list = findListByName(lists, listCount, listName);
        if (list) {
            insertAfter(list, value, value2);
            saveListsToFile(filename, lists, *listCount);
            cout << "Успешно!" << endl;
        } else {
            cout << "Список '" << listName << "' не найден" << endl;
        }
    }
    // Обработка команды LINSERT_BEFORE - вставка перед элементом
    else if (sscanf(command, "LINSERT_BEFORE %255s \"%511[^\"]\" \"%511[^\"]\"", listName, value, value2) == 3) {
        LinkedList* list = findListByName(lists, listCount, listName);
        if (list) {
            insertBefore(list, value, value2);
            saveListsToFile(filename, lists, *listCount);
            cout << "Успешно!" << endl;
        } else {
            cout << "Список '" << listName << "' не найден" << endl;
        }
    }
    // Обработка команды LPOP_FRONT - удаление из начала
    else if (sscanf(command, "LPOP_FRONT %255s", listName) == 1) {
        LinkedList* list = findListByName(lists, listCount, listName);
        if (list) {
            removeFromFront(list);
            saveListsToFile(filename, lists, *listCount);
            cout << "Успешно!" << endl;
        } else {
            cout << "Список не найден" << endl;
        }
    }
    // Обработка команды LPOP_BACK - удаление из конца
    else if (sscanf(command, "LPOP_BACK %255s", listName) == 1) {
        LinkedList* list = findListByName(lists, listCount, listName);
        if (list) {
            removeFromBack(list);
            saveListsToFile(filename, lists, *listCount);
            cout << "Успешно!" << endl;
        } else {
            cout << "Список не найден" << endl;
        }
    }
    // Обработка команды LREMOVE - удаление по значению
    else if (sscanf(command, "LREMOVE %255s \"%511[^\"]\"", listName, value) == 2) {
        LinkedList* list = findListByName(lists, listCount, listName);
        if (list) {
            removeByValue(list, value);
            saveListsToFile(filename, lists, *listCount);
            cout << "Успешно!" << endl;
        } else {
            cout << "Список не найден" << endl;
        }
    }
    // Обработка команды LREMOVE_BEFORE - удаление элемента перед указанным значением
    else if (sscanf(command, "LREMOVE_BEFORE %255s \"%511[^\"]\"", listName, value) == 2) {
        LinkedList* list = findListByName(lists, listCount, listName);
        if (list) {
            removeBefore(list, value);
            saveListsToFile(filename, lists, *listCount);
        } else {
            cout << "Список не найден" << endl;
        }
    }
    // Обработка команды LREMOVE_AFTER - удаление элемента после указанного значения
    else if (sscanf(command, "LREMOVE_AFTER %255s \"%511[^\"]\"", listName, value) == 2) {
        LinkedList* list = findListByName(lists, listCount, listName);
        if (list) {
            removeAfter(list, value);
            saveListsToFile(filename, lists, *listCount);
        } else {
            cout << "Список не найден" << endl;
        }
    }
    // Обработка команды LFIND - поиск элемента
    else if (sscanf(command, "LFIND %255s \"%511[^\"]\"", listName, value) == 2) {
        LinkedList* list = findListByName(lists, listCount, listName);
        if (list) {
            int index = findByValue(list, value);
            if (index != -1) {
                cout << "Найдено на позиции: " << index << endl;
            } else {
                cout << "Значение '" << value << "' не найдено" << endl;
            }
        } else {
            cout << "Список не найден" << endl;
        }
    }
    // Обработка команды LSIZE - получение размера списка
    else if (sscanf(command, "LSIZE %255s", listName) == 1) {
        LinkedList* list = findListByName(lists, listCount, listName);
        if (list) {
            cout << list->size << endl;
        } else {
            cout << "Список не найден" << endl;
        }
    }
    // Неизвестная команда или ошибка формата
    else {
        cout << "Ошибка формата команды! Значения должны быть в кавычках." << endl;
        cout << "Пример: LREMOVE_AFTER LIST \"значение\"" << endl;
    }
}