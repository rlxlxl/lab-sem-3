#define BOOST_TEST_MODULE DataStructuresTest
#include <boost/test/unit_test.hpp>
#include "libraries.hpp"
#include <sstream>
#include <string>
#include <vector>

// ======================== Array Tests ========================
BOOST_AUTO_TEST_SUITE(ArrayTest)

BOOST_AUTO_TEST_CASE(TestEmptyArray) {
    Array<int> arr;
    BOOST_CHECK_EQUAL(arr.get(1), 0);
}

BOOST_AUTO_TEST_CASE(TestMultiplePushBack) {
    Array<int> arr;
    for (int i = 1; i <= 100; i++) {
        arr.push_back(i);
    }
    BOOST_CHECK_EQUAL(arr.get(100), 100);
    BOOST_CHECK_EQUAL(arr.get(1), 1);
}

BOOST_AUTO_TEST_CASE(TestSetWithLargeIndex) {
    Array<int> arr;
    arr.set(1000, 999);
    BOOST_CHECK_EQUAL(arr.get(1000), 999);
}

BOOST_AUTO_TEST_CASE(TestResizeMultipleTimes) {
    Array<int> arr;
    for (int i = 1; i <= 200; i++) {
        arr.push_back(i);
    }
    BOOST_CHECK_EQUAL(arr.get(200), 200);
}

BOOST_AUTO_TEST_CASE(TestRemoveSequence) {
    Array<int> arr;
    for (int i = 1; i <= 10; i++) {
        arr.push_back(i);
    }
    for (int i = 1; i <= 10; i++) {
        arr.remove(i);
    }
    // Проверяем, что все элементы удалены
    for (int i = 1; i <= 10; i++) {
        BOOST_CHECK_EQUAL(arr.get(i), 0);
    }
}

BOOST_AUTO_TEST_CASE(TestChangeMultipleTimes) {
    Array<int> arr;
    arr.push_back(1);
    arr.change(1, 10);
    BOOST_CHECK_EQUAL(arr.get(1), 10);
    arr.change(1, 20);
    BOOST_CHECK_EQUAL(arr.get(1), 20);
    arr.change(1, 30);
    BOOST_CHECK_EQUAL(arr.get(1), 30);
}

BOOST_AUTO_TEST_CASE(TestConstructorWithZeroCapacity) {
    Array<int> arr(0);
    arr.push_back(1);
    BOOST_CHECK_EQUAL(arr.get(1), 1);
}

BOOST_AUTO_TEST_CASE(TestConstructorWithLargeCapacity) {
    Array<int> arr(1000);
    arr.set(500, 42);
    BOOST_CHECK_EQUAL(arr.get(500), 42);
}

BOOST_AUTO_TEST_CASE(TestRemoveUnsetElement) {
    Array<int> arr;
    arr.set(5, 10);
    arr.remove(5);
    arr.remove(5); // Попытка удалить уже удаленный элемент
    BOOST_CHECK_EQUAL(arr.get(5), 0);
}

BOOST_AUTO_TEST_CASE(TestGetUnsetElement) {
    Array<int> arr;
    arr.push_back(1);
    BOOST_CHECK_EQUAL(arr.get(5), 0); // Не установленный элемент
}

BOOST_AUTO_TEST_SUITE_END()

// ======================== DoublyList Tests ========================
BOOST_AUTO_TEST_SUITE(DoublyListTest)

BOOST_AUTO_TEST_CASE(TestMultiplePushOperations) {
    DoublyList<int> list;
    for (int i = 1; i <= 100; i++) {
        list.push_back(i);
    }
    BOOST_CHECK_EQUAL(list.get_tail()->data, 100);
    BOOST_CHECK_EQUAL(list.get_head()->data, 1);
}

