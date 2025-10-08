
#include "Array.hpp"
#include "SystemWork.hpp"
#include "CommandParser.hpp"
#include <cstring>
using namespace std;

Array<string> read_command_to_array(const string& command) {
    return tokenize_to_array(command);
}

int main(int argc, char* argv[]) {
    if (argc < 3) {
        cerr << "Использование: ./dbms --file file.data --query 'COMMAND ...'\n";
        return 1;
    }

    string file_path;
    string query_str;
    for (int i = 1; i < argc; ++i) {
        if (strcmp(argv[i], "--file") == 0 && i + 1 < argc) file_path = argv[++i];
        else if (strcmp(argv[i], "--query") == 0 && i + 1 < argc) query_str = argv[++i];
    }

    if (query_str.empty()) {
        bool after_file = false;
        for (int i = 1; i < argc; ++i) {
            if (after_file) {
                if (!query_str.empty()) query_str += " ";
                query_str += argv[i];
            }
            if (strcmp(argv[i], "--file") == 0) { ++i; after_file = true; }
        }
    }

    if (file_path.empty()) {
        cerr << "Не задан файл. Используйте --file <path>\n";
        return 1;
    }

    g_file_path = file_path;
    load_db(file_path);

    Array<string> toks = read_command_to_array(query_str);
    if (toks.max_index == 0) { cerr << "Пустая команда\n"; return 1; }

    // --- enum + switch ---
    enum class CommandType {
        MPUSH, MGET, MDEL,
        FPUSH, FGET, FDEL,
        LPUSH, LGET, LDEL,
        QPUSH, QPOP, QGET,
        SPUSH, SPOP, SGET,
        TINSERT, TFULL, TGET,
        PRINT, UNKNOWN

    };

    auto parse_command = [](const string& cmd) -> CommandType {
        if (cmd == "MPUSH") return CommandType::MPUSH;
        if (cmd == "MGET")  return CommandType::MGET;
        if (cmd == "MDEL")  return CommandType::MDEL;
        if (cmd == "FPUSH") return CommandType::FPUSH;
        if (cmd == "FGET")  return CommandType::FGET;
        if (cmd == "FDEL")  return CommandType::FDEL;
        if (cmd == "LPUSH") return CommandType::LPUSH;
        if (cmd == "LGET")  return CommandType::LGET;
        if (cmd == "LDEL")  return CommandType::LDEL;
        if (cmd == "QPUSH") return CommandType::QPUSH;
        if (cmd == "QPOP")  return CommandType::QPOP;
        if (cmd == "QGET")  return CommandType::QGET;
        if (cmd == "SPUSH") return CommandType::SPUSH;
        if (cmd == "SPOP")  return CommandType::SPOP;
        if (cmd == "SGET")  return CommandType::SGET;
        if (cmd == "TINSERT") return CommandType::TINSERT;
        if (cmd == "TFULL")  return CommandType::TFULL;
        if (cmd == "TGET")  return CommandType::TGET;
        if (cmd == "PRINT")  return CommandType::PRINT;
        return CommandType::UNKNOWN;
    };

    CommandType ctype = parse_command(toks.data[0]);
    switch (ctype) {
        case CommandType::MPUSH: cmd_Mpush(toks); break;
        case CommandType::MGET:  cmd_Mget(toks);  break;
        case CommandType::MDEL:  cmd_Mdel(toks);  break;
        case CommandType::FPUSH: cmd_Fpush(toks); break;
        case CommandType::FGET:  cmd_Fget(toks);  break;
        case CommandType::FDEL:  cmd_Fdel(toks);  break;
        case CommandType::LPUSH: cmd_Lpush(toks); break;
        case CommandType::LGET:  cmd_Lget(toks);  break;
        case CommandType::LDEL:  cmd_Ldel(toks);  break;
        case CommandType::QPUSH: cmd_Qpush(toks); break;
        case CommandType::QPOP:  cmd_Qpop(toks);  break;
        case CommandType::QGET:  cmd_Qget(toks);  break;
        case CommandType::SPUSH: cmd_Spush(toks); break;
        case CommandType::SPOP:  cmd_Spop(toks);  break;
        case CommandType::SGET:  cmd_Sget(toks);  break;
        case CommandType::TINSERT: cmd_Tinsert(toks); break;
        case CommandType::TFULL:  cmd_Tfull(toks);  break;
        case CommandType::TGET:  cmd_Tget(toks);  break;
        case CommandType::PRINT:  cmd_PRINT(toks);  break;
        default:
            cerr << "Неизвестная команда: " << toks.data[0] << "\n";
    }

    return 0;
}