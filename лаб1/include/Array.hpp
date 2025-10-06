#pragma once

#include <iostream>
using namespace std;

// Шаблонная структура для массива
template<typename T>
struct Array {
    T* data = nullptr;          // Динамический массив значений
    bool* is_set = nullptr;    // Флаги для отслеживания установленных элементов
    int capacity = 0;          // Текущая вместимость массива
    int max_index = 0;         // Максимальный индекс, где есть установленный элемент
    ~Array() {
        delete[] data;
        delete[] is_set;
    }
};

// Создание массива с заданной вместимостью
template<typename T>
Array<T> create_array_ar(int new_capacity) {
    Array<T> new_arr;
    if (new_capacity > 0) {
        new_arr.capacity = new_capacity;
        new_arr.data = new T[new_capacity]();      // Инициализация по умолчанию (0 для int)
        new_arr.is_set = new bool[new_capacity](); // Инициализация false
    }
    return new_arr;
}

// Вспомогательная функция для расширения массива
template<typename T>
void resize_array_ar(Array<T>& arr, int new_capacity) {
    // Устанавливаем новую вместимость (не меньше текущей)
    if (new_capacity < arr.capacity) {
        new_capacity = arr.capacity;
    }
    T* new_data = new T[new_capacity]();      // Новый массив с инициализацией
    bool* new_is_set = new bool[new_capacity](); // Новый массив флагов

    // Копируем существующие данные
    for (int i = 0; i < arr.max_index; i++) {
        new_data[i] = arr.data[i];
        new_is_set[i] = arr.is_set[i];
    }

    // Освобождаем старую память
    delete[] arr.data;
    delete[] arr.is_set;

    // Обновляем поля
    arr.data = new_data;
    arr.is_set = new_is_set;
    arr.capacity = new_capacity;
}

// Добавление элемента в конец массива
template<typename T>
void add_element_end_ar(Array<T>& arr, T new_element) {
    if (arr.max_index >= arr.capacity) {
        // Расширяем массив, удваивая вместимость или устанавливая минимальную
        int new_capacity = (arr.capacity == 0) ? 4 : arr.capacity * 2;
        resize_array_ar(arr, new_capacity);
    }
    arr.data[arr.max_index] = new_element;
    arr.is_set[arr.max_index] = true;
    arr.max_index++;
}

// Добавление элемента по индексу (1-based)
template<typename T>
void add_element_index_ar(Array<T>& arr, int index, T new_element) {
    if (index < 1) {
        cerr << "Неправильное значение индекса" << endl;
        return;
    }
    // Проверяем, нужен ли resize
    if (index > arr.capacity) {
        // Устанавливаем новую вместимость как max(index, capacity * 2)
        int new_capacity = (arr.capacity == 0) ? index : max(index, arr.capacity * 2);
        resize_array_ar(arr, new_capacity);
    }
    arr.data[index - 1] = new_element;
    arr.is_set[index - 1] = true;
    if (index > arr.max_index) {
        arr.max_index = index; // Обновляем max_index
    }
}

// Удаление элемента по индексу (1-based)
template<typename T>
void delete_element_index_ar(Array<T>& arr, int index) {
    if (index < 1 || index > arr.max_index) {
        cerr << "Неправильное значение индекса" << endl;
        return;
    }
    if (!arr.is_set[index - 1]) {
        cerr << "Элемент по индексу " << index << " не установлен" << endl;
        return;
    }
    arr.is_set[index - 1] = false;
    arr.data[index - 1] = T(); // Сбрасываем значение
    // Обновляем max_index, если удалили последний элемент
    if (index == arr.max_index) {
        while (arr.max_index > 0 && !arr.is_set[arr.max_index - 1]) {
            arr.max_index--;
        }
    }
}

// Изменение элемента по индексу (1-based)
template<typename T>
void change_element_index_ar(Array<T>& arr, int index, T new_value) {
    if (index < 1 || index > arr.max_index) {
        cerr << "Неправильное значение индекса" << endl;
        return;
    }
    if (!arr.is_set[index - 1]) {
        cerr << "Элемент по индексу " << index << " не установлен" << endl;
        return;
    }
    arr.data[index - 1] = new_value;
    arr.is_set[index - 1] = true;
}

// Получение элемента по индексу (1-based)

template<typename T>
T get_element_index_ar(const Array<T>& arr, int index) {
    if (index < 1 || index > arr.max_index) {
        cerr << "Неправильное значение индекса" << endl;
        return;
    }
    if (!arr.is_set[index - 1]) {
        cerr << "Элемент по индексу " << index << " не установлен" << endl;
        return T();
    }
    return arr.data[index - 1];
}

// Вывод массива
template<typename T>
void print_array_ar(const Array<T>& arr) {
    if (arr.max_index == 0) {
        cout << "Массив пуст" << endl;
        return;
    }
    cout << "Элементы массива: ";
    for (int i = 0; i < arr.max_index; i++) {
        if (arr.is_set[i]) {
            cout << arr.data[i] << " ";
        } else {
            cout << "0 ";
        }
    }
    cout << endl;
}



