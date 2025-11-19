#include <iostream>
#include <cstring>
#include "array.h"
#include "flist.h"
#include "list.h"
#include "stack.h"
#include "queue.h"
#include "tree.h"
#include "set.h"
using namespace std;

#define MAX_ARRAYS 100
#define MAX_LISTS 100
#define MAX_DLISTS 100
#define MAX_STACKS 100
#define MAX_QUEUES 100
#define MAX_TREES 100
#define MAX_SETS 100

// Функция для вывода подмассива
void printSubarray(const char* arr[], int size) {
    cout << "{";
    for (int i = 0; i < size; i++) {
        cout << arr[i];
        if (i < size - 1) {
            cout << ", ";
        }
    }
    cout << "}";
}

// Функция для вывода всех подмассивов массива
void printAllSubarrays(const char* arr[], int n) {
    int total = 1 << n; // 2^n
    int subarrayCount = 0;
    
    cout << "Все подмассивы массива (" << total << " всего):" << endl;
    cout << "==================================" << endl;
    
    for (int mask = 0; mask < total; mask++) {
        const char* currentSubarray[100];
        int currentSize = 0;
        
        // Формируем текущий подмассив на основе битовой маски
        for (int i = 0; i < n; i++) {
            if (mask & (1 << i)) {
                currentSubarray[currentSize++] = arr[i];
            }
        }
        
        // Выводим подмассив
        printSubarray(currentSubarray, currentSize);
        cout << endl;
        subarrayCount++;
    }
    
    cout << "==================================" << endl;
    cout << "Всего выведено подмассивов: " << subarrayCount << endl;
}

// Функция для выполнения команды MSUBS
void executeMSubsCommand(DynamicArray arrays[], int* arrayCount, const char* query) {
    char name[256];
    
    // Парсим команду: MSUBS <имя_массива>
    if (sscanf(query, "MSUBS %255s", name) == 1) {
        bool found = false;
        
        // Ищем массив по имени
        for (int i = 0; i < *arrayCount; i++) {
            if (arrays[i].size > 0 && strcmp(arrays[i].data[0], name) == 0) {
                found = true;
                int elementCount = arrays[i].size - 1;
                
                if (elementCount == 0) {
                    cout << "Массив '" << name << "' пуст" << endl;
                    cout << "Единственный подмассив: {}" << endl;
                } else {
                    // Создаем массив элементов (исключая имя массива)
                    const char** elements = new const char*[elementCount];
                    for (int j = 0; j < elementCount; j++) {
                        elements[j] = arrays[i].data[j + 1];
                    }
                    
                    printAllSubarrays(elements, elementCount);
                    delete[] elements;
                }
                break;
            }
        }
        
        if (!found) {
            cout << "Массив '" << name << "' не найден" << endl;
        }
    } else {
        cout << "Неверный формат команды MSUBS. Используйте: MSUBS <имя_массива>" << endl;
    }
}

