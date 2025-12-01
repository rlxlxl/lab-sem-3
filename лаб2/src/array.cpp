#include "array.h"
#include <iostream>
#include <cstdlib>
#include <cstring>
#include <cstdio>
using namespace std;

#define MAX_QUERY_LENGTH 1024

// Инициализация динамического массива
void initArray(DynamicArray* arr, int initialCapacity) {
    arr->data = (char**)malloc(initialCapacity * sizeof(char*));
    arr->capacity = initialCapacity;
    arr->size = 0;
}

// Освобождение памяти массива
void freeArray(DynamicArray* arr) {
    for (int i = 0; i < arr->size; i++) {
        free(arr->data[i]);  // Освобождаем каждую строку
    }
    free(arr->data);         // Освобождаем массив указателей
    arr->data = nullptr;
    arr->capacity = 0;
    arr->size = 0;
}

// Увеличение размера массива при переполнении
void resizeArray(DynamicArray* arr) {
    int newCapacity = arr->capacity * 2;  // Удваиваем вместимость
    char** newData = (char**)malloc(newCapacity * sizeof(char*));
    
    // Копируем старые данные
    for (int i = 0; i < arr->size; i++) {
        newData[i] = arr->data[i];
    }
    
    free(arr->data);
    arr->data = newData;
    arr->capacity = newCapacity;
}

// Добавление элемента в конец
void pushBack(DynamicArray* arr, const char* value) {
    if (arr->size >= arr->capacity) {
        resizeArray(arr);  // Автоматическое увеличение при необходимости
    }
    
    arr->data[arr->size] = (char*)malloc(strlen(value) + 1);
    strcpy(arr->data[arr->size], value);
    arr->size++;
}

// Вставка элемента по индексу
void insertAt(DynamicArray* arr, int index, const char* value) {
    if (index < 0 || index > arr->size) return;
    
    if (arr->size >= arr->capacity) {
        resizeArray(arr);
    }
    
    // Сдвиг элементов вправо для освобождения места
    for (int i = arr->size; i > index; i--) {
        arr->data[i] = arr->data[i - 1];
    }
    
    arr->data[index] = (char*)malloc(strlen(value) + 1);
    strcpy(arr->data[index], value);
    arr->size++;
}

// Получение элемента по индексу
const char* getAt(const DynamicArray* arr, int index) {
    if (index < 0 || index >= arr->size) {
        return nullptr;
    }
    return arr->data[index];
}

// Удаление элемента по индексу
void removeAt(DynamicArray* arr, int index) {
    if (index < 0 || index >= arr->size) return;
    
    free(arr->data[index]);  // Освобождаем память строки
    
    // Сдвиг элементов влево для заполнения пустоты
    for (int i = index; i < arr->size - 1; i++) {
        arr->data[i] = arr->data[i + 1];
    }
    
    arr->size--;
}

// Замена элемента по индексу
void replaceAt(DynamicArray* arr, int index, const char* value) {
    if (index < 0 || index >= arr->size) return;
    
    free(arr->data[index]);  // Освобождаем старую строку
    arr->data[index] = (char*)malloc(strlen(value) + 1);
    strcpy(arr->data[index], value);
}

// Получение текущего размера массива
int getSize(const DynamicArray* arr) {
    return arr->size;
}

// Поиск массива по имени
DynamicArray* findArrayByName(DynamicArray* arrays, int* arrayCount, const char* name) {
    for (int i = 0; i < *arrayCount; i++) {
        // Первый элемент массива хранит его имя
        if (arrays[i].size > 0 && strcmp(arrays[i].data[0], name) == 0) {
            return &arrays[i];
        }
    }
    return nullptr;
}

// Создание нового массива
DynamicArray* createArray(DynamicArray* arrays, int* arrayCount, const char* name) {
    DynamicArray* newArray = &arrays[*arrayCount];
    initArray(newArray);
    pushBack(newArray, name);  // Первый элемент - имя массива
    (*arrayCount)++;
    return newArray;
}

// Сохранение массивов в файл
void saveArraysToFile(const char* filename, DynamicArray* arrays, int arrayCount) {
    FILE* file = fopen(filename, "w");
    if (!file) {
        cout << "Не удалось открыть файл" << endl;
        return;
    }
    
    for (int i = 0; i < arrayCount; i++) {
        if (arrays[i].size > 0) {
            fprintf(file, "ARRAY %s\n", arrays[i].data[0]);  // Имя массива
            
            // Данные массива (начиная с индекса 1)
            for (int j = 1; j < arrays[i].size; j++) {
                fprintf(file, "  DATA %s\n", arrays[i].data[j]);
            }
            fprintf(file, "END_ARRAY\n");
        }
    }
    
    fclose(file);
}

// Загрузка массивов из файла
void loadArraysFromFile(const char* filename, DynamicArray* arrays, int* arrayCount) {
    FILE* file = fopen(filename, "r");
    if (!file) return;
    
    char line[MAX_QUERY_LENGTH];
    DynamicArray* currentArray = nullptr;
    
    while (fgets(line, sizeof(line), file)) {
        line[strcspn(line, "\n")] = 0;  // Удаление символа новой строки
        
        if (strncmp(line, "ARRAY ", 6) == 0) {
            char arrayName[256];
            sscanf(line + 6, "%255s", arrayName);
            currentArray = createArray(arrays, arrayCount, arrayName);
        } 
        else if (strncmp(line, "  DATA ", 7) == 0 && currentArray) {
            pushBack(currentArray, line + 7);  // Добавляем данные в текущий массив
        }
        else if (strcmp(line, "END_ARRAY") == 0) {
            currentArray = nullptr;  // Завершение текущего массива
        }
    }
    
    fclose(file);
}

