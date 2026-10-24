CC := clang
CPPFLAGS := -Iinclude
CFLAGS := -std=c17 -Wall -Wextra -Wpedantic -Wconversion -Wshadow -Wstrict-prototypes -Wmissing-prototypes -g -O0 -MMD -MP

BUILD_DIR := build
APP := $(BUILD_DIR)/error_demo
TEST := $(BUILD_DIR)/test_safe_math

LIB_OBJECT := $(BUILD_DIR)/safe_math.o
APP_OBJECT := $(BUILD_DIR)/main.o
DEPS := $(LIB_OBJECT:.o=.d) $(APP_OBJECT:.o=.d)

.PHONY: all test clean

all: $(APP)

$(APP): $(LIB_OBJECT) $(APP_OBJECT)
	$(CC) $^ -o $@

$(BUILD_DIR)/%.o: src/%.c
	@mkdir -p $(BUILD_DIR)
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

$(TEST): tests/test_safe_math.c $(LIB_OBJECT)
	$(CC) $(CPPFLAGS) $(CFLAGS) tests/test_safe_math.c $(LIB_OBJECT) -o $@

test: $(TEST)
	./$(TEST)

clean:
	rm -rf $(BUILD_DIR)

-include $(DEPS)
