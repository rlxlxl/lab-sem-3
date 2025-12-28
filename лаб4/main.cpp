#include <iostream>
using namespace std;
int main() {
    cout << "Введите sql запрос( для завершение введите пустую строку или ;): " << endl;
    cout << "> SELECT * FROM clients" << endl;
    cout << "Empty" << endl; 
    cout << "INSERT INTO clients (name, age) VALUES ('John', 30)" << endl; 
    cout << "Success" << endl;
}