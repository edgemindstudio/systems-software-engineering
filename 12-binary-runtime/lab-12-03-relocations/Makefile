CC := clang
CFLAGS := -std=c17 -Wall -Wextra -Wpedantic -Wconversion -Wshadow -Wstrict-prototypes -Wmissing-prototypes -D_POSIX_C_SOURCE=200809L -g -O0
BUILD_DIR := build
OBJECT := $(BUILD_DIR)/relocations.o

.PHONY: all clean

all: $(OBJECT)

$(OBJECT): src/main.c
	@mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(BUILD_DIR)
