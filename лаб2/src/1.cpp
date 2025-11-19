#include <iostream>
#include <string>
#include <sstream>
#include <cctype>
using namespace std;

template<typename T>
struct Stack 
{
private:

    struct Node
    {
        T data;
        Node* pNext;
        Node(T data = T(), Node* pNext = nullptr)
        {
            this -> data = data;
            this -> pNext = pNext;
        }
    };

    Node* head;
    int size;
public:
    Stack();
    ~Stack();
    void push(T data);
    void pop();
    T top();
    bool empty() { return size == 0; }
    void showStack();
    void clear();
    int getSize() { return size; }
};

template <typename T>
void Stack<T>::clear()
{
    while(size)
    {
        pop();
    }
}

template<typename T>
Stack<T>::Stack()
{
    size = 0;
    head = nullptr;
}

template <typename T>
Stack<T>::~Stack()
{
    clear();
}

template <typename T>
void Stack<T>::push(T data)
{
    head = new Node (data,head);
    size++;
}

template <typename T>
void Stack<T>::pop()
{
    if (head == nullptr)
    {
        cout << "Стэк пуст!" << endl;
        return;
    }
    else 
    {
        Node* toDelete = head;
        head = head -> pNext;
        delete toDelete;
        size--;
    }
}

template <typename T>
T Stack<T>::top()
{
    if (head == nullptr)
    {
        throw runtime_error("Стэк пуст!");
    }
    return head->data;
}

template <typename T>
void Stack<T>::showStack()
{
    if (head == nullptr)
    {
        cout << "Стэк пуст!" << endl;
        return;
    }
    else
    {
        Node* current = head;
        while(current != nullptr)
        {
            cout << current -> data << " ";
            current = current ->pNext;
        }
        cout << endl;
    }
}

// Функция для проверки, является ли строка оператором
bool isOperator(const string& token) {
    return token == "+" || token == "-" || token == "*" || token == "/";
}

// Функция для выполнения арифметической операции
int performOperation(int a, int b, const string& op) {
    if (op == "+") return a + b;
    if (op == "-") return a - b;
    if (op == "*") return a * b;
    if (op == "/") {
        if (b == 0) {
            throw runtime_error("Ошибка: деление на ноль!");
        }
        return a / b;
    }
    throw runtime_error("Неизвестный оператор: " + op);
}

// Функция для вычисления выражения в обратной польской записи
int evaluateRPN(const string& expression) {
    Stack<int> stack;
    stringstream ss(expression);
    string token;
    
    while (ss >> token) {
        if (isOperator(token)) {
            // Проверяем, что в стеке достаточно операндов
            if (stack.getSize() < 2) {
                throw runtime_error("Недостаточно операндов для операции: " + token);
            }
            
            // Извлекаем два последних операнда
            int b = stack.top();
            stack.pop();
            int a = stack.top();
            stack.pop();
            
            // Выполняем операцию и помещаем результат в стек
            int result = performOperation(a, b, token);
            stack.push(result);
        } else {
            // Пытаемся преобразовать токен в число
            try {
                int num = stoi(token);
                stack.push(num);
            } catch (const invalid_argument&) {
                throw runtime_error("Некорректный токен: " + token);
            } catch (const out_of_range&) {
                throw runtime_error("Число вне диапазона: " + token);
            }
        }
    }
    
    // Проверяем, что в стеке остался ровно один элемент
    if (stack.getSize() != 1) {
        throw runtime_error("Некорректное выражение");
    }
    
    return stack.top();
}

// Функция для разделения строки на токены 
void splitString(const string& str, char delimiter, string tokens[], int& count) {
    count = 0;
    string token;
    stringstream ss(str);
    
    while (getline(ss, token, delimiter)) {
        if (!token.empty()) {
            tokens[count++] = token;
        }
    }
}

int main()
{
    cout << "Калькулятор обратной польской записи" << endl;
    cout << "=====================================" << endl;
    
    // Пример из условия задачи: 3+1*4 в постфиксной записи = 3 1 4 * +
    string expression1 = "3 1 4 * +";
    cout << "Пример 1: " << expression1 << endl;
    try {
        int result1 = evaluateRPN(expression1);
        cout << "Результат: " << result1 << endl;
        cout << "Проверка: 3 + 1 * 4 = " << 3 + 1 * 4 << endl;
    } catch (const exception& e) {
        cout << "Ошибка: " << e.what() << endl;
    }
    cout << endl;
    
    // Дополнительные примеры
    string expressions[] = {
        "2 1 + 3 *",           // (2+1)*3 = 9
        "4 13 5 / +",          // 4 + (13/5) = 6
        "10 6 9 3 + -11 * / * 17 + 5 +", // сложный пример
        "5 1 2 + 4 * + 3 -"    // 5 + ((1+2)*4) - 3 = 14
    };
    
    int numExamples = sizeof(expressions) / sizeof(expressions[0]);
    
    for (int i = 0; i < numExamples; i++) {
        cout << "Пример " << i + 2 << ": " << expressions[i] << endl;
        try {
            int result = evaluateRPN(expressions[i]);
            cout << "Результат: " << result << endl;
        } catch (const exception& e) {
            cout << "Ошибка: " << e.what() << endl;
        }
        cout << endl;
    }
    
    // Интерактивный режим
    cout << "Интерактивный режим (для выхода введите 'exit')" << endl;
    cout << "Введите выражение в обратной польской записи: ";
    
    string input;
    while (getline(cin, input)) {
        if (input == "exit" || input == "quit") {
            break;
        }
        if (input.empty()) {
            continue;
        }
        
        try {
            int result = evaluateRPN(input);
            cout << "Результат: " << result << endl;
        } catch (const exception& e) {
            cout << "Ошибка: " << e.what() << endl;
        }
        
        cout << "Введите следующее выражение: ";
    }
    
    return 0;
}