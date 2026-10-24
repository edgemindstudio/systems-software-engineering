CXX := clang++
CXXFLAGS := -std=c++20 -Wall -Wextra -Wpedantic -Wconversion -Wshadow -Wnon-virtual-dtor -Wold-style-cast -Woverloaded-virtual -g -O0
BUILD_DIR := build
TARGET := $(BUILD_DIR)/raii_fd_demo

.PHONY: all clean

all: $(TARGET)

$(TARGET): src/main.cpp
	@mkdir -p $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) $< -o $@

clean:
	rm -rf $(BUILD_DIR)
