#include "tree.h"
#include "array.h"
#include <iostream>
#include <cstdlib>
#include <cstring>
#include <cstdio>
#include <queue>
#include <sstream>
#include <algorithm>
using namespace std;

#define MAX_QUERY_LENGTH 1024

// Инициализация бинарного дерева поиска
void initTree(Tree* tree, const char* name) {
    tree->root = nullptr;
    tree->size = 0;
    strncpy(tree->name, name, sizeof(tree->name) - 1);
    tree->name[sizeof(tree->name) - 1] = '\0';
}

// Рекурсивное освобождение памяти узлов дерева
void freeTreeNode(TreeNode* node) {
    if (node == nullptr) return;
    freeTreeNode(node->left);
    freeTreeNode(node->right);
    free(node);
}

// Освобождение памяти всего дерева
void freeTree(Tree* tree) {
    freeTreeNode(tree->root);
    tree->root = nullptr;
    tree->size = 0;
}

// Вставка значения в бинарное дерево поиска
void treeInsert(Tree* tree, int value) {
    TreeNode* newNode = (TreeNode*)malloc(sizeof(TreeNode));
    if (newNode == nullptr) {
        cout << "Ошибка выделения памяти" << endl;
        return;
    }
    newNode->data = value;
    newNode->left = nullptr;
    newNode->right = nullptr;
    
    if (tree->root == nullptr) {
        tree->root = newNode;
        tree->size++;
        return;
    }
    
    TreeNode* current = tree->root;
    TreeNode* parent = nullptr;
    
    while (current != nullptr) {
        parent = current;
        if (value < current->data) {
            current = current->left;
        } else if (value > current->data) {
            current = current->right;
        } else {
            // Дубликат - игнорируем вставку
            free(newNode);
            return;
        }
    }
    
    if (value < parent->data) {
        parent->left = newNode;
    } else {
        parent->right = newNode;
    }
    
    tree->size++;
}

// Поиск значения в дереве
bool treeFind(Tree* tree, int value) {
    TreeNode* current = tree->root;
    
    while (current != nullptr) {
        if (value == current->data) {
            return true;
        } else if (value < current->data) {
            current = current->left;
        } else {
            current = current->right;
        }
    }
    
    return false;
}

// Проверка, является ли дерево полным бинарным деревом
bool isTreeFull(TreeNode* node) {
    if (node == nullptr) return true;
    
    if (node->left == nullptr && node->right == nullptr) return true;
    
    if (node->left != nullptr && node->right != nullptr) {
        return isTreeFull(node->left) && isTreeFull(node->right);
    }
    
    return false;
}

// Вычисление высоты дерева
int treeHeight(TreeNode* node) {
    if (node == nullptr) {
        return 0;
    }
    
    int leftHeight = treeHeight(node->left);
    int rightHeight = treeHeight(node->right);
    
    return 1 + (leftHeight > rightHeight ? leftHeight : rightHeight);
}

// Прямой обход (Pre-order): корень - левое - правое
void printPreOrder(TreeNode* node) {
    if (node == nullptr) return;
    cout << node->data << " ";
    printPreOrder(node->left);
    printPreOrder(node->right);
}

// Симметричный обход (In-order): левое - корень - правое
void printInOrder(TreeNode* node) {
    if (node == nullptr) return;
    printInOrder(node->left);
    cout << node->data << " ";
    printInOrder(node->right);
}

// Обратный обход (Post-order): левое - правое - корень
void printPostOrder(TreeNode* node) {
    if (node == nullptr) return;
    printPostOrder(node->left);
    printPostOrder(node->right);
    cout << node->data << " ";
}

// Обход в ширину (Level-order)
void printLevelOrder(TreeNode* node) {
    if (node == nullptr) return;
    
    queue<TreeNode*> q;
    q.push(node);
    
    while (!q.empty()) {
        TreeNode* current = q.front();
        q.pop();
        cout << current->data << " ";
        
        if (current->left != nullptr) {
            q.push(current->left);
        }
        if (current->right != nullptr) {
            q.push(current->right);
        }
    }
}

