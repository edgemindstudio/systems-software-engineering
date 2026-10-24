CC := clang
CFLAGS := -std=c17 -Wall -Wextra -Wpedantic -Wconversion -Wshadow -Wstrict-prototypes -Wmissing-prototypes -D_POSIX_C_SOURCE=200809L -g -O0
BUILD_DIR := build
LAUNCHER := $(BUILD_DIR)/inherit_launcher
TARGET := $(BUILD_DIR)/inherit_target
.PHONY: all clean
all: $(LAUNCHER) $(TARGET)
$(LAUNCHER): src/launcher.c
	@mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) $< -o $@
$(TARGET): src/target.c
	@mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) $< -o $@
clean:
	rm -rf $(BUILD_DIR)
