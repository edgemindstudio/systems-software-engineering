CC := clang
CPPFLAGS := -Iinclude
WARNINGS := -Wall -Wextra -Wpedantic -Wconversion -Wshadow -Wstrict-prototypes -Wmissing-prototypes
CFLAGS := -std=c17 $(WARNINGS) -D_POSIX_C_SOURCE=200809L -g -O2 -pthread
SAN_FLAGS := -fsanitize=address,undefined -fno-omit-frame-pointer -O1

BUILD_DIR := build
PROTO_OBJ := $(BUILD_DIR)/protocol.o
SERVER := $(BUILD_DIR)/netservice-server
CLIENT := $(BUILD_DIR)/netservice-client

.PHONY: all clean test sanitize

all: $(SERVER) $(CLIENT)

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

$(PROTO_OBJ): src/protocol.c include/netservice/protocol.h | $(BUILD_DIR)
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

$(SERVER): src/server.c $(PROTO_OBJ) | $(BUILD_DIR)
	$(CC) $(CPPFLAGS) $(CFLAGS) src/server.c $(PROTO_OBJ) -o $@

$(CLIENT): src/client.c $(PROTO_OBJ) | $(BUILD_DIR)
	$(CC) $(CPPFLAGS) $(CFLAGS) src/client.c $(PROTO_OBJ) -o $@

test: all
	python3 tests/integration.py ./$(SERVER)

sanitize: clean
	$(MAKE) CFLAGS="-std=c17 $(WARNINGS) -D_POSIX_C_SOURCE=200809L -g $(SAN_FLAGS) -pthread" all
	ASAN_OPTIONS=detect_leaks=1:halt_on_error=1 \
	UBSAN_OPTIONS=halt_on_error=1 \
	python3 tests/integration.py ./$(SERVER)

clean:
	rm -rf $(BUILD_DIR) workspace