// Вывод содержимого массива
void printArray(DynamicArray* arrays, int* arrayCount, const char* arrayName) {
    DynamicArray* arr = findArrayByName(arrays, arrayCount, arrayName);
    if (!arr) {
        cout << "Массив '" << arrayName << "' не найден" << endl;
        return;
    }
    
    cout << "Массив: " << arrayName << endl;
    cout << "Размер: " << arr->size - 1 << endl;  // -1 потому что первый элемент - имя
    cout << "Элементы:" << endl;
    
    if (arr->size == 1) {
        cout << "  [пусто]" << endl;
    } else {
        for (int i = 1; i < arr->size; i++) {
            cout << "  [" << i - 1 << "] " << arr->data[i] << endl;
        }
    }
    cout << "--------------------" << endl;
}

#include "set.h"   // используем твой HashSet

void printAllSubarraysPowerset(DynamicArray* arr) {
    int n = arr->size - 1; // минус имя массива
    if (n <= 0) {
        cout << "{}" << endl;
        return;
    }

    int total = 1 << n;

    cout << "Все подмассивы:" << endl;

    for (int mask = 0; mask < total; mask++) {
        cout << "{";

        bool first = true;
        for (int i = 0; i < n; i++) {
            if (mask & (1 << i)) {
                if (!first) cout << "|";
                cout << arr->data[i + 1];
                first = false;
            }
        }

        cout << "}" << endl;
    }
}

// Обработка команд для массивов
void executeArrayCommand(DynamicArray* arrays, int* arrayCount, const char* command, const char* filename) {
    char cmd[50], arrayName[256], value[512];
    int index;
    
    // MPUSH - добавление в конец массива
    if (sscanf(command, "MPUSH %255s %511[^\n]", arrayName, value) == 2) {
        DynamicArray* arr = findArrayByName(arrays, arrayCount, arrayName);
        if (!arr) {
            arr = createArray(arrays, arrayCount, arrayName);
        }
        pushBack(arr, value);
        saveArraysToFile(filename, arrays, *arrayCount);
        cout << "Успешно!" << endl;
    }
    // MINSERT - вставка по индексу
    else if (sscanf(command, "MINSERT %255s %d %511[^\n]", arrayName, &index, value) == 3) {
        DynamicArray* arr = findArrayByName(arrays, arrayCount, arrayName);
        if (arr) {
            insertAt(arr, index + 1, value);  // +1 потому что индекс 0 - имя массива
            saveArraysToFile(filename, arrays, *arrayCount);
            cout << "Успешно!" << endl;
        } else {
            cout << "Массив не найден" << endl;
        }
    }
    // MGET - получение элемента по индексу
    else if (sscanf(command, "MGET %255s %d", arrayName, &index) == 2) {
        DynamicArray* arr = findArrayByName(arrays, arrayCount, arrayName);
        if (arr) {
            const char* result = getAt(arr, index + 1);  // +1 потому что индекс 0 - имя
            if (result) {
                cout << result << endl;
            } else {
                cout << "Несуществующий индекс" << endl;
            }
        } else {
            cout << "Массив не найден" << endl;
        }
    }
    // MDEL - удаление элемента по индексу
    else if (sscanf(command, "MDEL %255s %d", arrayName, &index) == 2) {
        DynamicArray* arr = findArrayByName(arrays, arrayCount, arrayName);
        if (arr) {
            removeAt(arr, index + 1);
            saveArraysToFile(filename, arrays, *arrayCount);
            cout << "Успешно!" << endl;
        } else {
            cout << "Массив не найден" << endl;
        }
    }
    // MREPLACE - замена элемента по индексу
    else if (sscanf(command, "MREPLACE %255s %d %511[^\n]", arrayName, &index, value) == 3) {
        DynamicArray* arr = findArrayByName(arrays, arrayCount, arrayName);
        if (arr) {
            replaceAt(arr, index + 1, value);
            saveArraysToFile(filename, arrays, *arrayCount);
            cout << "Успешно!" << endl;
        } else {
            cout << "Массив не найден" << endl;
        }
    }
    // MSIZE - получение размера массива
    else if (sscanf(command, "MSIZE %255s", arrayName) == 1) {
        DynamicArray* arr = findArrayByName(arrays, arrayCount, arrayName);
        if (arr) {
            cout << getSize(arr) - 1 << endl;  // -1 потому что первый элемент - имя
        } else {
            cout << "Массив не найден" << endl;
        }
    }
    // PRINT - вывод массива
    else if (sscanf(command, "PRINT %255s", arrayName) == 1) {
        printArray(arrays, arrayCount, arrayName);
    }
    //MPOW
   else if (sscanf(command, "MPOW %255s", arrayName) == 1) {
    DynamicArray* arr = findArrayByName(arrays, arrayCount, arrayName);
    if (!arr) {
        cout << "Массив не найден" << endl;
    } else {
        printAllSubarraysPowerset(arr);
    }
}
    else {
        cout << "Неизвестная команда" << endl;
    }
}