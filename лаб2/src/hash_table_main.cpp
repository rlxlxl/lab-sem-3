#include "hash_table.h"
#include <iostream>

using namespace std;

int main() {
    initHashTableSystem();
    
    cout << "=== Система хэш-таблиц ===" << endl;
    cout << "Введите команды (или 'quit' для выхода):" << endl;
    
    char command[1024];
    while (true) {
        cout << "> ";
        cin.getline(command, sizeof(command));
        
        if (strcmp(command, "quit") == 0) {
            break;
        }
        
        if (strlen(command) > 0) {
            executeHashTableCommand(command);
        }
    }
    
    return 0;
}
