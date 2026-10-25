CC := clang
CFLAGS := -std=c17 -Wall -Wextra -Wpedantic -Wconversion -Wshadow -Wstrict-prototypes -Wmissing-prototypes -D_POSIX_C_SOURCE=200809L -g -O0 -fPIC -Iinclude
BUILD_DIR := build
LIB_DIR := $(BUILD_DIR)/lib
BIN_DIR := $(BUILD_DIR)/bin

.PHONY: all clean
all: $(BIN_DIR)/demo

$(LIB_DIR)/libincidentdemo.so: src/lib.c include/demo.h
	@mkdir -p $(LIB_DIR)
	$(CC) $(CFLAGS) -shared src/lib.c -o $@

$(BIN_DIR)/demo: src/main.c include/demo.h $(LIB_DIR)/libincidentdemo.so
	@mkdir -p $(BIN_DIR)
	$(CC) -std=c17 -Wall -Wextra -Wpedantic -Wconversion -Wshadow -Wstrict-prototypes -Wmissing-prototypes -D_POSIX_C_SOURCE=200809L -g -O0 -Iinclude src/main.c -L$(LIB_DIR) -lincidentdemo -o $@

clean:
	rm -rf $(BUILD_DIR)
