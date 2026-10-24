CC := clang
CFLAGS := -std=c17 -Wall -Wextra -Wpedantic -Wconversion -Wshadow -Wstrict-prototypes -Wmissing-prototypes -D_POSIX_C_SOURCE=200809L -g -O0 -pthread
BUILD_DIR := build
TARGET := $(BUILD_DIR)/race_demo
TSAN_TARGET := $(BUILD_DIR)/race_demo_tsan
.PHONY: all tsan clean
all: $(TARGET)
$(TARGET): src/main.c
	@mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) $< -o $@
tsan: src/main.c
	@mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) -fsanitize=thread $< -o $(TSAN_TARGET)
clean:
	rm -rf $(BUILD_DIR)