// Удаление всего дерева
void treeDelete(Tree* tree) {
    freeTree(tree);
}

// Поиск дерева по имени
Tree* findTreeByName(Tree* trees, int treeCount, const char* name) {
    for (int i = 0; i < treeCount; i++) {
        if (strcmp(trees[i].name, name) == 0) {
            return &trees[i];
        }
    }
    return nullptr;
}

// Создание нового дерева
Tree* createTree(Tree* trees, int* treeCount, const char* name) {
    if (*treeCount >= MAX_TREES) {
        cout << "Достигнуто максимальное количество деревьев" << endl;
        return nullptr;
    }
    
    Tree* newTree = &trees[*treeCount];
    initTree(newTree, name);
    (*treeCount)++;
    return newTree;
}

// Рекурсивная функция для сохранения структуры дерева (pre-order с null-маркерами)
void saveTreeRecursive(FILE* file, TreeNode* node) {
    if (node == nullptr) {
        fprintf(file, "NULL\n");
        return;
    }
    
    fprintf(file, "%d\n", node->data);
    saveTreeRecursive(file, node->left);
    saveTreeRecursive(file, node->right);
}

// Сохранение деревьев в файл с сохранением структуры
void saveTreesToFile(const char* filename, Tree* trees, int treeCount) {
    FILE* file = fopen(filename, "w");
    if (!file) {
        cout << "Ошибка открытия файла для записи" << endl;
        return;
    }
    
    fprintf(file, "TREE_COUNT %d\n", treeCount);
    
    for (int i = 0; i < treeCount; i++) {
        fprintf(file, "TREE_START %s %zu\n", trees[i].name, trees[i].size);
        if (trees[i].root != nullptr) {
            saveTreeRecursive(file, trees[i].root);
        }
        fprintf(file, "TREE_END\n");
    }
    
    fclose(file);
    cout << "Деревья сохранены в '" << filename << "'" << endl;
}

// Рекурсивная функция для загрузки структуры дерева
TreeNode* loadTreeRecursive(FILE* file) {
    char line[256];
    if (!fgets(line, sizeof(line), file)) {
        return nullptr;
    }
    
    line[strcspn(line, "\n")] = 0;
    
    if (strcmp(line, "NULL") == 0) {
        return nullptr;
    }
    
    int value;
    if (sscanf(line, "%d", &value) == 1) {
        TreeNode* node = (TreeNode*)malloc(sizeof(TreeNode));
        if (node == nullptr) {
            return nullptr;
        }
        node->data = value;
        node->left = loadTreeRecursive(file);
        node->right = loadTreeRecursive(file);
        return node;
    }
    
    return nullptr;
}

// Загрузка деревьев из файла
void loadTreesFromFile(const char* filename, Tree* trees, int* treeCount) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        cout << "Ошибка открытия файла для чтения" << endl;
        return;
    }
    
    char line[256];
    int savedTreeCount = 0;
    
    // Читаем количество деревьев
    if (fgets(line, sizeof(line), file)) {
        sscanf(line, "TREE_COUNT %d", &savedTreeCount);
    }
    
    *treeCount = 0;
    
    while (fgets(line, sizeof(line), file)) {
        line[strcspn(line, "\n")] = 0;
        
        if (strncmp(line, "TREE_START ", 11) == 0) {
            char treeName[256];
            size_t treeSize;
            if (sscanf(line + 11, "%255s %zu", treeName, &treeSize) == 2) {
                if (*treeCount < MAX_TREES) {
                    Tree* tree = &trees[*treeCount];
                    initTree(tree, treeName);
                    tree->root = loadTreeRecursive(file);
                    tree->size = treeSize;
                    (*treeCount)++;
                }
            }
        }
    }
    
    fclose(file);
    cout << "Деревья загружены из '" << filename << "'" << endl;
}

