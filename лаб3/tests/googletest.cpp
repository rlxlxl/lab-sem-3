#include <gtest/gtest.h>
#include "libraries.hpp"
// ------------------------ Array ------------------------
TEST(ArrayTest, PushBackAndGet) {
Array<int> arr;
arr.push_back(5);
arr.push_back(10);
EXPECT_EQ(arr.get(1), 5);
EXPECT_EQ(arr.get(2), 10);
}

TEST(ArrayTest, SetAndRemove) {
Array<int> arr;
arr.set(3, 15);
EXPECT_EQ(arr.get(3), 15);
arr.remove(3);
EXPECT_EQ(arr.get(3), 0);
}

TEST(ArrayTest, Change) {
Array<int> arr;
arr.push_back(7);
arr.change(1, 20);
EXPECT_EQ(arr.get(1), 20);
}

TEST(ArrayTest, InvalidIndex) {
Array<int> arr;
arr.push_back(1);
EXPECT_NO_FATAL_FAILURE(arr.get(0));
EXPECT_NO_FATAL_FAILURE(arr.change(5, 100));
EXPECT_NO_FATAL_FAILURE(arr.remove(5));
}

// ------------------------ DoublyList ------------------------
TEST(DoublyListTest, PushFrontAndBack) {
DoublyList<int> list;
list.push_back(1);
list.push_front(0);
EXPECT_EQ(list.get_head()->data, 0);
EXPECT_EQ(list.get_tail()->data, 1);
}

TEST(DoublyListTest, InsertAndDelete) {
DoublyList<int> list;
list.push_back(1);
list.push_back(3);
auto node = list.find(3);
list.insert_before(node, 2);
EXPECT_EQ(list.find(2)->data, 2);
list.delete_value(2);
EXPECT_EQ(list.find(2), nullptr);
}

// ------------------------ ForwardList ------------------------
TEST(ForwardListTest, PushAndFind) {
ForwardList<int> list;
list.push_back(1);
list.push_front(0);
EXPECT_EQ(list.find(0)->data, 0);
EXPECT_EQ(list.find(1)->data, 1);
EXPECT_EQ(list.find(2), nullptr);
}

TEST(ForwardListTest, EraseOperations) {
ForwardList<int> list;
list.push_back(1);
list.push_back(2);
list.push_back(3);
auto node2 = list.find(2);
list.erase(node2);
EXPECT_EQ(list.find(2), nullptr);
list.erase_by_value(1);
EXPECT_EQ(list.find(1), nullptr);
}

// ------------------------ BST ------------------------
TEST(BSTTest, InsertAndFind) {
BST tree;
tree.insert(5);
tree.insert(3);
tree.insert(7);
EXPECT_TRUE(tree.find(5));
EXPECT_TRUE(tree.find(3));
EXPECT_FALSE(tree.find(10));
}

// ------------------------ HashTable ------------------------
TEST(HashTableTest, InsertFindRemove) {
HashTable<string,int> ht;
ht.insert("a", 1);
ht.insert("b", 2);
int value;
EXPECT_TRUE(ht.find("a", value));
EXPECT_EQ(value, 1);
ht.remove("a");
EXPECT_FALSE(ht.find("a", value));
}

// ------------------------ Queue ------------------------
TEST(QueueTest, PushPop) {
Queue<int> q;
q.push(1);
q.push(2);
EXPECT_EQ(q.pop(), 1);
EXPECT_EQ(q.pop(), 2);
EXPECT_TRUE(q.is_empty());
}

// ------------------------ Stack ------------------------
TEST(StackTest, PushPopPeek) {
Stack<int> s;
s.push(1);
s.push(2);
EXPECT_EQ(s.peek(), 2);
EXPECT_EQ(s.pop(), 2);
EXPECT_EQ(s.pop(), 1);
EXPECT_TRUE(s.is_empty());
}

TEST(StackTest, CopyFrom) {
Stack<int> s1;
s1.push(1);
s1.push(2);
Stack<int> s2;
s2.copy_from(s1);
EXPECT_EQ(s2.pop(), 2);
EXPECT_EQ(s2.pop(), 1);
}

int main(int argc, char **argv) {
::testing::InitGoogleTest(&argc, argv);
int test_count = ::testing::UnitTest::GetInstance()->total_test_count();
    std::cout << "Всего тестов зарегистрировано: " << test_count << std::endl;
return RUN_ALL_TESTS();
}
