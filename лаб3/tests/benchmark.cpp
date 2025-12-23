#include <benchmark/benchmark.h>
#include "libraries.hpp"
#include <vector>
#include <random>

// ======================== Array Benchmarks ========================
static void BM_Array_PushBack(benchmark::State& state) {
    for (auto _ : state) {
        Array<int> arr;
        for (int i = 0; i < state.range(0); i++) {
            arr.push_back(i);
        }
    }
}
BENCHMARK(BM_Array_PushBack)->Range(8, 8<<10);

static void BM_Array_Set(benchmark::State& state) {
    for (auto _ : state) {
        Array<int> arr;
        for (int i = 1; i <= state.range(0); i++) {
            arr.set(i, i * 2);
        }
    }
}
BENCHMARK(BM_Array_Set)->Range(8, 8<<10);

static void BM_Array_Get(benchmark::State& state) {
    Array<int> arr;
    for (int i = 1; i <= 1000; i++) {
        arr.push_back(i);
    }
    for (auto _ : state) {
        benchmark::DoNotOptimize(arr.get(500));
    }
}
BENCHMARK(BM_Array_Get);

static void BM_Array_Remove(benchmark::State& state) {
    for (auto _ : state) {
        Array<int> arr;
        for (int i = 1; i <= 1000; i++) {
            arr.push_back(i);
        }
        for (int i = 1; i <= state.range(0); i++) {
            arr.remove(i);
        }
    }
}
BENCHMARK(BM_Array_Remove)->Range(8, 256);

// ======================== DoublyList Benchmarks ========================
static void BM_DoublyList_PushBack(benchmark::State& state) {
    for (auto _ : state) {
        DoublyList<int> list;
        for (int i = 0; i < state.range(0); i++) {
            list.push_back(i);
        }
    }
}
BENCHMARK(BM_DoublyList_PushBack)->Range(8, 8<<10);

static void BM_DoublyList_PushFront(benchmark::State& state) {
    for (auto _ : state) {
        DoublyList<int> list;
        for (int i = 0; i < state.range(0); i++) {
            list.push_front(i);
        }
    }
}
BENCHMARK(BM_DoublyList_PushFront)->Range(8, 8<<10);

static void BM_DoublyList_Find(benchmark::State& state) {
    DoublyList<int> list;
    for (int i = 0; i < 1000; i++) {
        list.push_back(i);
    }
    for (auto _ : state) {
        benchmark::DoNotOptimize(list.find(500));
    }
}
BENCHMARK(BM_DoublyList_Find);

static void BM_DoublyList_InsertBefore(benchmark::State& state) {
    for (auto _ : state) {
        DoublyList<int> list;
        list.push_back(0);
        list.push_back(1000);
        auto node = list.find(1000);
        for (int i = 1; i < state.range(0); i++) {
            list.insert_before(node, i);
        }
    }
}
BENCHMARK(BM_DoublyList_InsertBefore)->Range(8, 256);

static void BM_DoublyList_DeleteValue(benchmark::State& state) {
    for (auto _ : state) {
        DoublyList<int> list;
        for (int i = 0; i < state.range(0); i++) {
            list.push_back(i);
        }
        for (int i = 0; i < state.range(0) / 2; i++) {
            list.delete_value(i);
        }
    }
}
BENCHMARK(BM_DoublyList_DeleteValue)->Range(8, 256);

// ======================== ForwardList Benchmarks ========================
static void BM_ForwardList_PushBack(benchmark::State& state) {
    for (auto _ : state) {
        ForwardList<int> list;
        for (int i = 0; i < state.range(0); i++) {
            list.push_back(i);
        }
    }
}
BENCHMARK(BM_ForwardList_PushBack)->Range(8, 8<<10);

static void BM_ForwardList_PushFront(benchmark::State& state) {
    for (auto _ : state) {
        ForwardList<int> list;
        for (int i = 0; i < state.range(0); i++) {
            list.push_front(i);
        }
    }
}
BENCHMARK(BM_ForwardList_PushFront)->Range(8, 8<<10);

static void BM_ForwardList_Find(benchmark::State& state) {
    ForwardList<int> list;
    for (int i = 0; i < 1000; i++) {
        list.push_back(i);
    }
    for (auto _ : state) {
        benchmark::DoNotOptimize(list.find(500));
    }
}
BENCHMARK(BM_ForwardList_Find);

static void BM_ForwardList_EraseByValue(benchmark::State& state) {
    for (auto _ : state) {
        ForwardList<int> list;
        for (int i = 0; i < state.range(0); i++) {
            list.push_back(i);
        }
        for (int i = 0; i < state.range(0) / 2; i++) {
            list.erase_by_value(i);
        }
    }
}
BENCHMARK(BM_ForwardList_EraseByValue)->Range(8, 256);

// ======================== BST Benchmarks ========================
static void BM_BST_Insert(benchmark::State& state) {
    for (auto _ : state) {
        BST tree;
        for (int i = 0; i < state.range(0); i++) {
            tree.insert(i);
        }
    }
}
BENCHMARK(BM_BST_Insert)->Range(8, 8<<10);

