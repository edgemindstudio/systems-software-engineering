CC := clang
CFLAGS := -std=c17 -Wall -Wextra -Wpedantic -Wconversion -Wshadow -Wstrict-prototypes -Wmissing-prototypes -D_POSIX_C_SOURCE=200809L -g -O0
BUILD_DIR := build
TARGET := $(BUILD_DIR)/udp_demo
.PHONY: all clean
all: $(TARGET)
$(TARGET): src/main.c
	@mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) $< -o $@
clean:
	rm -rf $(BUILD_DIR)