BOOST_AUTO_TEST_CASE(TestMultiplePushFront) {
    DoublyList<int> list;
    for (int i = 100; i >= 1; i--) {
        list.push_front(i);
    }
    BOOST_CHECK_EQUAL(list.get_head()->data, 1);
    BOOST_CHECK_EQUAL(list.get_tail()->data, 100);
}

BOOST_AUTO_TEST_CASE(TestInsertBeforeMiddle) {
    DoublyList<int> list;
    list.push_back(1);
    list.push_back(3);
    list.push_back(4);
    auto node = list.find(3);
    list.insert_before(node, 2);
    auto node2 = list.find(2);
    BOOST_CHECK_NE(node2, nullptr);
    BOOST_CHECK_EQUAL(node2->data, 2);
}

BOOST_AUTO_TEST_CASE(TestInsertAfterMiddle) {
    DoublyList<int> list;
    list.push_back(1);
    list.push_back(2);
    list.push_back(4);
    auto node = list.find(2);
    list.insert_after(node, 3);
    auto node3 = list.find(3);
    BOOST_CHECK_NE(node3, nullptr);
    BOOST_CHECK_EQUAL(node3->data, 3);
}

BOOST_AUTO_TEST_CASE(TestDeleteMultipleValues) {
    DoublyList<int> list;
    for (int i = 1; i <= 10; i++) {
        list.push_back(i);
    }
    for (int i = 1; i <= 5; i++) {
        list.delete_value(i);
    }
    for (int i = 1; i <= 5; i++) {
        BOOST_CHECK_EQUAL(list.find(i), nullptr);
    }
    for (int i = 6; i <= 10; i++) {
        BOOST_CHECK_NE(list.find(i), nullptr);
    }
}

BOOST_AUTO_TEST_CASE(TestDeleteBeforeMiddle) {
    DoublyList<int> list;
    list.push_back(1);
    list.push_back(2);
    list.push_back(3);
    list.push_back(4);
    list.delete_before_value(3);
    BOOST_CHECK_EQUAL(list.find(2), nullptr);
    BOOST_CHECK_NE(list.find(1), nullptr);
    BOOST_CHECK_NE(list.find(3), nullptr);
}

BOOST_AUTO_TEST_CASE(TestDeleteAfterMiddle) {
    DoublyList<int> list;
    list.push_back(1);
    list.push_back(2);
    list.push_back(3);
    list.push_back(4);
    list.delete_after_value(2);
    BOOST_CHECK_EQUAL(list.find(3), nullptr);
    BOOST_CHECK_NE(list.find(2), nullptr);
    BOOST_CHECK_NE(list.find(4), nullptr);
}

BOOST_AUTO_TEST_CASE(TestEraseMultiple) {
    DoublyList<int> list;
    for (int i = 1; i <= 10; i++) {
        list.push_back(i);
    }
    // Используем delete_value вместо прямого доступа к Node
    for (int i = 2; i <= 9; i++) {
        list.delete_value(i);
    }
    BOOST_CHECK_NE(list.find(1), nullptr);
    BOOST_CHECK_NE(list.find(10), nullptr);
    for (int i = 2; i <= 9; i++) {
        BOOST_CHECK_EQUAL(list.find(i), nullptr);
    }
}

BOOST_AUTO_TEST_CASE(TestPopAllElements) {
    DoublyList<int> list;
    for (int i = 1; i <= 10; i++) {
        list.push_back(i);
    }
    for (int i = 0; i < 10; i++) {
        list.pop_front();
    }
    BOOST_CHECK_EQUAL(list.get_head(), nullptr);
    BOOST_CHECK_EQUAL(list.get_tail(), nullptr);
}

BOOST_AUTO_TEST_SUITE_END()

// ======================== ForwardList Tests ========================
BOOST_AUTO_TEST_SUITE(ForwardListTest)

BOOST_AUTO_TEST_CASE(TestMultiplePushBack) {
    ForwardList<int> list;
    for (int i = 1; i <= 100; i++) {
        list.push_back(i);
    }
    auto node = list.find(100);
    BOOST_CHECK_NE(node, nullptr);
    BOOST_CHECK_EQUAL(node->data, 100);
}

