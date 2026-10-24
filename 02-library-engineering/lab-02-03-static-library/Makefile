CC := clang
AR := ar
RANLIB := ranlib

CPPFLAGS := -Iinclude
CFLAGS := -std=c17 -Wall -Wextra -Wpedantic -Wconversion -Wshadow -Wstrict-prototypes -Wmissing-prototypes -g -O0

BUILD_DIR := build
OBJ_DIR := $(BUILD_DIR)/obj
LIB_DIR := $(BUILD_DIR)/lib
BIN_DIR := $(BUILD_DIR)/bin

LIB := $(LIB_DIR)/libtextops.a
LIB_OBJECT := $(OBJ_DIR)/textops.o
APP := $(BIN_DIR)/static_demo
TEST := $(BIN_DIR)/test_textops

.PHONY: all test clean inspect

all: $(LIB) $(APP)

$(LIB_OBJECT): src/textops.c include/textops.h
	@mkdir -p $(OBJ_DIR)
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

$(LIB): $(LIB_OBJECT)
	@mkdir -p $(LIB_DIR)
	$(AR) rcs $@ $^
	$(RANLIB) $@

$(APP): app/main.c $(LIB)
	@mkdir -p $(BIN_DIR)
	$(CC) $(CPPFLAGS) $(CFLAGS) app/main.c -L$(LIB_DIR) -ltextops -o $@

$(TEST): tests/test_textops.c $(LIB)
	@mkdir -p $(BIN_DIR)
	$(CC) $(CPPFLAGS) $(CFLAGS) tests/test_textops.c -L$(LIB_DIR) -ltextops -o $@

test: $(TEST)
	./$(TEST)

inspect: $(LIB) $(APP)
	$(AR) t $(LIB)
	nm --defined-only $(LIB)
	ldd $(APP)

clean:
	rm -rf $(BUILD_DIR)