// Рекурсивный сбор листьев
void collectLeaves(TreeNode* node, DynamicArray* leaves) {
    if (node == nullptr) return;
    
    if (node->left == nullptr && node->right == nullptr) {
        char valueStr[20];
        snprintf(valueStr, sizeof(valueStr), "%d", node->data); //Записываем данные в строку
        pushBack(leaves, valueStr);
        return;
    }
    
    collectLeaves(node->left, leaves);
    collectLeaves(node->right, leaves);
}

// Функция для сортировки массива чисел (пузырьковая сортировка)
void sortLeavesArray(DynamicArray* leaves) {
    for (int i = 0; i < leaves->size - 1; i++) {
        for (int j = 0; j < leaves->size - i - 1; j++) {
            int val1 = atoi(leaves->data[j]); //Преобразование строки в целое число
            int val2 = atoi(leaves->data[j + 1]);
            if (val1 > val2) {
                // Меняем местами
                char* temp = leaves->data[j];
                leaves->data[j] = leaves->data[j + 1];
                leaves->data[j + 1] = temp;
            }
        }
    }
}

// Вывод всех листьев дерева в порядке возрастания
void printTreeLeaves(Tree* tree) {
    if (tree == nullptr || tree->root == nullptr) {
        cout << "Дерево пусто" << endl;
        return;
    }
    
    // Создаем временный массив для хранения листьев
    DynamicArray leaves;
    initArray(&leaves, 10);
    
    // Собираем листья
    collectLeaves(tree->root, &leaves);
    
    if (leaves.size == 0) {
        cout << "В дереве нет листьев" << endl;
    } else {
        // Сортируем массив листьев
        sortLeavesArray(&leaves);
        
        cout << "Листья дерева в порядке возрастания: ";
        for (int i = 0; i < leaves.size; i++) {
            cout << leaves.data[i];
            if (i < leaves.size - 1) {
                cout << " ";
            }
        }
        cout << endl;
    }
    
    // Освобождаем временный массив
    freeArray(&leaves);
}

// Удаление узла из дерева (дополнительная функция)
TreeNode* findMin(TreeNode* node) {
    while (node && node->left != nullptr) {
        node = node->left;
    }
    return node;
}

bool treeRemove(TreeNode** root, int value) {
    if (*root == nullptr) return false;
    
    if (value < (*root)->data) {
        return treeRemove(&(*root)->left, value);
    } else if (value > (*root)->data) {
        return treeRemove(&(*root)->right, value);
    } else {
        // Узел найден
        if ((*root)->left == nullptr) {
            TreeNode* temp = (*root)->right;
            free(*root);
            *root = temp;
            return true;
        } else if ((*root)->right == nullptr) {
            TreeNode* temp = (*root)->left;
            free(*root);
            *root = temp;
            return true;
        } else {
            // У узла два потомка
            TreeNode* temp = findMin((*root)->right);
            (*root)->data = temp->data;
            return treeRemove(&(*root)->right, temp->data);
        }
    }
}

// Вывод информации о дереве
void printTreeInfo(Tree* tree) {
    if (tree == nullptr) {
        cout << "Дерево не существует" << endl;
        return;
    }
    
    cout << "Дерево: " << tree->name << endl;
    cout << "Размер: " << tree->size << endl;
    cout << "Высота: " << treeHeight(tree->root) << endl;
    cout << "Полное: " << (isTreeFull(tree->root) ? "да" : "нет") << endl;
    
    cout << "In-order: ";
    printInOrder(tree->root);
    cout << endl;
    
    cout << "Pre-order: ";
    printPreOrder(tree->root);
    cout << endl;
    
    cout << "Post-order: ";
    printPostOrder(tree->root);
    cout << endl;
    
    cout << "Level-order: ";
    printLevelOrder(tree->root);
    cout << endl;
}

