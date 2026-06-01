# c-lab / 09-debug-sanitize
#
# Demonstrates debug vs release builds and sanitizer usage.
# The "buggy" sources have intentional bugs that sanitizers catch.
# The "fixed" sources show the corrected code.
# The test binary only tests the fixed code.

CC      ?= cc
STD     ?= -std=c11
WARN    := -Wall -Wextra -Wpedantic -Wformat=2
INCS    := -Iinclude
DEPGEN  := -MMD -MP

BUILD   := build

# Debug flags (default)
DBG_FLAGS := -O0 -g -fno-omit-frame-pointer
SAN_FLAGS := -fsanitize=address,undefined

# Release flags
REL_FLAGS := -O2 -DNDEBUG

.PHONY: all test clean debug release

# Default: build test binary without sanitizers (for CI)
all: $(BUILD)/test_debug

# Debug build: with sanitizers
debug: CFLAGS := $(STD) $(WARN) $(DBG_FLAGS) $(SAN_FLAGS) $(INCS) $(DEPGEN)
debug: LDFLAGS := $(SAN_FLAGS)
debug: $(BUILD)/test_debug
	@./$(BUILD)/test_debug

# Release build: optimized, no debug
release: CFLAGS := $(STD) $(WARN) $(REL_FLAGS) $(INCS) $(DEPGEN)
release: LDFLAGS :=
release: $(BUILD)/test_debug

# Default CFLAGS (no sanitizer for plain all/test targets)
CFLAGS  ?= $(STD) $(WARN) $(DBG_FLAGS) $(INCS) $(DEPGEN)
LDFLAGS ?=

$(BUILD)/test_debug: $(BUILD)/test_debug.o $(BUILD)/fixed.o
	$(CC) $(LDFLAGS) -o $@ $^

$(BUILD)/%.o: src/%.c | $(BUILD)
	$(CC) $(CFLAGS) -c -o $@ $<

$(BUILD)/test_%.o: tests/test_%.c | $(BUILD)
	$(CC) $(CFLAGS) -c -o $@ $<

$(BUILD):
	mkdir -p $@

test: $(BUILD)/test_debug
	@./$(BUILD)/test_debug

clean:
	rm -rf $(BUILD)

-include $(BUILD)/*.d
