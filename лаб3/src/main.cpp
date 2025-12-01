#include <gtest/gtest.h>  
#include <iostream>

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);

    std::cout << "Всего тестов: " 
              << ::testing::UnitTest::GetInstance()->total_test_count() 
              << std::endl;

    return RUN_ALL_TESTS();
}