BOOST_AUTO_TEST_CASE(TestMultiplePushFront) {
    ForwardList<int> list;
    for (int i = 100; i >= 1; i--) {
        list.push_front(i);
    }
    BOOST_CHECK_EQUAL(list.get_head()->data, 1);
    auto node = list.find(100);
    BOOST_CHECK_NE(node, nullptr);
}

BOOST_AUTO_TEST_CASE(TestInsertBeforeMultiple) {
    ForwardList<int> list;
    list.push_back(1);
    list.push_back(5);
    auto node = list.find(5);
    list.insert_before(node, 2);
    list.insert_before(node, 3);
    list.insert_before(node, 4);
    BOOST_CHECK_NE(list.find(2), nullptr);
    BOOST_CHECK_NE(list.find(3), nullptr);
    BOOST_CHECK_NE(list.find(4), nullptr);
}

BOOST_AUTO_TEST_CASE(TestInsertAfterMultiple) {
    ForwardList<int> list;
    list.push_back(1);
    auto node = list.find(1);
    list.insert_after(node, 2);
    auto node2 = list.find(2);
    list.insert_after(node2, 3);
    auto node3 = list.find(3);
    list.insert_after(node3, 4);
    BOOST_CHECK_NE(list.find(4), nullptr);
}

BOOST_AUTO_TEST_CASE(TestEraseByValueMultiple) {
    ForwardList<int> list;
    for (int i = 1; i <= 10; i++) {
        list.push_back(i);
    }
    for (int i = 2; i <= 9; i++) {
        list.erase_by_value(i);
    }
    BOOST_CHECK_NE(list.find(1), nullptr);
    BOOST_CHECK_NE(list.find(10), nullptr);
    for (int i = 2; i <= 9; i++) {
        BOOST_CHECK_EQUAL(list.find(i), nullptr);
    }
}

BOOST_AUTO_TEST_CASE(TestEraseBeforeMultiple) {
    ForwardList<int> list;
    for (int i = 1; i <= 10; i++) {
        list.push_back(i);
    }
    auto node5 = list.find(5);
    list.erase_before(node5);
    auto node7 = list.find(7);
    list.erase_before(node7);
    BOOST_CHECK_EQUAL(list.find(4), nullptr);
    BOOST_CHECK_EQUAL(list.find(6), nullptr);
}

BOOST_AUTO_TEST_CASE(TestEraseAfterMultiple) {
    ForwardList<int> list;
    for (int i = 1; i <= 10; i++) {
        list.push_back(i);
    }
    auto node2 = list.find(2);
    list.erase_after(node2);
    auto node4 = list.find(4);
    list.erase_after(node4);
    BOOST_CHECK_EQUAL(list.find(3), nullptr);
    BOOST_CHECK_EQUAL(list.find(5), nullptr);
}

BOOST_AUTO_TEST_CASE(TestPopAll) {
    ForwardList<int> list;
    for (int i = 1; i <= 10; i++) {
        list.push_back(i);
    }
    for (int i = 0; i < 10; i++) {
        list.pop_front();
    }
    BOOST_CHECK_EQUAL(list.get_head(), nullptr);
}

BOOST_AUTO_TEST_CASE(TestClearAfterOperations) {
    ForwardList<int> list;
    for (int i = 1; i <= 100; i++) {
        list.push_back(i);
    }
    list.clear();
    BOOST_CHECK_EQUAL(list.get_head(), nullptr);
    // Проверяем, что можем использовать после clear
    list.push_back(1);
    BOOST_CHECK_NE(list.get_head(), nullptr);
}

BOOST_AUTO_TEST_SUITE_END()

// ======================== BST Tests ========================
BOOST_AUTO_TEST_SUITE(BSTTest)

