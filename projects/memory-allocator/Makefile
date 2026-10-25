CC := clang
CPPFLAGS := -Iinclude
WARNINGS := -Wall -Wextra -Wpedantic -Wconversion -Wshadow -Wstrict-prototypes -Wmissing-prototypes
CFLAGS := -std=c17 $(WARNINGS) -D_POSIX_C_SOURCE=200809L -g -O2
SANITIZER_FLAGS := -fsanitize=address,undefined -fno-omit-frame-pointer -O1

BUILD_DIR := build
LIBOBJ := $(BUILD_DIR)/allocator.o
TESTBIN := $(BUILD_DIR)/test_allocator
BENCHBIN := $(BUILD_DIR)/allocator_benchmark

.PHONY: all clean test sanitize benchmark

all: $(TESTBIN) $(BENCHBIN)

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

$(LIBOBJ): src/allocator.c include/allocator/allocator.h | $(BUILD_DIR)
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

$(TESTBIN): tests/test_allocator.c $(LIBOBJ) | $(BUILD_DIR)
	$(CC) $(CPPFLAGS) $(CFLAGS) tests/test_allocator.c $(LIBOBJ) -o $@

$(BENCHBIN): bench/benchmark.c $(LIBOBJ) | $(BUILD_DIR)
	$(CC) $(CPPFLAGS) $(CFLAGS) bench/benchmark.c $(LIBOBJ) -o $@

test: $(TESTBIN)
	./$(TESTBIN)

benchmark: $(BENCHBIN)
	./$(BENCHBIN)

sanitize: clean
	$(MAKE) CFLAGS="-std=c17 $(WARNINGS) -D_POSIX_C_SOURCE=200809L -g $(SANITIZER_FLAGS)" $(TESTBIN)
	ASAN_OPTIONS=detect_leaks=1:halt_on_error=1 \
	UBSAN_OPTIONS=halt_on_error=1 \
	./$(TESTBIN)

clean:
	rm -rf $(BUILD_DIR) workspace
