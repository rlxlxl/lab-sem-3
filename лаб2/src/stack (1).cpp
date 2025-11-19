#include "stack.h"
#include <iostream>
#include <cstdlib>
#include <cstring>
#include <cstdio>
using namespace std;

#define MAX_QUERY_LENGTH 1024

// Инициализация стека
void initStack(Stack* stack, const char* name) {
    stack->top = nullptr;      // Вершина стека изначально пуста
    stack->size = 0;           // Начальный размер стека
    strcpy(stack->name, name); // Сохраняем имя стека
}

// Освобождение памяти стека
void freeStack(Stack* stack) {
    StackNode* current = stack->top;
    while (current != nullptr) {
        StackNode* next = current->next;  // Сохраняем указатель на следующий элемент
        free(current->data);              // Освобождаем данные
        free(current);                    // Освобождаем узел
        current = next;                   // Переходим к следующему элементу
    }
    stack->top = nullptr;  // Обнуляем вершину
    stack->size = 0;       // Сбрасываем размер
}

// Добавление элемента в стек
void stackPush(Stack* stack, const char* value) {
    StackNode* newNode = (StackNode*)malloc(sizeof(StackNode));
    newNode->data = (char*)malloc(strlen(value) + 1);  // Память для строки
    strcpy(newNode->data, value);      // Копируем значение
    newNode->next = stack->top;        // Новый элемент указывает на старую вершину
    stack->top = newNode;              // Обновляем вершину стека
    stack->size++;                     // Увеличиваем счетчик элементов
}

// Удаление элемента из стека
char stackPop(Stack* stack) {
    if (stack->top == nullptr) {
        cout << "Стек пуст" << endl;
        return '\0';
    }
    
    StackNode* temp = stack->top;      // Сохраняем текущую вершину
    char ret = temp->data[0];
    stack->top = stack->top->next;     // Перемещаем вершину на следующий элемент
    free(temp->data);                  // Освобождаем данные
    free(temp);                        // Освобождаем узел
    stack->size--;                    // Уменьшаем счетчик элементов
    return ret;
}

// Просмотр верхнего элемента без удаления
const char* stackPeek(Stack* stack) {
    if (stack->top == nullptr) {
        return nullptr;  // Стек пуст
    }
    return stack->top->data;  // Возвращаем данные вершины
}

// Получение размера стека
int getStackSize(Stack* stack) {
    return stack->size;
}

// Поиск стека по имени
Stack* findStackByName(Stack* stacks, int* stackCount, const char* name) {
    for (int i = 0; i < *stackCount; i++) {
        if (strcmp(stacks[i].name, name) == 0) {
            return &stacks[i];  // Возвращаем найденный стек
        }
    }
    return nullptr;  // Стек не найден
}

// Создание нового стека
Stack* createStack(Stack* stacks, int* stackCount, const char* name) {
    Stack* newStack = &stacks[*stackCount];  // Берем следующий свободный слот
    initStack(newStack, name);               // Инициализируем стек
    (*stackCount)++;                         // Увеличиваем счетчик стеков
    return newStack;                         // Возвращаем новый стек
}

// Сохранение стеков в файл
void saveStacksToFile(const char* filename, Stack* stacks, int stackCount) {
    FILE* file = fopen(filename, "w");
    if (!file) {
        cout << "Ошибка открытия файла для записи" << endl;
        return;
    }
    
    for (int i = 0; i < stackCount; i++) {
        fprintf(file, "STACK %s\n", stacks[i].name);  // Запись имени стека
        
        // Запись элементов стека (сверху вниз)
        StackNode* current = stacks[i].top;
        while (current != nullptr) {
            fprintf(file, "  SDATA %s\n", current->data);  // Запись данных элемента
            current = current->next;
        }
        fprintf(file, "END_STACK\n");  // Маркер конца стека
    }
    
    fclose(file);
}

