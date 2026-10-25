CC := clang
CPPFLAGS := -Iinclude
WARNINGS := -Wall -Wextra -Wpedantic -Wconversion -Wshadow -Wstrict-prototypes -Wmissing-prototypes
CFLAGS := -std=c17 $(WARNINGS) -D_POSIX_C_SOURCE=200809L -g -O2 -pthread
SANITIZER_FLAGS := -fsanitize=address,undefined -fno-omit-frame-pointer -O1
TSAN_FLAGS := -fsanitize=thread -fno-omit-frame-pointer -O1

BUILD_DIR := build
LIBOBJ := $(BUILD_DIR)/thread_runtime.o
TESTBIN := $(BUILD_DIR)/test_runtime
BENCHBIN := $(BUILD_DIR)/thread_runtime_benchmark

.PHONY: all clean test sanitize tsan benchmark

all: $(TESTBIN) $(BENCHBIN)

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

$(LIBOBJ): src/thread_runtime.c include/thread_runtime/thread_runtime.h | $(BUILD_DIR)
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

$(TESTBIN): tests/test_runtime.c $(LIBOBJ) | $(BUILD_DIR)
	$(CC) $(CPPFLAGS) $(CFLAGS) tests/test_runtime.c $(LIBOBJ) -o $@

$(BENCHBIN): bench/benchmark.c $(LIBOBJ) | $(BUILD_DIR)
	$(CC) $(CPPFLAGS) $(CFLAGS) bench/benchmark.c $(LIBOBJ) -o $@

test: $(TESTBIN)
	./$(TESTBIN)

benchmark: $(BENCHBIN)
	./$(BENCHBIN)

sanitize: clean
	$(MAKE) CFLAGS="-std=c17 $(WARNINGS) -D_POSIX_C_SOURCE=200809L -g $(SANITIZER_FLAGS) -pthread" $(TESTBIN)
	ASAN_OPTIONS=detect_leaks=1:halt_on_error=1 \
	UBSAN_OPTIONS=halt_on_error=1 \
	./$(TESTBIN)

tsan: clean
	$(MAKE) CFLAGS="-std=c17 $(WARNINGS) -D_POSIX_C_SOURCE=200809L -g $(TSAN_FLAGS) -pthread" $(TESTBIN)
	TSAN_OPTIONS=halt_on_error=1 \
	./$(TESTBIN)

clean:
	rm -rf $(BUILD_DIR) workspace
