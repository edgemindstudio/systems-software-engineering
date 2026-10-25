CC := clang
CPPFLAGS := -Iinclude
WARNINGS := -Wall -Wextra -Wpedantic -Wconversion -Wshadow -Wstrict-prototypes -Wmissing-prototypes
CFLAGS := -std=c17 $(WARNINGS) -D_POSIX_C_SOURCE=200809L -g -O2 -pthread
SAN_FLAGS := -fsanitize=address,undefined -fno-omit-frame-pointer -O1
TSAN_FLAGS := -fsanitize=thread -fno-omit-frame-pointer -O1

BUILD_DIR := build
PROTO_OBJ := $(BUILD_DIR)/protocol.o
DIAG_OBJ := $(BUILD_DIR)/diagnostics.o
DAEMON := $(BUILD_DIR)/sysdiag-daemon
CLIENT := $(BUILD_DIR)/sysdiag

.PHONY: all clean test sanitize tsan

all: $(DAEMON) $(CLIENT)

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

$(PROTO_OBJ): src/protocol.c include/sysdiag/protocol.h | $(BUILD_DIR)
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

$(DIAG_OBJ): src/diagnostics.c include/sysdiag/diagnostics.h | $(BUILD_DIR)
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

$(DAEMON): src/daemon.c $(PROTO_OBJ) $(DIAG_OBJ) | $(BUILD_DIR)
	$(CC) $(CPPFLAGS) $(CFLAGS) src/daemon.c $(PROTO_OBJ) $(DIAG_OBJ) -o $@

$(CLIENT): src/client.c $(PROTO_OBJ) | $(BUILD_DIR)
	$(CC) $(CPPFLAGS) $(CFLAGS) src/client.c $(PROTO_OBJ) -o $@

test: all
	python3 tests/integration.py ./$(DAEMON) ./$(CLIENT)

sanitize: clean
	$(MAKE) CFLAGS="-std=c17 $(WARNINGS) -D_POSIX_C_SOURCE=200809L -g $(SAN_FLAGS) -pthread" all
	ASAN_OPTIONS=detect_leaks=1:halt_on_error=1 \
	UBSAN_OPTIONS=halt_on_error=1 \
	python3 tests/integration.py ./$(DAEMON) ./$(CLIENT)

tsan: clean
	$(MAKE) CFLAGS="-std=c17 $(WARNINGS) -D_POSIX_C_SOURCE=200809L -g $(TSAN_FLAGS) -pthread" all
	TSAN_OPTIONS=halt_on_error=1 \
	python3 tests/integration.py ./$(DAEMON) ./$(CLIENT)

clean:
	rm -rf $(BUILD_DIR) workspace
