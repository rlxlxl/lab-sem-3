#ifndef STACK_H
#define STACK_H

struct StackNode {
    char* data;
    StackNode* next;
};

struct Stack {
    StackNode* top;
    int size;
    char name[50];
};

void initStack(Stack* stack, const char* name);
void freeStack(Stack* stack);
void stackPush(Stack* stack, const char* value);
char stackPop(Stack* stack);
const char* stackPeek(Stack* stack);
int getStackSize(Stack* stack);

void executeStackCommand(Stack* stacks, int* stackCount, const char* command, const char* filename);
void saveStacksToFile(const char* filename, Stack* stacks, int stackCount);
void loadStacksFromFile(const char* filename, Stack* stacks, int* stackCount);
Stack* findStackByName(Stack* stacks, int* stackCount, const char* name);
Stack* createStack(Stack* stacks, int* stackCount, const char* name);

#endif