int main(int argc, char* argv[]) {
    char* filename = nullptr; // Имя файла для сохранения/загрузки
    char* query = nullptr; // Команда для выполнения
    
    // Парсинг аргументов: --file <filename> --query '<command>'
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "--file") == 0 && i + 1 < argc) {
            filename = argv[++i]; // Следующий аргумент - имя файла
        } else if (strcmp(argv[i], "--query") == 0 && i + 1 < argc) {
            query = argv[++i]; // Следующий аргумент - команда
        }
    }
    
    // Проверка обязательных аргументов
    if (!filename || !query) {
        cout << "Правильный синтаксис" << argv[0] << " --file <файл> --query '<команда>'" << endl;
        return 1;
    }
    
    // Массивы структур для хранения всех контейнеров
    DynamicArray arrays[MAX_ARRAYS];
    int arrayCount = 0;
    FLinkedList flists[MAX_LISTS];
    int flistCount = 0;
    LinkedList dlists[MAX_DLISTS];
    int dlistCount = 0;
    Stack stacks[MAX_STACKS];
    int stackCount = 0;
    Queue queues[MAX_QUEUES];
    int queueCount = 0;
    Tree trees[MAX_TREES];
    int treeCount = 0;
    HashSet sets[MAX_SETS];
    int setCount = 0;
    
    //Загрузка данных из файла
    loadArraysFromFile(filename, arrays, &arrayCount);
    loadFListsFromFile(filename, flists, &flistCount);
    loadListsFromFile(filename, dlists, &dlistCount);
    loadStacksFromFile(filename, stacks, &stackCount);
    loadQueuesFromFile(filename, queues, &queueCount);
    loadTreesFromFile(filename, trees, &treeCount);
    loadSetsFromFile(filename, sets, &setCount);
    
    //Определение типа команды
    if (strncmp(query, "M", 1) == 0) {
        // Проверяем, не является ли это командой MSUBS
        if (strncmp(query, "MSUBS", 5) == 0) {
            executeMSubsCommand(arrays, &arrayCount, query);
        } else {
            executeArrayCommand(arrays, &arrayCount, query, filename);
        }
    }
    else if (strncmp(query, "F", 1) == 0) {
        executeFListCommand(flists, &flistCount, query, filename);
    }
    else if (strncmp(query, "L", 1) == 0) {
        executeListCommand(dlists, &dlistCount, query, filename);
    }
    else if (strncmp(query, "S", 1) == 0) {
        executeStackCommand(stacks, &stackCount, query, filename);
    }
    else if (strncmp(query, "Q", 1) == 0) {
        executeQueueCommand(queues, &queueCount, query, filename);
    }
    else if (strncmp(query, "T", 1) == 0) {
        executeTreeCommand(trees, &treeCount, query, filename);
    }
    else if (strncmp(query, "H", 1) == 0) {
        executeSetCommand(sets, &setCount, query, filename);
    }
    //strncmp сравнивает строку (первые пять символов с print)
    // возвращает 0 если совпадает
    else if (strncmp(query, "PRINT", 5) == 0) {
        char name[256];
        //Читает данные из строки query по формату
        //Читает строку максимум из 255 символов
        if (sscanf(query, "PRINT %255s", name) == 1) { //если успешно возвращает 1
            bool found = false;
            
            //ищем в массивах
            for (int i = 0; i < arrayCount; i++) {
                if (arrays[i].size > 0 && strcmp(arrays[i].data[0], name) == 0) {
                    cout << "Массив: " << name << endl;
                    cout << "Размер: " << arrays[i].size - 1 << endl;
                    cout << "Элементы:" << endl;
                    
                    if (arrays[i].size == 1) {
                        cout << "  [пусто]" << endl;
                    } else {
                        for (int j = 1; j < arrays[i].size; j++) {
                            cout << "  [" << j - 1 << "] " << arrays[i].data[j] << endl;
                        }
                    }
                    cout << "--------------------" << endl;
                    found = true;
                    break;
                }
            }
            
            //ищем в фсписках
            if (!found) {
                for (int i = 0; i < flistCount; i++) {
                    if (strcmp(flists[i].name, name) == 0) {
                        cout << "Односвязный список: " << name << endl;
                        cout << "Размер: " << flists[i].size << endl;
                        cout << "Элементы:" << endl;
                        
                        if (flists[i].head == nullptr) {
                            cout << "  [пусто]" << endl;
                        } else {
                            FListNode* current = flists[i].head;
                            int index = 0;
                            while (current != nullptr) {
                                cout << "  [" << index << "] " << current->data << endl;
                                current = current->next;
                                index++;
                            }
                        }
                        cout << "--------------------" << endl;
                        found = true;
                        break;
                    }
                }
            }
            
            //ищем в списках
            if (!found) {
                for (int i = 0; i < dlistCount; i++) {
                    if (strcmp(dlists[i].name, name) == 0) {
                        cout << "Двусвязный список: " << name << endl;
                        cout << "Размер: " << dlists[i].size << endl;
                        cout << "Элементы:" << endl;
                        
                        if (dlists[i].head == nullptr) {
                            cout << "  [пусто]" << endl;
                        } else {
                            ListNode* current = dlists[i].head;
                            int index = 0;
                            while (current != nullptr) {
                                cout << "  [" << index << "] " << current->data << endl;
                                current = current->next;
                                index++;
                            }
                        }
                        cout << "--------------------" << endl;
                        found = true;
                        break;
                    }
                }
            }
            
            //ищем в стеках
            if (!found) {
                for (int i = 0; i < stackCount; i++) {
                    if (strcmp(stacks[i].name, name) == 0) {
                        cout << "Стек: " << name << endl;
                        cout << "Размер: " << stacks[i].size << endl;
                        cout << "Элементы (сверху вниз):" << endl;
                        
                        if (stacks[i].top == nullptr) {
                            cout << "  [пусто]" << endl;
                        } else {
                            StackNode* current = stacks[i].top;
                            int index = 0;
                            while (current != nullptr) {
                                cout << "  [" << index << "] " << current->data << endl;
                                current = current->next;
                                index++;
                            }
                        }
                        cout << "--------------------" << endl;
                        found = true;
                        break;
                    }
                }
            }
            
            //ищем в очередях
            if (!found) {
                for (int i = 0; i < queueCount; i++) {
                    if (strcmp(queues[i].name, name) == 0) {
                        cout << "Очередь: " << name << endl;
                        cout << "Размер: " << queues[i].size << endl;
                        cout << "Элементы (от начала к концу):" << endl;
                        
                        if (queues[i].front == nullptr) {
                            cout << "  [пусто]" << endl;
                        } else {
                            QueueNode* current = queues[i].front;
                            int index = 0;
                            while (current != nullptr) {
                                cout << "  [" << index << "] " << current->data << endl;
                                current = current->next;
                                index++;
                            }
                        }
                        cout << "--------------------" << endl;
                        found = true;
                        break;
                    }
                }
            }
            //вывод дерева
     if (!found) {
    for (int i = 0; i < treeCount; i++) {
        if (strcmp(trees[i].name, name) == 0) {
            cout << "Бинарное дерево: " << name << endl;
            cout << "Размер: " << trees[i].size << endl;
            cout << "Высота: " << treeHeight(trees[i].root) << endl;

            cout << "Pre-order:" << endl; // Прямой обход (корень-левый-правый)
            if (trees[i].root == nullptr) {
                cout << "  [пусто]" << endl;
            } else {
                printPreOrder(trees[i].root);
                cout << endl;
            }
            
            cout << "In-order:" << endl; // Симметричный обход (левый-корень-правый)
            if (trees[i].root == nullptr) {
                cout << "  [пусто]" << endl;
            } else {
                printInOrder(trees[i].root);
                cout << endl;
            }
            
            cout << "Post-order:" << endl; // Обратный обход (левый-правый-корень)
            if (trees[i].root == nullptr) {
                cout << "  [пусто]" << endl;
            } else {
                printPostOrder(trees[i].root);
                cout << endl;
            }
            
            cout << "Level-order:" << endl; // Обход в ширину (по уровням)
            if (trees[i].root == nullptr) {
                cout << "  [пусто]" << endl;
            } else {
                printLevelOrder(trees[i].root);
                cout << endl;
            }
            
            cout << "--------------------" << endl;
            found = true;
            break;
        }
    }
    if (!found) { //Ищем по имени структуры
    for (int i = 0; i < setCount; i++) {
        if (strcmp(sets[i].name, name) == 0) {
            cout << "Множество: " << name << endl;
            cout << "Размер: " << sets[i].size << endl;
            cout << "Элементы:" << endl;
            
            if (sets[i].size == 0) {
                cout << "  [пусто]" << endl;
            } else {
                int elementCount = 0;
                for (int j = 0; j < sets[i].capacity; j++) {
                    SetNode* current = sets[i].buckets[j];
                    while (current != nullptr) {
                        cout << "  [" << elementCount << "] " << current->key << endl;
                        current = current->next;
                        elementCount++;
                    }
                }
            }
            cout << "--------------------" << endl;
            found = true;
            break;
        }
    }
}

}       
            if (!found) {
                cout << "Структура '" << name << "' не найдена" << endl;
            }
        } else {
            cout << "Неверный формат команды PRINT" << endl;
        }
    }
    else {
        cout << "Неизвестный тип команды" << endl;
    }
    
    //Очищаем память
    for (int i = 0; i < arrayCount; i++) {
        freeArray(&arrays[i]);
    }
    for (int i = 0; i < flistCount; i++) {
        freeFList(&flists[i]);
    }
    for (int i = 0; i < dlistCount; i++) {
        freeList(&dlists[i]);
    }
    for (int i = 0; i < stackCount; i++) {
        freeStack(&stacks[i]);
    }
    for (int i = 0; i < queueCount; i++) {
        freeQueue(&queues[i]);
    }
    for (int i = 0; i < treeCount; i++) {
        freeTree(&trees[i]);
    }
    for (int i = 0; i < setCount; i++) {
        freeSet(&sets[i]);
}
    
    return 0;
}