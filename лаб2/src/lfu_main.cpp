#include "lfu_cache.h"

using namespace std;

void printHelp() {
    cout << "=== LFU Cache Testing System ===" << endl;
    cout << "Команды:" << endl;
    cout << "  LCREATE <name> <capacity>     - создать LFU кэш" << endl;
    cout << "  LSET <cache> <key> <value>    - установить значение" << endl;
    cout << "  LGET <cache> <key>            - получить значение" << endl;
    cout << "  LPRINT <cache>                - вывести содержимое кэша" << endl;
    cout << "  LDELETE <cache>               - удалить кэш" << endl;
    cout << "  LSIZE <cache>                 - показать размер кэша" << endl;
    cout << "  HELP                          - показать эту справку" << endl;
    cout << "  EXIT                          - выход" << endl;
    cout << "=================================" << endl;
}

int main() {
    initLFUCacheSystem();
    
    cout << "Система LFU кэша инициализирована" << endl;
    printHelp();
    
    char command[1024];
    
    while (true) {
        cout << "LFU> ";
        cin.getline(command, sizeof(command));
        
        if (strcmp(command, "EXIT") == 0 || strcmp(command, "QUIT") == 0) {
            cout << "Выход из системы LFU кэша..." << endl;
            break;
        } else if (strcmp(command, "HELP") == 0) {
            printHelp();
        } else {
            // Обработка команд LFU кэша
            executeLFUCacheCommand(command);
        }
    }
    
    return 0;
}