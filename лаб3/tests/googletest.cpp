#include <gtest/gtest.h>
#include "libraries.hpp"
#include <sstream>
#include <string>

// ======================== Array Tests ========================
TEST(ArrayTest, PushBackAndGet) {
    Array<int> arr;
    arr.push_back(5);
    arr.push_back(10);
    EXPECT_EQ(arr.get(1), 5);
    EXPECT_EQ(arr.get(2), 10);
}

TEST(ArrayTest, ConstructorWithCapacity) {
    Array<int> arr(10);
    arr.set(5, 42);
    EXPECT_EQ(arr.get(5), 42);
}

TEST(ArrayTest, SetAndRemove) {
    Array<int> arr;
    arr.set(3, 15);
    EXPECT_EQ(arr.get(3), 15);
    arr.remove(3);
    EXPECT_EQ(arr.get(3), 0);
}

TEST(ArrayTest, RemoveAtMaxIndex) {
    Array<int> arr;
    arr.push_back(1);
    arr.push_back(2);
    arr.push_back(3);
    arr.remove(3);
    arr.remove(2);
    arr.remove(1);
}

TEST(ArrayTest, Change) {
    Array<int> arr;
    arr.push_back(7);
    arr.change(1, 20);
    EXPECT_EQ(arr.get(1), 20);
}

TEST(ArrayTest, ResizeOnPushBack) {
    Array<int> arr;
    for (int i = 1; i <= 10; i++) {
        arr.push_back(i);
    }
    EXPECT_EQ(arr.get(10), 10);
}

TEST(ArrayTest, ResizeOnSet) {
    Array<int> arr;
    arr.set(20, 100);
    EXPECT_EQ(arr.get(20), 100);
}

TEST(ArrayTest, InvalidIndexGet) {
    Array<int> arr;
    arr.push_back(1);
    EXPECT_EQ(arr.get(0), 0);  // Invalid index
    EXPECT_EQ(arr.get(100), 0); // Out of bounds
}

TEST(ArrayTest, InvalidIndexSet) {
    Array<int> arr;
    arr.set(0, 5); // Should fail silently
    arr.set(-1, 5); // Should fail silently
}

TEST(ArrayTest, InvalidIndexChange) {
    Array<int> arr;
    arr.push_back(1);
    arr.change(0, 100); // Invalid
    arr.change(5, 100); // Out of bounds
}

TEST(ArrayTest, InvalidIndexRemove) {
    Array<int> arr;
    arr.push_back(1);
    arr.remove(0); // Invalid
    arr.remove(5); // Out of bounds
    arr.remove(1); // Valid
    arr.remove(1); // Already removed
}

TEST(ArrayTest, PrintEmpty) {
    Array<int> arr;
    std::ostringstream oss;
    std::streambuf* old = std::cout.rdbuf(oss.rdbuf());
    arr.print();
    std::cout.rdbuf(old);
    EXPECT_TRUE(oss.str().find("пуст") != std::string::npos);
}

TEST(ArrayTest, PrintWithData) {
    Array<int> arr;
    arr.push_back(1);
    arr.push_back(2);
    std::ostringstream oss;
    std::streambuf* old = std::cout.rdbuf(oss.rdbuf());
    arr.print();
    std::cout.rdbuf(old);
    EXPECT_FALSE(oss.str().empty());
}

// ======================== DoublyList Tests ========================
TEST(DoublyListTest, EmptyList) {
    DoublyList<int> list;
    EXPECT_EQ(list.get_head(), nullptr);
    EXPECT_EQ(list.get_tail(), nullptr);
}

TEST(DoublyListTest, PushFrontAndBack) {
    DoublyList<int> list;
    list.push_back(1);
    list.push_front(0);
    EXPECT_EQ(list.get_head()->data, 0);
    EXPECT_EQ(list.get_tail()->data, 1);
}

TEST(DoublyListTest, PushFrontOnly) {
    DoublyList<int> list;
    list.push_front(1);
    list.push_front(0);
    EXPECT_EQ(list.get_head()->data, 0);
    EXPECT_EQ(list.get_tail()->data, 1);
}

