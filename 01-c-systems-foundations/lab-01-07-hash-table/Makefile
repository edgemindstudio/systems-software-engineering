CC := clang
CPPFLAGS := -Iinclude
CFLAGS := -std=c17 -Wall -Wextra -Wpedantic -Wconversion -Wshadow -Wstrict-prototypes -Wmissing-prototypes -g -O0 -MMD -MP
SANFLAGS := -fsanitize=address,undefined -fno-omit-frame-pointer

BUILD_DIR := build
TARGET := $(BUILD_DIR)/lab
TEST_TARGET := $(BUILD_DIR)/tests

SOURCES := $(wildcard src/*.c)
LIB_SOURCES := $(filter-out src/main.c,$(SOURCES))
OBJECTS := $(SOURCES:src/%.c=$(BUILD_DIR)/%.o)
LIB_OBJECTS := $(filter-out $(BUILD_DIR)/main.o,$(OBJECTS))
DEPS := $(OBJECTS:.o=.d)

TEST_SOURCES := $(wildcard tests/*.c)

.PHONY: all test sanitize clean

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CC) $(OBJECTS) -o $@

$(BUILD_DIR)/%.o: src/%.c
	@mkdir -p $(BUILD_DIR)
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

$(TEST_TARGET): $(TEST_SOURCES) $(LIB_OBJECTS)
	@mkdir -p $(BUILD_DIR)
	$(CC) $(CPPFLAGS) $(CFLAGS) $(TEST_SOURCES) $(LIB_OBJECTS) -o $@

test: $(TEST_TARGET)
	./$(TEST_TARGET)

sanitize:
	@mkdir -p $(BUILD_DIR)/sanitize
	$(CC) $(CPPFLAGS) $(CFLAGS) $(SANFLAGS) $(SOURCES) -o $(BUILD_DIR)/sanitize/lab
	./$(BUILD_DIR)/sanitize/lab
	$(CC) $(CPPFLAGS) $(CFLAGS) $(SANFLAGS) $(TEST_SOURCES) $(LIB_SOURCES) -o $(BUILD_DIR)/sanitize/tests
	./$(BUILD_DIR)/sanitize/tests

clean:
	rm -rf $(BUILD_DIR)

-include $(DEPS)
