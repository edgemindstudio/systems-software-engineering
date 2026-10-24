CC := clang

CFLAGS := -std=c17 -Wall -Wextra -Wpedantic -Wconversion -Wshadow -Wstrict-prototypes -Wmissing-prototypes -g -O0
PICFLAGS := -fPIC

BUILD_DIR := build
SONAME := librecord.so.1
REAL_NAME := librecord.so.1.0.0
LINK_NAME := librecord.so

V1_DIR := $(BUILD_DIR)/v1
V2_DIR := $(BUILD_DIR)/v2
BIN_DIR := $(BUILD_DIR)/bin

V1_LIB := $(V1_DIR)/$(REAL_NAME)
V2_LIB := $(V2_DIR)/$(REAL_NAME)
CONSUMER := $(BIN_DIR)/record_consumer

.PHONY: all v1 v2 consumer test clean inspect

all: v1 v2 consumer

v1: $(V1_LIB)
v2: $(V2_LIB)
consumer: $(CONSUMER)

$(V1_DIR)/record.o: v1/src/record.c v1/include/record.h
	@mkdir -p $(V1_DIR)
	$(CC) -Iv1/include $(CFLAGS) $(PICFLAGS) -c $< -o $@

$(V2_DIR)/record.o: v2/src/record.c v2/include/record.h
	@mkdir -p $(V2_DIR)
	$(CC) -Iv2/include $(CFLAGS) $(PICFLAGS) -c $< -o $@

$(V1_LIB): $(V1_DIR)/record.o
	$(CC) -shared -Wl,-soname,$(SONAME) -o $@ $^
	ln -sfn $(REAL_NAME) $(V1_DIR)/$(SONAME)
	ln -sfn $(SONAME) $(V1_DIR)/$(LINK_NAME)

$(V2_LIB): $(V2_DIR)/record.o
	$(CC) -shared -Wl,-soname,$(SONAME) -o $@ $^
	ln -sfn $(REAL_NAME) $(V2_DIR)/$(SONAME)
	ln -sfn $(SONAME) $(V2_DIR)/$(LINK_NAME)

$(CONSUMER): consumer/main.c $(V1_LIB)
	@mkdir -p $(BIN_DIR)
	$(CC) -Iv1/include $(CFLAGS) consumer/main.c -L$(V1_DIR) -lrecord -o $@

test: all
	LD_LIBRARY_PATH=$(V1_DIR) ./$(CONSUMER)
	@set +e; \
	LD_LIBRARY_PATH=$(V2_DIR) ./$(CONSUMER) >$(BUILD_DIR)/v2-output.txt 2>&1; \
	status=$$?; \
	set -e; \
	cat $(BUILD_DIR)/v2-output.txt; \
	test $$status -eq 2; \
	grep -q "ABI MISMATCH" $(BUILD_DIR)/v2-output.txt

inspect: all
	@echo "=== V1 SONAME ==="
	readelf -d $(V1_LIB) | grep SONAME
	@echo "=== V2 SONAME ==="
	readelf -d $(V2_LIB) | grep SONAME
	@echo "=== V1 EXPORTED SYMBOLS ==="
	nm -D --defined-only $(V1_LIB)
	@echo "=== V2 EXPORTED SYMBOLS ==="
	nm -D --defined-only $(V2_LIB)

clean:
	rm -rf $(BUILD_DIR)