TEST(DoublyListTest, PushBackOnly) {
    DoublyList<int> list;
    list.push_back(1);
    list.push_back(2);
    EXPECT_EQ(list.get_head()->data, 1);
    EXPECT_EQ(list.get_tail()->data, 2);
}

TEST(DoublyListTest, InsertBefore) {
    DoublyList<int> list;
    list.push_back(1);
    list.push_back(3);
    auto node = list.find(3);
    list.insert_before(node, 2);
    EXPECT_EQ(list.find(2)->data, 2);
}

TEST(DoublyListTest, InsertBeforeHead) {
    DoublyList<int> list;
    list.push_back(2);
    auto node = list.find(2);
    list.insert_before(node, 1);
    EXPECT_EQ(list.get_head()->data, 1);
}

TEST(DoublyListTest, InsertAfter) {
    DoublyList<int> list;
    list.push_back(1);
    list.push_back(3);
    auto node = list.find(1);
    list.insert_after(node, 2);
    EXPECT_EQ(list.find(2)->data, 2);
}

TEST(DoublyListTest, InsertAfterTail) {
    DoublyList<int> list;
    list.push_back(1);
    auto node = list.find(1);
    list.insert_after(node, 2);
    EXPECT_EQ(list.get_tail()->data, 2);
}

TEST(DoublyListTest, InsertBeforeInvalidNode) {
    DoublyList<int> list;
    list.push_back(1);
    DoublyList<int> other;
    other.push_back(99);
    auto invalid_node = other.find(99);
    list.insert_before(invalid_node, 2); // Should fail
}

TEST(DoublyListTest, InsertAfterInvalidNode) {
    DoublyList<int> list;
    list.push_back(1);
    DoublyList<int> other;
    other.push_back(99);
    auto invalid_node = other.find(99);
    list.insert_after(invalid_node, 2); // Should fail
}

TEST(DoublyListTest, InsertBeforeNullNode) {
    DoublyList<int> list;
    list.insert_before(nullptr, 1); // Should fail
}

TEST(DoublyListTest, InsertAfterNullNode) {
    DoublyList<int> list;
    list.insert_after(nullptr, 1); // Should fail
}

TEST(DoublyListTest, PopFront) {
    DoublyList<int> list;
    list.push_back(1);
    list.push_back(2);
    list.pop_front();
    EXPECT_EQ(list.get_head()->data, 2);
}

TEST(DoublyListTest, PopFrontLast) {
    DoublyList<int> list;
    list.push_back(1);
    list.pop_front();
    EXPECT_EQ(list.get_head(), nullptr);
    EXPECT_EQ(list.get_tail(), nullptr);
}

TEST(DoublyListTest, PopFrontEmpty) {
    DoublyList<int> list;
    list.pop_front(); // Should fail silently
}

TEST(DoublyListTest, PopBack) {
    DoublyList<int> list;
    list.push_back(1);
    list.push_back(2);
    list.pop_back();
    EXPECT_EQ(list.get_tail()->data, 1);
}

TEST(DoublyListTest, PopBackLast) {
    DoublyList<int> list;
    list.push_back(1);
    list.pop_back();
    EXPECT_EQ(list.get_head(), nullptr);
    EXPECT_EQ(list.get_tail(), nullptr);
}

TEST(DoublyListTest, PopBackEmpty) {
    DoublyList<int> list;
    list.pop_back(); // Should fail silently
}

TEST(DoublyListTest, Erase) {
    DoublyList<int> list;
    list.push_back(1);
    list.push_back(2);
    list.push_back(3);
    auto node = list.find(2);
    list.erase(node);
    EXPECT_EQ(list.find(2), nullptr);
}

TEST(DoublyListTest, EraseHead) {
    DoublyList<int> list;
    list.push_back(1);
    list.push_back(2);
    auto node = list.get_head();
    list.erase(node);
    EXPECT_EQ(list.get_head()->data, 2);
}

TEST(DoublyListTest, EraseTail) {
    DoublyList<int> list;
    list.push_back(1);
    list.push_back(2);
    auto node = list.get_tail();
    list.erase(node);
    EXPECT_EQ(list.get_tail()->data, 1);
}

TEST(DoublyListTest, EraseNull) {
    DoublyList<int> list;
    list.erase(nullptr); // Should fail
}

