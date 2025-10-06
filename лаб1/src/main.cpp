/*#include "Queue.hpp"
#include "Stack.hpp"
#include "Array.hpp"
#include "DoublyList.hpp"
#include "ForwardList.hpp"*/
#include "Array.hpp"
#include "SystemWork.hpp"
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
using namespace std;

enum commands {
    MPUSH = 1,
    MDEL = 2,
    MGET = 3,
    FPUSH = 4,
    FDEL = 5,
    FGET = 6,
    LPUSH = 7,
    LDEL = 8,
    LGET = 9,
    QPUSH = 10,
    QPOP = 11,
    QGET = 12,
    SPUSH = 13,
    SPOP = 14,
    SGET = 15,
    EXIT = 0
};

int main(int argc, char* argv[]) {
    bool short_commands = false;
    string path = string(argv[2]);
    Array<string> lines = write_in_dbms(path);
    Array<string> commands = read_command(argv[3]);
    
    if (commands.data[0] == "QPOP" || commands.data[0] == "SPOP") {
        short_commands = true;
    }

    if (short_commands) {
    }
}

