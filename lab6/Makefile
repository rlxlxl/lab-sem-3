CXX = clang++
CXXFLAGS = -std=c++17 -Wall
TARGET = lab6_app
SOURCE = main.cpp

# Получаем флаги из pkg-config
PQXX_CFLAGS = $(shell pkg-config --cflags libpqxx)
PQXX_LIBS = $(shell pkg-config --libs libpqxx)

all: $(TARGET)

$(TARGET): $(SOURCE)
	$(CXX) $(CXXFLAGS) $(SOURCE) -o $(TARGET) $(PQXX_CFLAGS) $(PQXX_LIBS)

clean:
	rm -f $(TARGET)

.PHONY: all clean