TEST(DoublyListTest, EraseInvalidNode) {
    DoublyList<int> list;
    list.push_back(1);
    DoublyList<int> other;
    other.push_back(99);
    auto invalid = other.find(99);
    list.erase(invalid); // Should fail
}

TEST(DoublyListTest, DeleteValue) {
    DoublyList<int> list;
    list.push_back(1);
    list.push_back(3);
    auto node = list.find(3);
    list.insert_before(node, 2);
    EXPECT_EQ(list.find(2)->data, 2);
    list.delete_value(2);
    EXPECT_EQ(list.find(2), nullptr);
}

TEST(DoublyListTest, DeleteValueNotFound) {
    DoublyList<int> list;
    list.push_back(1);
    list.delete_value(99); // Should fail
}

TEST(DoublyListTest, DeleteBeforeValue) {
    DoublyList<int> list;
    list.push_back(1);
    list.push_back(2);
    list.push_back(3);
    list.delete_before_value(2);
    EXPECT_EQ(list.find(1), nullptr);
}

TEST(DoublyListTest, DeleteBeforeValueFirst) {
    DoublyList<int> list;
    list.push_back(1);
    list.delete_before_value(1); // Should fail
}

TEST(DoublyListTest, DeleteAfterValue) {
    DoublyList<int> list;
    list.push_back(1);
    list.push_back(2);
    list.push_back(3);
    list.delete_after_value(2);
    EXPECT_EQ(list.find(3), nullptr);
}

TEST(DoublyListTest, DeleteAfterValueLast) {
    DoublyList<int> list;
    list.push_back(1);
    list.delete_after_value(1); // Should fail
}

TEST(DoublyListTest, Find) {
    DoublyList<int> list;
    list.push_back(1);
    list.push_back(2);
    EXPECT_NE(list.find(1), nullptr);
    EXPECT_NE(list.find(2), nullptr);
    EXPECT_EQ(list.find(3), nullptr);
}

TEST(DoublyListTest, Print) {
    DoublyList<int> list;
    list.push_back(1);
    list.push_back(2);
    std::ostringstream oss;
    std::streambuf* old = std::cout.rdbuf(oss.rdbuf());
    list.print();
    std::cout.rdbuf(old);
    EXPECT_FALSE(oss.str().empty());
}

TEST(DoublyListTest, PrintEmpty) {
    DoublyList<int> list;
    std::ostringstream oss;
    std::streambuf* old = std::cout.rdbuf(oss.rdbuf());
    list.print();
    std::cout.rdbuf(old);
    EXPECT_TRUE(oss.str().find("пуст") != std::string::npos);
}

// ======================== ForwardList Tests ========================
TEST(ForwardListTest, PushAndFind) {
    ForwardList<int> list;
    list.push_back(1);
    list.push_front(0);
    EXPECT_EQ(list.find(0)->data, 0);
    EXPECT_EQ(list.find(1)->data, 1);
    EXPECT_EQ(list.find(2), nullptr);
}

TEST(ForwardListTest, PushFront) {
    ForwardList<int> list;
    list.push_front(2);
    list.push_front(1);
    EXPECT_EQ(list.get_head()->data, 1);
}

TEST(ForwardListTest, PushBack) {
    ForwardList<int> list;
    list.push_back(1);
    list.push_back(2);
    EXPECT_EQ(list.get_head()->data, 1);
    auto node = list.find(2);
    EXPECT_NE(node, nullptr);
}

TEST(ForwardListTest, InsertBefore) {
    ForwardList<int> list;
    list.push_back(1);
    list.push_back(3);
    auto node = list.find(3);
    list.insert_before(node, 2);
    EXPECT_NE(list.find(2), nullptr);
}

TEST(ForwardListTest, InsertBeforeHead) {
    ForwardList<int> list;
    list.push_back(2);
    auto node = list.find(2);
    list.insert_before(node, 1);
    EXPECT_EQ(list.get_head()->data, 1);
}

TEST(ForwardListTest, InsertBeforeInvalid) {
    ForwardList<int> list;
    list.push_back(1);
    ForwardList<int> other;
    other.push_back(99);
    auto invalid = other.find(99);
    list.insert_before(invalid, 2); // Should fail
}

TEST(ForwardListTest, InsertBeforeNull) {
    ForwardList<int> list;
    list.insert_before(nullptr, 1); // Should fail
}

