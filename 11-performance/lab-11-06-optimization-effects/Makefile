CC := clang
COMMON := -std=c17 -Wall -Wextra -Wpedantic -Wconversion -Wshadow -Wstrict-prototypes -Wmissing-prototypes -D_POSIX_C_SOURCE=200809L -g
BUILD_DIR := build
O0 := $(BUILD_DIR)/optimization_o0
O2 := $(BUILD_DIR)/optimization_o2

.PHONY: all clean

all: $(O0) $(O2)

$(O0): src/main.c
	@mkdir -p $(BUILD_DIR)
	$(CC) $(COMMON) -O0 $< -o $@

$(O2): src/main.c
	@mkdir -p $(BUILD_DIR)
	$(CC) $(COMMON) -O2 $< -o $@

clean:
	rm -rf $(BUILD_DIR)
