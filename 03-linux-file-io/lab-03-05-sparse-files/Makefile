CC := clang
CFLAGS := -std=c17 -Wall -Wextra -Wpedantic -Wconversion -Wshadow -Wstrict-prototypes -Wmissing-prototypes -g -O0
BUILD_DIR := build
TARGET := $(BUILD_DIR)/sparse_demo

.PHONY: all clean
all: $(TARGET)
$(TARGET): src/main.c
	@mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) $< -o $@
clean:
	rm -rf $(BUILD_DIR)
