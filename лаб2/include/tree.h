// tree.h
#ifndef TREE_H
#define TREE_H
#include <cstddef>

#define MAX_TREES 100
#define MAX_TREE_NAME 50

typedef struct TreeNode {
    int data;
    struct TreeNode* left;
    struct TreeNode* right;
} TreeNode;

typedef struct Tree {
    char name[MAX_TREE_NAME];
    TreeNode* root;
    size_t size;
} Tree;

// Прототипы функций
void initTree(Tree* tree, const char* name);
void freeTree(Tree* tree);
void treeInsert(Tree* tree, int value);
bool treeFind(Tree* tree, int value);
bool treeRemove(TreeNode** root, int value);
bool isTreeFull(TreeNode* node);
int treeHeight(TreeNode* node);
void printPreOrder(TreeNode* node);
void printInOrder(TreeNode* node);
void printPostOrder(TreeNode* node);
void printLevelOrder(TreeNode* node);
void printTreeLeaves(Tree* tree);
void printTreeInfo(Tree* tree);
void loadTreesFromFile(const char* filename, Tree* trees, int* treeCount);
void executeTreeCommand(Tree* trees, int* treeCount, const char* command, const char* filename);

#endif