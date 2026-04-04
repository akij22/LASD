CXX := g++
CXXFLAGS := -std=c++17 -O2 -Wall -Wextra
SRC := main.cpp
TARGET := quicksort
ARGS ?= 16

.PHONY: build run clean

build: $(TARGET)

$(TARGET): $(SRC)
	$(CXX) $(CXXFLAGS) $< -o $@

run: build
	./$(TARGET) $(ARGS)

clean:
	rm -f $(TARGET)
