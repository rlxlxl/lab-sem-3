#pragma once

#include <iostream>
#include <optional>
using namespace std;

template<typename T>
struct Array {
    T* data = nullptr;
    bool* is_set = nullptr; // Tracks which indices are explicitly set
    int capacity = 0;
    int max_index = 0; // Tracks the highest index explicitly set
    ~Array() {
        delete[] data;
        delete[] is_set;
    }
};

template<typename T>
Array<T> create_array_ar(int new_capacity) {
    Array<T> new_arr;
    if (new_capacity > 0) {
        new_arr.capacity = new_capacity;
        new_arr.data = new T[new_capacity](); // Initialize with default value (0 for int)
        new_arr.is_set = new bool[new_capacity](); // Initialize all to false
    }
    return new_arr;
}

template<typename T>
void add_element_end_ar(Array<T>& arr, T new_element) {
    if (arr.max_index < arr.capacity) {
        arr.data[arr.max_index] = new_element;
        arr.is_set[arr.max_index] = true;
        arr.max_index++; // Move max_index to the next position
    } else {
        std::cerr << "Невозможно добавить элемент: массив полон" << std::endl;
    }
}

template<typename T>
void add_element_index_ar(Array<T>& arr, int index, T new_element) {
    if (index < 1 || index > arr.capacity) {
        std::cerr << "Неправильное значение индекса" << std::endl;
        return;
    }
    arr.data[index - 1] = new_element;
    arr.is_set[index - 1] = true; // Mark this index as explicitly set
    if (index > arr.max_index) {
        arr.max_index = index; // Update max_index if this is the highest index set
    }
}

template<typename T>
void delete_element_index_ar(Array<T>& arr, int index) {
    if (index < 1 || index > arr.max_index) {
        std::cerr << "Неправильное значение индекса" << std::endl;
        return;
    }
    if (!arr.is_set[index - 1]) {
        std::cerr << "Элемент по индексу " << index << " не установлен" << std::endl;
        return;
    }
    arr.is_set[index - 1] = false; // Mark as unset
    arr.data[index - 1] = T(); // Reset to default value (e.g., 0 for int)
    // Update max_index if the deleted element was at the highest index
    if (index == arr.max_index) {
        while (arr.max_index > 0 && !arr.is_set[arr.max_index - 1]) {
            arr.max_index--;
        }
    }
}

template<typename T>
void change_element_index_ar(Array<T>& arr, int index, T new_value) {
    if (index < 1 || index > arr.max_index) {
        std::cerr << "Неправильное значение индекса" << std::endl;
        return;
    }
    if (!arr.is_set[index - 1]) {
        std::cerr << "Элемент по индексу " << index << " не установлен" << std::endl;
        return;
    }
    arr.data[index - 1] = new_value;
    arr.is_set[index - 1] = true; // Ensure it remains marked as set
}

template<typename T>
std::optional<T> get_element_index_ar(Array<T>& arr, int index) {
    if (index < 1 || index > arr.max_index) {
        std::cerr << "Неправильное значение индекса" << std::endl;
        return std::nullopt;
    }
    if (!arr.is_set[index - 1]) {
        std::cerr << "Элемент по индексу " << index << " не установлен" << std::endl;
        return std::nullopt;
    }
    return arr.data[index - 1];
}

template<typename T>
void print_array_ar(Array<T>& arr) {
    if (arr.max_index == 0) {
        std::cerr << "Массив пуст" << std::endl;
        return;
    }
    std::cout << "Элементы массива: ";
    for (int i = 0; i < arr.max_index; i++) {
        if (arr.is_set[i]) {
            std::cout << arr.data[i] << " ";
        } else {
            std::cout << "0 "; // Print 0 for unset positions
        }
    }
    std::cout << std::endl;
}

