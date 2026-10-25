CC := clang
CFLAGS := -std=c17 -Wall -Wextra -Wpedantic -Wconversion -Wshadow -Wstrict-prototypes -Wmissing-prototypes -D_POSIX_C_SOURCE=200809L -g -O0 -fPIC -Iinclude
BUILD_DIR := build
LIB_DIR := $(BUILD_DIR)/lib
BIN_DIR := $(BUILD_DIR)/bin
LIB_REAL := $(LIB_DIR)/libruntimemath.so.1.0.0
LIB_SONAME := libruntimemath.so.1
LIB_LINK := libruntimemath.so
APP := $(BIN_DIR)/runtime_loader_demo

.PHONY: all clean

all: $(APP)

$(LIB_REAL): src/runtime_math.c include/runtime_math.h
	@mkdir -p $(LIB_DIR)
	$(CC) $(CFLAGS) -shared -Wl,-soname,$(LIB_SONAME) src/runtime_math.c -o $@
	ln -sf libruntimemath.so.1.0.0 $(LIB_DIR)/$(LIB_SONAME)
	ln -sf $(LIB_SONAME) $(LIB_DIR)/$(LIB_LINK)

$(APP): src/main.c include/runtime_math.h $(LIB_REAL)
	@mkdir -p $(BIN_DIR)
	$(CC) -std=c17 -Wall -Wextra -Wpedantic -Wconversion -Wshadow -Wstrict-prototypes -Wmissing-prototypes -D_POSIX_C_SOURCE=200809L -g -O0 -Iinclude src/main.c -L$(LIB_DIR) -lruntimemath 		-Wl,-rpath,'$$ORIGIN/../lib' -o $@

clean:
	rm -rf $(BUILD_DIR)