TEST(ForwardListTest, InsertAfter) {
    ForwardList<int> list;
    list.push_back(1);
    list.push_back(3);
    auto node = list.find(1);
    list.insert_after(node, 2);
    EXPECT_NE(list.find(2), nullptr);
}

TEST(ForwardListTest, InsertAfterNull) {
    ForwardList<int> list;
    list.insert_after(nullptr, 1); // Should fail
}

TEST(ForwardListTest, PopFront) {
    ForwardList<int> list;
    list.push_back(1);
    list.push_back(2);
    list.pop_front();
    EXPECT_EQ(list.get_head()->data, 2);
}

TEST(ForwardListTest, PopFrontEmpty) {
    ForwardList<int> list;
    list.pop_front(); // Should do nothing
}

TEST(ForwardListTest, PopBack) {
    ForwardList<int> list;
    list.push_back(1);
    list.push_back(2);
    list.pop_back();
    EXPECT_EQ(list.find(2), nullptr);
}

TEST(ForwardListTest, PopBackSingle) {
    ForwardList<int> list;
    list.push_back(1);
    list.pop_back();
    EXPECT_EQ(list.get_head(), nullptr);
}

TEST(ForwardListTest, PopBackEmpty) {
    ForwardList<int> list;
    list.pop_back(); // Should do nothing
}

TEST(ForwardListTest, Erase) {
    ForwardList<int> list;
    list.push_back(1);
    list.push_back(2);
    list.push_back(3);
    auto node2 = list.find(2);
    list.erase(node2);
    EXPECT_EQ(list.find(2), nullptr);
}

TEST(ForwardListTest, EraseHead) {
    ForwardList<int> list;
    list.push_back(1);
    list.push_back(2);
    auto node = list.get_head();
    list.erase(node);
    EXPECT_EQ(list.get_head()->data, 2);
}

TEST(ForwardListTest, EraseNull) {
    ForwardList<int> list;
    list.erase(nullptr); // Should do nothing
}

TEST(ForwardListTest, EraseByValue) {
    ForwardList<int> list;
    list.push_back(1);
    list.push_back(2);
    list.push_back(3);
    list.erase_by_value(2);
    EXPECT_EQ(list.find(2), nullptr);
}

TEST(ForwardListTest, EraseByValueHead) {
    ForwardList<int> list;
    list.push_back(1);
    list.erase_by_value(1);
    EXPECT_EQ(list.get_head(), nullptr);
}

TEST(ForwardListTest, EraseBefore) {
    ForwardList<int> list;
    list.push_back(1);
    list.push_back(2);
    list.push_back(3);
    auto node = list.find(2);
    list.erase_before(node);
    EXPECT_EQ(list.find(1), nullptr);
}

TEST(ForwardListTest, EraseBeforeHead) {
    ForwardList<int> list;
    list.push_back(1);
    auto node = list.find(1);
    list.erase_before(node); // Should do nothing
}

TEST(ForwardListTest, EraseAfter) {
    ForwardList<int> list;
    list.push_back(1);
    list.push_back(2);
    list.push_back(3);
    auto node = list.find(1);
    list.erase_after(node);
    EXPECT_EQ(list.find(2), nullptr);
}

TEST(ForwardListTest, EraseAfterLast) {
    ForwardList<int> list;
    list.push_back(1);
    auto node = list.find(1);
    list.erase_after(node); // Should do nothing
}

TEST(ForwardListTest, Clear) {
    ForwardList<int> list;
    list.push_back(1);
    list.push_back(2);
    list.clear();
    EXPECT_EQ(list.get_head(), nullptr);
}

TEST(ForwardListTest, PrintWhile) {
    ForwardList<int> list;
    list.push_back(1);
    list.push_back(2);
    std::ostringstream oss;
    std::streambuf* old = std::cout.rdbuf(oss.rdbuf());
    list.print_while();
    std::cout.rdbuf(old);
    EXPECT_FALSE(oss.str().empty());
}

TEST(ForwardListTest, PrintFor) {
    ForwardList<int> list;
    list.push_back(1);
    list.push_back(2);
    std::ostringstream oss;
    std::streambuf* old = std::cout.rdbuf(oss.rdbuf());
    list.print_for();
    std::cout.rdbuf(old);
    EXPECT_FALSE(oss.str().empty());
}

