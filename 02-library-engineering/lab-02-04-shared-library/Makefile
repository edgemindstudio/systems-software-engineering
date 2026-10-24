CC := clang

CPPFLAGS := -Iinclude
CFLAGS := -std=c17 -Wall -Wextra -Wpedantic -Wconversion -Wshadow -Wstrict-prototypes -Wmissing-prototypes -g -O0
PICFLAGS := -fPIC

BUILD_DIR := build
OBJ_DIR := $(BUILD_DIR)/obj
LIB_DIR := $(BUILD_DIR)/lib
BIN_DIR := $(BUILD_DIR)/bin

VERSION := 1.0.0
MAJOR := 1
REAL_NAME := libcounter.so.$(VERSION)
SONAME := libcounter.so.$(MAJOR)
LINK_NAME := libcounter.so

REAL_LIB := $(LIB_DIR)/$(REAL_NAME)
SONAME_LINK := $(LIB_DIR)/$(SONAME)
DEV_LINK := $(LIB_DIR)/$(LINK_NAME)

LIB_OBJECT := $(OBJ_DIR)/counter.o
APP := $(BIN_DIR)/shared_demo
TEST := $(BIN_DIR)/test_counter

RPATH := -Wl,-rpath,'$$ORIGIN/../lib'

.PHONY: all test clean inspect

all: $(REAL_LIB) $(SONAME_LINK) $(DEV_LINK) $(APP)

$(LIB_OBJECT): src/counter.c include/counter.h
	@mkdir -p $(OBJ_DIR)
	$(CC) $(CPPFLAGS) $(CFLAGS) $(PICFLAGS) -c $< -o $@

$(REAL_LIB): $(LIB_OBJECT)
	@mkdir -p $(LIB_DIR)
	$(CC) -shared -Wl,-soname,$(SONAME) -o $@ $^

$(SONAME_LINK): $(REAL_LIB)
	ln -sfn $(REAL_NAME) $@

$(DEV_LINK): $(SONAME_LINK)
	ln -sfn $(SONAME) $@

$(APP): app/main.c $(DEV_LINK)
	@mkdir -p $(BIN_DIR)
	$(CC) $(CPPFLAGS) $(CFLAGS) app/main.c -L$(LIB_DIR) -lcounter $(RPATH) -o $@

$(TEST): tests/test_counter.c $(DEV_LINK)
	@mkdir -p $(BIN_DIR)
	$(CC) $(CPPFLAGS) $(CFLAGS) tests/test_counter.c -L$(LIB_DIR) -lcounter $(RPATH) -o $@

test: $(TEST)
	./$(TEST)

inspect: all
	readelf -d $(REAL_LIB)
	nm -D --defined-only $(REAL_LIB)
	ldd $(APP)

clean:
	rm -rf $(BUILD_DIR)
