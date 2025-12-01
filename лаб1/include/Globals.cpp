#include "SystemWork.hpp"

Array<string> names_M = create_array_ar<string>(4);
Array<Array<string>*> data_M = create_array_ar<Array<string>*>(4);

Array<string> names_F = create_array_ar<string>(4);
Array<ForwardList<string>*> data_F = create_array_ar<ForwardList<string>*>(4);

Array<string> names_L = create_array_ar<string>(4);
Array<DoublyList<string>*> data_L = create_array_ar<DoublyList<string>*>(4);

Array<string> names_Q = create_array_ar<string>(4);
Array<Queue<string>*> data_Q = create_array_ar<Queue<string>*>(4);

Array<string> names_S = create_array_ar<string>(4);
Array<Stack<string>*> data_S = create_array_ar<Stack<string>*>(4);

Array<string> names_T = create_array_ar<string>(4);
Array<FullBinaryTree*> data_T = create_array_ar<FullBinaryTree*>(4);  

string g_file_path;