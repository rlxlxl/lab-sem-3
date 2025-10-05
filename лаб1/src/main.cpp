#include "Array.hpp"
#include "ForwardList.hpp"
#include "DoublyList.hpp"
#include <Stack.hpp>
int main() {
    Stack<int> test = create_stack<int>();
    print_stack<int>(test);
    push_stack(test, 12);
    push_stack(test, 10);
    pop_stack(test);
    print_stack<int>(test);
}