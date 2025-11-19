#ifndef ARRAY_H
#define ARRAY_H

struct DynamicArray {
    char** data;        //массив 
    int capacity;       //вместимость
    int size;           //размер
};

//база
void initArray(DynamicArray* arr, int initialCapacity = 10);
void freeArray(DynamicArray* arr);
void pushBack(DynamicArray* arr, const char* value);
void insertAt(DynamicArray* arr, int index, const char* value);
const char* getAt(const DynamicArray* arr, int index);
void removeAt(DynamicArray* arr, int index);
void replaceAt(DynamicArray* arr, int index, const char* value);
int getSize(const DynamicArray* arr);

//команды
void executeArrayCommand(DynamicArray* arrays, int* arrayCount, const char* command, const char* filename);

//файлы
void saveArraysToFile(const char* filename, DynamicArray* arrays, int arrayCount);
void loadArraysFromFile(const char* filename, DynamicArray* arrays, int* arrayCount);

//функции
DynamicArray* findArrayByName(DynamicArray* arrays, int* arrayCount, const char* name);
DynamicArray* createArray(DynamicArray* arrays, int* arrayCount, const char* name);

//PRINT
void printArray(DynamicArray* arrays, int* arrayCount, const char* arrayName);

#endif