// Загрузка стеков из файла
void loadStacksFromFile(const char* filename, Stack* stacks, int* stackCount) {
    FILE* file = fopen(filename, "r");
    if (!file) return;
    
    char line[MAX_QUERY_LENGTH];
    Stack* currentStack = nullptr;
    char** tempData = nullptr;     // Временное хранилище данных
    int tempSize = 0;              // Количество временных данных
    int tempCapacity = 10;         // Вместимость временного хранилища
    
    tempData = (char**)malloc(tempCapacity * sizeof(char*));
    
    while (fgets(line, sizeof(line), file)) {
        line[strcspn(line, "\n")] = 0;  // Удаление символа новой строки
        
        if (strncmp(line, "STACK ", 6) == 0) {
            // Обработка предыдущего стека если есть
            if (currentStack && tempSize > 0) {
                // Восстанавливаем стек в правильном порядке (LIFO)
                for (int i = tempSize - 1; i >= 0; i--) {
                    stackPush(currentStack, tempData[i]);  // Добавляем в обратном порядке
                    free(tempData[i]);                     // Освобождаем временные данные
                }
                tempSize = 0;  // Сбрасываем счетчик
            }
            
            // Создание нового стека
            char stackName[256];
            sscanf(line + 6, "%255s", stackName);
            currentStack = findStackByName(stacks, stackCount, stackName);
            if (!currentStack) {
                currentStack = createStack(stacks, stackCount, stackName);
            }
        } 
        else if (strncmp(line, "  SDATA ", 8) == 0 && currentStack) {
            // Сохранение данных во временное хранилище
            if (tempSize >= tempCapacity) {
                tempCapacity *= 2;  // Удваиваем вместимость
                tempData = (char**)realloc(tempData, tempCapacity * sizeof(char*));
            }
            tempData[tempSize] = (char*)malloc(strlen(line + 8) + 1);
            strcpy(tempData[tempSize], line + 8);
            tempSize++;
        }
        else if (strcmp(line, "END_STACK") == 0) {
            // Завершение текущего стека
            if (currentStack && tempSize > 0) {
                for (int i = tempSize - 1; i >= 0; i--) {
                    stackPush(currentStack, tempData[i]);
                    free(tempData[i]);
                }
                tempSize = 0;
            }
            currentStack = nullptr;  // Сброс текущего стека
        }
    }
    
    // Обработка последнего стека если нужно
    if (currentStack && tempSize > 0) {
        for (int i = tempSize - 1; i >= 0; i--) {
            stackPush(currentStack, tempData[i]);
            free(tempData[i]);
        }
    }
    
    free(tempData);  // Освобождение временного хранилища
    fclose(file);
}

// Обработка команд для стеков
void executeStackCommand(Stack* stacks, int* stackCount, const char* command, const char* filename) {
    char cmd[50], stackName[256], value[512];
    
    // SPUSH - добавление элемента в стек
    if (sscanf(command, "SPUSH %255s %511[^\n]", stackName, value) == 2) {
        Stack* stack = findStackByName(stacks, stackCount, stackName);
        if (!stack) {
            stack = createStack(stacks, stackCount, stackName);
        }
        stackPush(stack, value);
        saveStacksToFile(filename, stacks, *stackCount);
        cout << "Успешно!" << endl;
    }
    // SPOP - удаление элемента из стека
    else if (sscanf(command, "SPOP %255s", stackName) == 1) {
        Stack* stack = findStackByName(stacks, stackCount, stackName);
        if (stack) {
            stackPop(stack);
            saveStacksToFile(filename, stacks, *stackCount);
            cout << "Успешно!" << endl;
        } else {
            cout << "Стек не найден" << endl;
        }
    }
    // SPEEK - просмотр верхнего элемента
    else if (sscanf(command, "SPEEK %255s", stackName) == 1) {
        Stack* stack = findStackByName(stacks, stackCount, stackName);
        if (stack) {
            const char* result = stackPeek(stack);
            if (result) {
                cout << result << endl;
            } else {
                cout << "Стек пуст" << endl;
            }
        } else {
            cout << "Стек не найден" << endl;
        }
    }
    // SSIZE - получение размера стека
    else if (sscanf(command, "SSIZE %255s", stackName) == 1) {
        Stack* stack = findStackByName(stacks, stackCount, stackName);
        if (stack) {
            cout << stack->size << endl;
        } else {
            cout << "Стек не найден" << endl;
        }
    }
    else {
        cout << "Неизвестная команда стека" << endl;
    }
}