// Обработка команд для деревьев
void executeTreeCommand(Tree* trees, int* treeCount, const char* command, const char* filename) {
    char cmd[50], treeName[256], valueStr[512];
    int value;
    
    // TCREATE - создание нового дерева
    if (sscanf(command, "TCREATE %255s", treeName) == 1) {
        if (createTree(trees, treeCount, treeName)) {
            cout << "Дерево '" << treeName << "' создано" << endl;
            saveTreesToFile(filename, trees, *treeCount);
        }
    }
    // TINSERT - вставка значения в дерево
    else if (sscanf(command, "TINSERT %255s %511s", treeName, valueStr) == 2) {
        if (sscanf(valueStr, "%d", &value) == 1) {
            Tree* tree = findTreeByName(trees, *treeCount, treeName);
            if (!tree) {
                tree = createTree(trees, treeCount, treeName);
                if (!tree) return;
            }
            treeInsert(tree, value);
            saveTreesToFile(filename, trees, *treeCount);
            cout << "Значение " << value << " добавлено в дерево '" << treeName << "'" << endl;
        } else {
            cout << "Неверный формат числа: " << valueStr << endl;
        }
    }
    // TFIND - поиск значения в дереве
    else if (sscanf(command, "TFIND %255s %511s", treeName, valueStr) == 2) {
        if (sscanf(valueStr, "%d", &value) == 1) {
            Tree* tree = findTreeByName(trees, *treeCount, treeName);
            if (tree) {
                if (treeFind(tree, value)) {
                    cout << "Значение " << value << " найдено в дереве '" << treeName << "'" << endl;
                } else {
                    cout << "Значение " << value << " не найдено в дереве '" << treeName << "'" << endl;
                }
            } else {
                cout << "Дерево '" << treeName << "' не найдено" << endl;
            }
        } else {
            cout << "Неверный формат числа: " << valueStr << endl;
        }
    }
    // TREMOVE - удаление значения из дерева
    else if (sscanf(command, "TREMOVE %255s %511s", treeName, valueStr) == 2) {
        if (sscanf(valueStr, "%d", &value) == 1) {
            Tree* tree = findTreeByName(trees, *treeCount, treeName);
            if (tree) {
                if (treeRemove(&tree->root, value)) {
                    tree->size--;
                    saveTreesToFile(filename, trees, *treeCount);
                    cout << "Значение " << value << " удалено из дерева '" << treeName << "'" << endl;
                } else {
                    cout << "Значение " << value << " не найдено в дереве '" << treeName << "'" << endl;
                }
            } else {
                cout << "Дерево '" << treeName << "' не найдено" << endl;
            }
        } else {
            cout << "Неверный формат числа: " << valueStr << endl;
        }
    }
    // TFULL - проверка полноты дерева
    else if (sscanf(command, "TFULL %255s", treeName) == 1) {
        Tree* tree = findTreeByName(trees, *treeCount, treeName);
        if (tree) {
            if (isTreeFull(tree->root)) {
                cout << "Дерево '" << treeName << "' является полным" << endl;
            } else {
                cout << "Дерево '" << treeName << "' не является полным" << endl;
            }
        } else {
            cout << "Дерево '" << treeName << "' не найдено" << endl;
        }
    }
    // TDELETE - удаление всего дерева
    else if (sscanf(command, "TDELETE %255s", treeName) == 1) {
        Tree* tree = findTreeByName(trees, *treeCount, treeName);
        if (tree) {
            freeTree(tree);
            saveTreesToFile(filename, trees, *treeCount);
            cout << "Дерево '" << treeName << "' удалено" << endl;
        } else {
            cout << "Дерево '" << treeName << "' не найдено" << endl;
        }
    }
    // TSIZE - получение размера дерева
    else if (sscanf(command, "TSIZE %255s", treeName) == 1) {
        Tree* tree = findTreeByName(trees, *treeCount, treeName);
        if (tree) {
            cout << "Размер дерева '" << treeName << "': " << tree->size << endl;
        } else {
            cout << "Дерево '" << treeName << "' не найдено" << endl;
        }
    }
    // THEIGHT - вычисление высоты дерева
    else if (sscanf(command, "THEIGHT %255s", treeName) == 1) {
        Tree* tree = findTreeByName(trees, *treeCount, treeName);
        if (tree) {
            int height = treeHeight(tree->root);
            cout << "Высота дерева '" << treeName << "': " << height << endl;
        } else {
            cout << "Дерево '" << treeName << "' не найдено" << endl;
        }
    }
    // TLEAVES - вывод всех листьев дерева
    else if (sscanf(command, "TLEAVES %255s", treeName) == 1) {
        Tree* tree = findTreeByName(trees, *treeCount, treeName);
        if (tree) {
            printTreeLeaves(tree);
        } else {
            cout << "Дерево '" << treeName << "' не найдено" << endl;
        }
    }
    // TPRINT - вывод информации о дереве
    else if (sscanf(command, "TPRINT %255s", treeName) == 1) {
        Tree* tree = findTreeByName(trees, *treeCount, treeName);
        if (tree) {
            printTreeInfo(tree);
        } else {
            cout << "Дерево '" << treeName << "' не найдено" << endl;
        }
    }
    // TSEQUENCE - обработка последовательности чисел
    else if (strncmp(command, "TSEQUENCE", 9) == 0) {
        char sequence[1024];
        if (sscanf(command, "TSEQUENCE %255s %1023[^\n]", treeName, sequence) == 2) {
            Tree* tree = findTreeByName(trees, *treeCount, treeName);
            if (!tree) {
                tree = createTree(trees, treeCount, treeName);
                if (!tree) return;
            }
            
            // Очищаем дерево
            freeTree(tree);
            initTree(tree, treeName);
            
            // Безопасный разбор последовательности
            char* token = strtok(sequence, " ");
            int count = 0;
            
            while (token != nullptr) {
                int num = atoi(token);
                treeInsert(tree, num);
                count++;
                token = strtok(nullptr, " ");
            }
            
            int height = treeHeight(tree->root);
            cout << "Добавлено " << count << " значений. Высота дерева: " << height << endl;
            
            saveTreesToFile(filename, trees, *treeCount);
        } else {
            cout << "Неверный формат команды TSEQUENCE" << endl;
        }
    }
    // TSAVE - принудительное сохранение
    else if (strcmp(command, "TSAVE") == 0) {
        saveTreesToFile(filename, trees, *treeCount);
    }
    // TLOAD - принудительная загрузка
    else if (strcmp(command, "TLOAD") == 0) {
        loadTreesFromFile(filename, trees, treeCount);
    }
    // TLIST - список всех деревьев
    else if (strcmp(command, "TLIST") == 0) {
        cout << "Количество деревьев: " << *treeCount << endl;
        for (int i = 0; i < *treeCount; i++) {
            cout << "  " << i + 1 << ". " << trees[i].name << " (размер: " << trees[i].size << ")" << endl;
        }
    }
    else {
        cout << "Неизвестная команда дерева" << endl;
        cout << "Доступные команды:" << endl;
        cout << "  TCREATE <name> - создать дерево" << endl;
        cout << "  TINSERT <tree> <value> - вставить значение" << endl;
        cout << "  TFIND <tree> <value> - найти значение" << endl;
        cout << "  TREMOVE <tree> <value> - удалить значение" << endl;
        cout << "  TDELETE <tree> - удалить дерево" << endl;
        cout << "  TSIZE <tree> - размер дерева" << endl;
        cout << "  THEIGHT <tree> - высота дерева" << endl;
        cout << "  TFULL <tree> - проверка полноты" << endl;
        cout << "  TLEAVES <tree> - вывод листьев" << endl;
        cout << "  TPRINT <tree> - информация о дереве" << endl;
        cout << "  TSEQUENCE <tree> <values> - вставить последовательность" << endl;
        cout << "  TSAVE - сохранить деревья" << endl;
        cout << "  TLOAD - загрузить деревья" << endl;
        cout << "  TLIST - список деревьев" << endl;
    }
}