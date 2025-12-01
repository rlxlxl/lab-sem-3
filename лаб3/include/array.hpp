#pragma once

#include <iostream>
using namespace std;

template<typename T>
class Array {
private:
    T* data;          // Данные
    bool* is_set;     // Флаг установленности
    int capacity;     // Размер выделенной памяти
    int max_index;    // Количество реально заполненных элементов

    void resize(int new_capacity) {
        if (new_capacity < capacity)
            new_capacity = capacity;

        T* new_data = new T[new_capacity]();
        bool* new_is_set = new bool[new_capacity]();

        for (int i = 0; i < max_index; i++) {
            new_data[i] = data[i];
            new_is_set[i] = is_set[i];
        }

        delete[] data;
        delete[] is_set;

        data = new_data;
        is_set = new_is_set;
        capacity = new_capacity;
    }

public:

    // Конструктор по умолчанию
    Array() : data(nullptr), is_set(nullptr), capacity(0), max_index(0) {}

    // Конструктор с начальной вместимостью
    Array(int new_capacity) : data(nullptr), is_set(nullptr), capacity(0), max_index(0) {
        if (new_capacity > 0) {
            capacity = new_capacity;
            data = new T[new_capacity]();
            is_set = new bool[new_capacity]();
        }
    }

    // Деструктор
    ~Array() {
        delete[] data;
        delete[] is_set;
    }

    // Добавление в конец
    void push_back(T value) {
        if (max_index >= capacity) {
            int new_capacity = (capacity == 0) ? 4 : capacity * 2;
            resize(new_capacity);
        }
        data[max_index] = value;
        is_set[max_index] = true;
        max_index++;
    }

    // Добавление по индексу (1-based)
    void set(int index, T value) {
        if (index < 1) {
            cerr << "Неправильное значение индекса\n";
            return;
        }

        if (index > capacity) {
            int new_capacity = (capacity == 0) ? index : max(index, capacity * 2);
            resize(new_capacity);
        }

        data[index - 1] = value;
        is_set[index - 1] = true;

        if (index > max_index)
            max_index = index;
    }

    // Удаление по индексу
    void remove(int index) {
        if (index < 1 || index > max_index) {
            cerr << "Неправильное значение индекса\n";
            return;
        }

        if (!is_set[index - 1]) {
            cerr << "Элемент по индексу " << index << " не установлен\n";
            return;
        }

        is_set[index - 1] = false;
        data[index - 1] = T();

        if (index == max_index) {
            while (max_index > 0 && !is_set[max_index - 1]) {
                max_index--;
            }
        }
    }

    // Изменение элемента по индексу
    void change(int index, T value) {
        if (index < 1 || index > max_index) {
            cerr << "Неправильное значение индекса\n";
            return;
        }

        if (!is_set[index - 1]) {
            cerr << "Элемент по индексу " << index << " не установлен\n";
            return;
        }

        data[index - 1] = value;
    }

    // Получение элемента
    T get(int index) const {
        if (index < 1 || index > max_index) {
            cerr << "Неправильное значение индекса\n";
            return T();
        }

        if (!is_set[index - 1]) {
            cerr << "Элемент по индексу " << index << " не установлен\n";
            return T();
        }

        return data[index - 1];
    }

    // Печать массива
    void print() const {
        if (max_index == 0) {
            cout << "Массив пуст\n";
            return;
        }

        cout << "Элементы массива: ";
        for (int i = 0; i < max_index; i++) {
            if (is_set[i])
                cout << data[i] << " ";
            else
                cout << "0 ";
        }
        cout << endl;
    }
};
