CC := clang
CPPFLAGS := -Iinclude
WARNINGS := -Wall -Wextra -Wpedantic -Wconversion -Wshadow -Wstrict-prototypes -Wmissing-prototypes
CFLAGS := -std=c17 $(WARNINGS) -D_POSIX_C_SOURCE=200809L -g -O2
SAN_FLAGS := -fsanitize=address,undefined -fno-omit-frame-pointer -O1

BUILD_DIR := build
TARGET := $(BUILD_DIR)/binspect
OBJECTS := $(BUILD_DIR)/main.o $(BUILD_DIR)/elf_view.o

.PHONY: all clean test sanitize

all: $(TARGET)

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

$(BUILD_DIR)/main.o: src/main.c include/binspect/elf_view.h | $(BUILD_DIR)
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/elf_view.o: src/elf_view.c include/binspect/elf_view.h | $(BUILD_DIR)
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

$(TARGET): $(OBJECTS)
	$(CC) $(CFLAGS) $(OBJECTS) -o $@

test: all
	./tests/integration.sh $(TARGET)

sanitize: clean
	$(MAKE) CFLAGS="-std=c17 $(WARNINGS) -D_POSIX_C_SOURCE=200809L -g $(SAN_FLAGS)" all
	ASAN_OPTIONS=detect_leaks=1:halt_on_error=1 \
	UBSAN_OPTIONS=halt_on_error=1 \
	./tests/integration.sh $(TARGET)

clean:
	rm -rf $(BUILD_DIR) workspace