BOOST_AUTO_TEST_CASE(TestInsertSequential) {
    BST tree;
    for (int i = 1; i <= 100; i++) {
        tree.insert(i);
    }
    for (int i = 1; i <= 100; i++) {
        BOOST_CHECK(tree.find(i));
    }
}

BOOST_AUTO_TEST_CASE(TestInsertReverseOrder) {
    BST tree;
    for (int i = 100; i >= 1; i--) {
        tree.insert(i);
    }
    for (int i = 1; i <= 100; i++) {
        BOOST_CHECK(tree.find(i));
    }
}

BOOST_AUTO_TEST_CASE(TestInsertBalanced) {
    BST tree;
    // Вставляем в порядке, который создает более сбалансированное дерево
    std::vector<int> values = {50, 25, 75, 12, 37, 62, 87, 6, 18, 31, 43};
    for (int val : values) {
        tree.insert(val);
    }
    for (int val : values) {
        BOOST_CHECK(tree.find(val));
    }
}

BOOST_AUTO_TEST_CASE(TestFindAllElements) {
    BST tree;
    std::vector<int> values;
    for (int i = 0; i < 50; i++) {
        int val = i * 7 + 3;
        values.push_back(val);
        tree.insert(val);
    }
    for (int val : values) {
        BOOST_CHECK(tree.find(val));
    }
}

BOOST_AUTO_TEST_CASE(TestFindNonExistentMany) {
    BST tree;
    for (int i = 1; i <= 50; i++) {
        tree.insert(i * 2); // Только четные числа
    }
    for (int i = 1; i <= 50; i++) {
        BOOST_CHECK(!tree.find(i * 2 - 1)); // Нечетные не должны быть найдены
    }
}

BOOST_AUTO_TEST_SUITE_END()

// ======================== HashTable Tests ========================
BOOST_AUTO_TEST_SUITE(HashTableTest)

BOOST_AUTO_TEST_CASE(TestInsertLargeSequence) {
    HashTable<std::string, int> ht;
    for (int i = 0; i < 1000; i++) {
        ht.insert("key" + std::to_string(i), i);
    }
    BOOST_CHECK_EQUAL(ht.get_size(), 1000);
    int value;
    for (int i = 0; i < 1000; i++) {
        BOOST_CHECK(ht.find("key" + std::to_string(i), value));
        BOOST_CHECK_EQUAL(value, i);
    }
}

BOOST_AUTO_TEST_CASE(TestInsertUpdateMany) {
    HashTable<std::string, int> ht;
    for (int i = 0; i < 100; i++) {
        ht.insert("key", i); // Многократное обновление одного ключа
    }
    int value;
    BOOST_CHECK(ht.find("key", value));
    BOOST_CHECK_EQUAL(value, 99); // Последнее значение
    BOOST_CHECK_EQUAL(ht.get_size(), 1);
}

BOOST_AUTO_TEST_CASE(TestRemoveSequence) {
    HashTable<std::string, int> ht;
    for (int i = 0; i < 100; i++) {
        ht.insert("key" + std::to_string(i), i);
    }
    for (int i = 0; i < 50; i++) {
        ht.remove("key" + std::to_string(i));
    }
    BOOST_CHECK_EQUAL(ht.get_size(), 50);
    int value;
    for (int i = 0; i < 50; i++) {
        BOOST_CHECK(!ht.find("key" + std::to_string(i), value));
    }
    for (int i = 50; i < 100; i++) {
        BOOST_CHECK(ht.find("key" + std::to_string(i), value));
    }
}

BOOST_AUTO_TEST_CASE(TestCollisionChain) {
    HashTable<int, int> ht(8); // Маленькая емкость для коллизий
    for (int i = 0; i < 100; i++) {
        ht.insert(i, i * 10);
    }
    BOOST_CHECK_EQUAL(ht.get_size(), 100);
    int value;
    for (int i = 0; i < 100; i++) {
        BOOST_CHECK(ht.find(i, value));
        BOOST_CHECK_EQUAL(value, i * 10);
    }
}

