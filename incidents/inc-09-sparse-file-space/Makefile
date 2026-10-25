CC := clang
CFLAGS := -std=c17 -Wall -Wextra -Wpedantic -Wconversion -Wshadow -Wstrict-prototypes -Wmissing-prototypes -D_POSIX_C_SOURCE=200809L -g -O0
BUILD_DIR := build
TARGET := $(BUILD_DIR)/incident_09_sparse_file_space

.PHONY: all clean
all: $(TARGET)

$(TARGET): src/main.c
	@mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -rf $(BUILD_DIR) workspace