TEST(ForwardListTest, PrintEmpty) {
    ForwardList<int> list;
    std::ostringstream oss;
    std::streambuf* old = std::cout.rdbuf(oss.rdbuf());
    list.print_while();
    std::cout.rdbuf(old);
    EXPECT_TRUE(oss.str().find("пуст") != std::string::npos);
}

// ======================== BST Tests ========================
TEST(BSTTest, EmptyTree) {
    BST tree;
    EXPECT_FALSE(tree.find(5));
}

TEST(BSTTest, InsertAndFind) {
    BST tree;
    tree.insert(5);
    tree.insert(3);
    tree.insert(7);
    EXPECT_TRUE(tree.find(5));
    EXPECT_TRUE(tree.find(3));
    EXPECT_TRUE(tree.find(7));
    EXPECT_FALSE(tree.find(10));
}

TEST(BSTTest, InsertDuplicate) {
    BST tree;
    tree.insert(5);
    tree.insert(5);
    EXPECT_TRUE(tree.find(5));
}

TEST(BSTTest, InsertMany) {
    BST tree;
    for (int i = 1; i <= 10; i++) {
        tree.insert(i);
    }
    for (int i = 1; i <= 10; i++) {
        EXPECT_TRUE(tree.find(i));
    }
}

TEST(BSTTest, FindNonExistent) {
    BST tree;
    tree.insert(5);
    EXPECT_FALSE(tree.find(10));
    EXPECT_FALSE(tree.find(1));
}

TEST(BSTTest, PrintEmpty) {
    BST tree;
    std::ostringstream oss;
    std::streambuf* old = std::cout.rdbuf(oss.rdbuf());
    tree.print_pretty();
    std::cout.rdbuf(old);
    EXPECT_TRUE(oss.str().find("пустое") != std::string::npos);
}

TEST(BSTTest, PrintWithData) {
    BST tree;
    tree.insert(5);
    tree.insert(3);
    tree.insert(7);
    std::ostringstream oss;
    std::streambuf* old = std::cout.rdbuf(oss.rdbuf());
    tree.print_pretty();
    std::cout.rdbuf(old);
    EXPECT_FALSE(oss.str().empty());
}

// ======================== HashTable Tests ========================
TEST(HashTableTest, Constructor) {
    HashTable<string, int> ht(32);
    EXPECT_EQ(ht.get_capacity(), 32);
    EXPECT_EQ(ht.get_size(), 0);
}

TEST(HashTableTest, InsertFindRemove) {
    HashTable<string, int> ht;
    ht.insert("a", 1);
    ht.insert("b", 2);
    int value;
    EXPECT_TRUE(ht.find("a", value));
    EXPECT_EQ(value, 1);
    ht.remove("a");
    EXPECT_FALSE(ht.find("a", value));
}

TEST(HashTableTest, InsertUpdate) {
    HashTable<string, int> ht;
    ht.insert("key", 1);
    ht.insert("key", 2); // Update
    int value;
    EXPECT_TRUE(ht.find("key", value));
    EXPECT_EQ(value, 2);
}

TEST(HashTableTest, InsertMany) {
    HashTable<string, int> ht;
    for (int i = 0; i < 20; i++) {
        ht.insert("key" + std::to_string(i), i);
    }
    EXPECT_EQ(ht.get_size(), 20);
}

TEST(HashTableTest, RemoveNonExistent) {
    HashTable<string, int> ht;
    ht.remove("nonexistent"); // Should fail silently
}

TEST(HashTableTest, FindNonExistent) {
    HashTable<string, int> ht;
    int value;
    EXPECT_FALSE(ht.find("nonexistent", value));
}

TEST(HashTableTest, CollisionHandling) {
    HashTable<int, int> ht(4); // Small capacity to force collisions
    for (int i = 0; i < 10; i++) {
        ht.insert(i, i * 10);
    }
    int value;
    for (int i = 0; i < 10; i++) {
        EXPECT_TRUE(ht.find(i, value));
        EXPECT_EQ(value, i * 10);
    }
}