static void BM_BST_InsertRandom(benchmark::State& state) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(1, 1000000);
    
    for (auto _ : state) {
        BST tree;
        for (int i = 0; i < state.range(0); i++) {
            tree.insert(dis(gen));
        }
    }
}
BENCHMARK(BM_BST_InsertRandom)->Range(8, 8<<10);

static void BM_BST_Find(benchmark::State& state) {
    BST tree;
    for (int i = 0; i < 1000; i++) {
        tree.insert(i);
    }
    for (auto _ : state) {
        benchmark::DoNotOptimize(tree.find(500));
    }
}
BENCHMARK(BM_BST_Find);

static void BM_BST_FindNotFound(benchmark::State& state) {
    BST tree;
    for (int i = 0; i < 1000; i++) {
        tree.insert(i);
    }
    for (auto _ : state) {
        benchmark::DoNotOptimize(tree.find(2000));
    }
}
BENCHMARK(BM_BST_FindNotFound);

// ======================== HashTable Benchmarks ========================
static void BM_HashTable_Insert(benchmark::State& state) {
    for (auto _ : state) {
        HashTable<std::string, int> ht;
        for (int i = 0; i < state.range(0); i++) {
            ht.insert("key" + std::to_string(i), i);
        }
    }
}
BENCHMARK(BM_HashTable_Insert)->Range(8, 8<<10);

static void BM_HashTable_Find(benchmark::State& state) {
    HashTable<std::string, int> ht;
    for (int i = 0; i < 1000; i++) {
        ht.insert("key" + std::to_string(i), i);
    }
    int value;
    for (auto _ : state) {
        benchmark::DoNotOptimize(ht.find("key500", value));
    }
}
BENCHMARK(BM_HashTable_Find);

static void BM_HashTable_Remove(benchmark::State& state) {
    for (auto _ : state) {
        HashTable<std::string, int> ht;
        for (int i = 0; i < 1000; i++) {
            ht.insert("key" + std::to_string(i), i);
        }
        for (int i = 0; i < state.range(0); i++) {
            ht.remove("key" + std::to_string(i));
        }
    }
}
BENCHMARK(BM_HashTable_Remove)->Range(8, 256);

static void BM_HashTable_CollisionHandling(benchmark::State& state) {
    for (auto _ : state) {
        HashTable<int, int> ht(16); // Small capacity to force collisions
        for (int i = 0; i < state.range(0); i++) {
            ht.insert(i, i * 10);
        }
    }
}
BENCHMARK(BM_HashTable_CollisionHandling)->Range(8, 8<<10);

// ======================== Queue Benchmarks ========================
static void BM_Queue_Push(benchmark::State& state) {
    for (auto _ : state) {
        Queue<int> q;
        for (int i = 0; i < state.range(0); i++) {
            q.push(i);
        }
    }
}
BENCHMARK(BM_Queue_Push)->Range(8, 8<<10);

static void BM_Queue_Pop(benchmark::State& state) {
    for (auto _ : state) {
        Queue<int> q;
        for (int i = 0; i < state.range(0); i++) {
            q.push(i);
        }
        for (int i = 0; i < state.range(0); i++) {
            benchmark::DoNotOptimize(q.pop());
        }
    }
}
BENCHMARK(BM_Queue_Pop)->Range(8, 8<<10);

static void BM_Queue_PushPop(benchmark::State& state) {
    Queue<int> q;
    for (auto _ : state) {
        q.push(1);
        benchmark::DoNotOptimize(q.pop());
    }
}
BENCHMARK(BM_Queue_PushPop);

// ======================== Stack Benchmarks ========================
static void BM_Stack_Push(benchmark::State& state) {
    for (auto _ : state) {
        Stack<int> s;
        for (int i = 0; i < state.range(0); i++) {
            s.push(i);
        }
    }
}
BENCHMARK(BM_Stack_Push)->Range(8, 8<<10);

static void BM_Stack_Pop(benchmark::State& state) {
    for (auto _ : state) {
        Stack<int> s;
        for (int i = 0; i < state.range(0); i++) {
            s.push(i);
        }
        for (int i = 0; i < state.range(0); i++) {
            benchmark::DoNotOptimize(s.pop());
        }
    }
}
BENCHMARK(BM_Stack_Pop)->Range(8, 8<<10);

static void BM_Stack_Peek(benchmark::State& state) {
    Stack<int> s;
    for (int i = 0; i < 1000; i++) {
        s.push(i);
    }
    for (auto _ : state) {
        benchmark::DoNotOptimize(s.peek());
    }
}
BENCHMARK(BM_Stack_Peek);

static void BM_Stack_CopyFrom(benchmark::State& state) {
    Stack<int> s1;
    for (int i = 0; i < state.range(0); i++) {
        s1.push(i);
    }
    for (auto _ : state) {
        Stack<int> s2;
        s2.copy_from(s1);
        benchmark::DoNotOptimize(s2);
    }
}
BENCHMARK(BM_Stack_CopyFrom)->Range(8, 256);

BENCHMARK_MAIN();