BOOST_AUTO_TEST_CASE(TestDifferentTypes) {
    HashTable<int, std::string> ht;
    ht.insert(1, "one");
    ht.insert(2, "two");
    ht.insert(3, "three");
    std::string value;
    BOOST_CHECK(ht.find(1, value));
    BOOST_CHECK_EQUAL(value, "one");
    BOOST_CHECK(ht.find(2, value));
    BOOST_CHECK_EQUAL(value, "two");
}

BOOST_AUTO_TEST_CASE(TestCustomCapacity) {
    HashTable<std::string, int> ht(64);
    BOOST_CHECK_EQUAL(ht.get_capacity(), 64);
    for (int i = 0; i < 200; i++) {
        ht.insert("key" + std::to_string(i), i);
    }
    BOOST_CHECK_EQUAL(ht.get_size(), 200);
}

BOOST_AUTO_TEST_SUITE_END()

// ======================== Queue Tests ========================
BOOST_AUTO_TEST_SUITE(QueueTest)

BOOST_AUTO_TEST_CASE(TestLargePushPop) {
    Queue<int> q;
    for (int i = 0; i < 1000; i++) {
        q.push(i);
    }
    BOOST_CHECK_EQUAL(q.get_size(), 1000);
    for (int i = 0; i < 1000; i++) {
        BOOST_CHECK_EQUAL(q.pop(), i);
    }
    BOOST_CHECK(q.is_empty());
}

BOOST_AUTO_TEST_CASE(TestCircularWrapAround) {
    Queue<int> q;
    // Заполняем до края
    for (int i = 0; i < 10; i++) {
        q.push(i);
    }
    // Удаляем несколько
    for (int i = 0; i < 5; i++) {
        q.pop();
    }
    // Добавляем еще - должно использовать освободившееся место
    for (int i = 10; i < 20; i++) {
        q.push(i);
    }
    BOOST_CHECK_EQUAL(q.get_size(), 15);
    // Проверяем порядок
    for (int i = 5; i < 20; i++) {
        BOOST_CHECK_EQUAL(q.pop(), i);
    }
}

BOOST_AUTO_TEST_CASE(TestMultipleResize) {
    Queue<int> q;
    // Добавляем много элементов, чтобы вызвать несколько resize
    for (int i = 0; i < 10000; i++) {
        q.push(i);
    }
    BOOST_CHECK_EQUAL(q.get_size(), 10000);
    for (int i = 0; i < 10000; i++) {
        BOOST_CHECK_EQUAL(q.pop(), i);
    }
}

BOOST_AUTO_TEST_CASE(TestInterleavedPushPop) {
    Queue<int> q;
    q.push(1);
    q.push(2);
    BOOST_CHECK_EQUAL(q.pop(), 1);
    q.push(3);
    q.push(4);
    BOOST_CHECK_EQUAL(q.pop(), 2);
    BOOST_CHECK_EQUAL(q.pop(), 3);
    q.push(5);
    BOOST_CHECK_EQUAL(q.pop(), 4);
    BOOST_CHECK_EQUAL(q.pop(), 5);
    BOOST_CHECK(q.is_empty());
}

BOOST_AUTO_TEST_CASE(TestSizeAfterOperations) {
    Queue<int> q;
    BOOST_CHECK_EQUAL(q.get_size(), 0);
    q.push(1);
    BOOST_CHECK_EQUAL(q.get_size(), 1);
    q.push(2);
    BOOST_CHECK_EQUAL(q.get_size(), 2);
    q.pop();
    BOOST_CHECK_EQUAL(q.get_size(), 1);
    q.pop();
    BOOST_CHECK_EQUAL(q.get_size(), 0);
}

BOOST_AUTO_TEST_SUITE_END()