TEST(HashTableTest, Print) {
    HashTable<string, int> ht;
    ht.insert("a", 1);
    ht.insert("b", 2);
    std::ostringstream oss;
    std::streambuf* old = std::cout.rdbuf(oss.rdbuf());
    ht.print();
    std::cout.rdbuf(old);
    EXPECT_FALSE(oss.str().empty());
}

// ======================== Queue Tests ========================
TEST(QueueTest, EmptyQueue) {
    Queue<int> q;
    EXPECT_TRUE(q.is_empty());
    EXPECT_EQ(q.get_size(), 0);
}

TEST(QueueTest, PushPop) {
    Queue<int> q;
    q.push(1);
    q.push(2);
    EXPECT_EQ(q.pop(), 1);
    EXPECT_EQ(q.pop(), 2);
    EXPECT_TRUE(q.is_empty());
}

TEST(QueueTest, PopEmpty) {
    Queue<int> q;
    int val = q.pop(); // Should return default
    (void)val;
}

TEST(QueueTest, Resize) {
    Queue<int> q;
    for (int i = 0; i < 10; i++) {
        q.push(i);
    }
    for (int i = 0; i < 10; i++) {
        EXPECT_EQ(q.pop(), i);
    }
}

TEST(QueueTest, CircularBehavior) {
    Queue<int> q;
    q.push(1);
    q.push(2);
    q.pop();
    q.push(3);
    EXPECT_EQ(q.pop(), 2);
    EXPECT_EQ(q.pop(), 3);
}

TEST(QueueTest, Print) {
    Queue<int> q;
    q.push(1);
    q.push(2);
    std::ostringstream oss;
    std::streambuf* old = std::cout.rdbuf(oss.rdbuf());
    q.print();
    std::cout.rdbuf(old);
    EXPECT_FALSE(oss.str().empty());
}

TEST(QueueTest, PrintEmpty) {
    Queue<int> q;
    std::ostringstream oss;
    std::streambuf* old = std::cout.rdbuf(oss.rdbuf());
    q.print();
    std::cout.rdbuf(old);
    EXPECT_TRUE(oss.str().find("пуста") != std::string::npos);
}

// ======================== Stack Tests ========================
TEST(StackTest, EmptyStack) {
    Stack<int> s;
    EXPECT_TRUE(s.is_empty());
    EXPECT_EQ(s.size(), 0);
}

TEST(StackTest, PushPopPeek) {
    Stack<int> s;
    s.push(1);
    s.push(2);
    EXPECT_EQ(s.peek(), 2);
    EXPECT_EQ(s.pop(), 2);
    EXPECT_EQ(s.pop(), 1);
    EXPECT_TRUE(s.is_empty());
}

TEST(StackTest, PopEmpty) {
    Stack<int> s;
    int val = s.pop(); // Should return default
    (void)val;
}

TEST(StackTest, PeekEmpty) {
    Stack<int> s;
    int val = s.peek(); // Should return default
    (void)val;
}

TEST(StackTest, CopyFrom) {
    Stack<int> s1;
    s1.push(1);
    s1.push(2);
    Stack<int> s2;
    s2.copy_from(s1);
    EXPECT_EQ(s2.pop(), 2);
    EXPECT_EQ(s2.pop(), 1);
    EXPECT_FALSE(s1.is_empty()); // Original unchanged
}

TEST(StackTest, CopyFromEmpty) {
    Stack<int> s1;
    Stack<int> s2;
    s2.push(99);
    s2.copy_from(s1);
    EXPECT_TRUE(s2.is_empty());
}

TEST(StackTest, Clear) {
    Stack<int> s;
    s.push(1);
    s.push(2);
    s.clear();
    EXPECT_TRUE(s.is_empty());
    EXPECT_EQ(s.size(), 0);
}

TEST(StackTest, Print) {
    Stack<int> s;
    s.push(1);
    s.push(2);
    std::ostringstream oss;
    std::streambuf* old = std::cout.rdbuf(oss.rdbuf());
    s.print();
    std::cout.rdbuf(old);
    EXPECT_FALSE(oss.str().empty());
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    int test_count = ::testing::UnitTest::GetInstance()->total_test_count();
    std::cout << "Всего тестов зарегистрировано: " << test_count << std::endl;
    return RUN_ALL_TESTS();
}
