CXX = g++
CXXFLAGS = -std=c++20 -pthread -Wall -Wextra -O2

# Имена исполняемых файлов
TARGET1 = task1_race_sync
TARGET2 = task2_trainings
TARGET3 = task3_readers_writers

# Исходные файлы
SRC1 = task1_race_sync.cpp
SRC2 = task2_trainings.cpp
SRC3 = task3_readers_writers.cpp

.PHONY: all clean run1 run2 run3

all: $(TARGET1) $(TARGET2) $(TARGET3)

$(TARGET1): $(SRC1)
	$(CXX) $(CXXFLAGS) -o $(TARGET1) $(SRC1)

$(TARGET2): $(SRC2)
	$(CXX) $(CXXFLAGS) -o $(TARGET2) $(SRC2)

$(TARGET3): $(SRC3)
	$(CXX) $(CXXFLAGS) -o $(TARGET3) $(SRC3)

run1: $(TARGET1)
	./$(TARGET1)

run2: $(TARGET2)
	./$(TARGET2)

run3: $(TARGET3)
	./$(TARGET3)

clean:
	rm -f $(TARGET1) $(TARGET2) $(TARGET3)

