CC := clang
CPPFLAGS := -Iinclude -Isrc
CFLAGS := -std=c17 -Wall -Wextra -Wpedantic -Wconversion -Wshadow -Wstrict-prototypes -Wmissing-prototypes -g -O0 -MMD -MP

BUILD_DIR := build
APP := $(BUILD_DIR)/telemetry_demo
TEST := $(BUILD_DIR)/test_telemetry

LIB_SOURCES := src/telemetry.c src/format_internal.c
LIB_OBJECTS := $(LIB_SOURCES:src/%.c=$(BUILD_DIR)/%.o)
APP_OBJECT := $(BUILD_DIR)/main.o
DEPS := $(LIB_OBJECTS:.o=.d) $(APP_OBJECT:.o=.d)

.PHONY: all test clean

all: $(APP)

$(APP): $(LIB_OBJECTS) $(APP_OBJECT)
	$(CC) $^ -o $@

$(BUILD_DIR)/%.o: src/%.c
	@mkdir -p $(BUILD_DIR)
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

$(TEST): tests/test_telemetry.c $(LIB_OBJECTS)
	$(CC) $(CPPFLAGS) $(CFLAGS) tests/test_telemetry.c $(LIB_OBJECTS) -o $@

test: $(TEST)
	./$(TEST)

clean:
	rm -rf $(BUILD_DIR)

-include $(DEPS)