// ======================== Stack Tests ========================
BOOST_AUTO_TEST_SUITE(StackTest)

BOOST_AUTO_TEST_CASE(TestLargePushPop) {
    Stack<int> s;
    for (int i = 0; i < 1000; i++) {
        s.push(i);
    }
    BOOST_CHECK_EQUAL(s.size(), 1000);
    for (int i = 999; i >= 0; i--) {
        BOOST_CHECK_EQUAL(s.pop(), i);
    }
    BOOST_CHECK(s.is_empty());
}

BOOST_AUTO_TEST_CASE(TestPeekDoesNotRemove) {
    Stack<int> s;
    s.push(1);
    s.push(2);
    s.push(3);
    BOOST_CHECK_EQUAL(s.peek(), 3);
    BOOST_CHECK_EQUAL(s.size(), 3);
    BOOST_CHECK_EQUAL(s.peek(), 3); // Должно быть то же значение
    BOOST_CHECK_EQUAL(s.size(), 3); // Размер не изменился
}

BOOST_AUTO_TEST_CASE(TestCopyFromLargeStack) {
    Stack<int> s1;
    for (int i = 0; i < 100; i++) {
        s1.push(i);
    }
    Stack<int> s2;
    s2.copy_from(s1);
    BOOST_CHECK_EQUAL(s2.size(), 100);
    BOOST_CHECK_EQUAL(s1.size(), 100); // Оригинал не изменился
    for (int i = 99; i >= 0; i--) {
        BOOST_CHECK_EQUAL(s2.pop(), i);
    }
    // Оригинал все еще полный
    BOOST_CHECK_EQUAL(s1.size(), 100);
}

BOOST_AUTO_TEST_CASE(TestCopyFromEmptyStack) {
    Stack<int> s1;
    Stack<int> s2;
    s2.push(1);
    s2.push(2);
    s2.copy_from(s1);
    BOOST_CHECK(s2.is_empty());
}

BOOST_AUTO_TEST_CASE(TestMultipleCopyFrom) {
    Stack<int> s1;
    s1.push(1);
    Stack<int> s2;
    s2.push(99);
    s2.copy_from(s1);
    BOOST_CHECK_EQUAL(s2.pop(), 1);
    
    s1.push(2);
    s1.push(3);
    s2.copy_from(s1);
    // s1 теперь содержит [1, 2, 3] (сверху вниз), поэтому размер должен быть 3
    BOOST_CHECK_EQUAL(s2.size(), 3);
    BOOST_CHECK_EQUAL(s2.pop(), 3);
    BOOST_CHECK_EQUAL(s2.pop(), 2);
    BOOST_CHECK_EQUAL(s2.pop(), 1);
}

BOOST_AUTO_TEST_CASE(TestClearAndReuse) {
    Stack<int> s;
    for (int i = 0; i < 100; i++) {
        s.push(i);
    }
    s.clear();
    BOOST_CHECK(s.is_empty());
    BOOST_CHECK_EQUAL(s.size(), 0);
    // Используем после clear
    s.push(1);
    s.push(2);
    BOOST_CHECK_EQUAL(s.size(), 2);
    BOOST_CHECK_EQUAL(s.pop(), 2);
}

BOOST_AUTO_TEST_CASE(TestInterleavedOperations) {
    Stack<int> s;
    s.push(1);
    BOOST_CHECK_EQUAL(s.peek(), 1);
    s.push(2);
    BOOST_CHECK_EQUAL(s.peek(), 2);
    BOOST_CHECK_EQUAL(s.pop(), 2);
    s.push(3);
    BOOST_CHECK_EQUAL(s.peek(), 3);
    BOOST_CHECK_EQUAL(s.pop(), 3);
    BOOST_CHECK_EQUAL(s.pop(), 1);
    BOOST_CHECK(s.is_empty());
}

BOOST_AUTO_TEST_SUITE_END()

