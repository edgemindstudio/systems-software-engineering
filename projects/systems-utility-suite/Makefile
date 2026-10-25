CC := clang
CPPFLAGS := -Iinclude
WARNINGS := -Wall -Wextra -Wpedantic -Wconversion -Wshadow -Wstrict-prototypes -Wmissing-prototypes
CFLAGS := -std=c17 $(WARNINGS) -D_POSIX_C_SOURCE=200809L -g -O2
SANITIZER_FLAGS := -fsanitize=address,undefined -fno-omit-frame-pointer -O1
BUILD_DIR := build
TARGET := $(BUILD_DIR)/sysutil

SOURCES := \
	src/main.c \
	src/common.c \
	src/cmd_stat.c \
	src/cmd_copy.c \
	src/cmd_walk.c \
	src/cmd_hex.c \
	src/cmd_proc.c

OBJECTS := $(SOURCES:src/%.c=$(BUILD_DIR)/%.o)

.PHONY: all clean test sanitize

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CC) $(CFLAGS) $(OBJECTS) -o $@

$(BUILD_DIR)/%.o: src/%.c
	@mkdir -p $(BUILD_DIR)
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

test: all
	./tests/integration.sh $(TARGET)

sanitize: clean
	$(MAKE) CFLAGS="-std=c17 $(WARNINGS) -D_POSIX_C_SOURCE=200809L -g $(SANITIZER_FLAGS)" all
	ASAN_OPTIONS=detect_leaks=1:halt_on_error=1 \
	UBSAN_OPTIONS=halt_on_error=1 \
	./tests/integration.sh $(TARGET)

clean:
	rm -rf $(BUILD_DIR) workspace
