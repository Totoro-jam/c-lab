# c-lab / 10-system-programming
#
# POSIX system programming: fork, exec, pipes, signals.
# Uses _POSIX_C_SOURCE for portability.

CC      ?= cc
STD     ?= -std=c11
WARN    := -Wall -Wextra -Wpedantic -Wformat=2
DBG     := -O0 -g
INCS    := -Iinclude
DEPGEN  := -MMD -MP

ifdef SANITIZE
SAN := -fsanitize=address,undefined -fno-omit-frame-pointer
endif

CFLAGS  := $(STD) $(WARN) $(DBG) $(INCS) $(DEPGEN) $(SAN)
LDFLAGS := $(SAN)

BUILD   := build

LIB_SRCS := src/process.c src/pipeutil.c
LIB_OBJS := $(patsubst src/%.c,$(BUILD)/%.o,$(LIB_SRCS))

TEST_BIN := $(BUILD)/test_system

.PHONY: all test clean

all: $(TEST_BIN)

$(TEST_BIN): $(BUILD)/test_system.o $(LIB_OBJS)
	$(CC) $(LDFLAGS) -o $@ $^

$(BUILD)/%.o: src/%.c | $(BUILD)
	$(CC) $(CFLAGS) -c -o $@ $<

$(BUILD)/test_%.o: tests/test_%.c | $(BUILD)
	$(CC) $(CFLAGS) -c -o $@ $<

$(BUILD):
	mkdir -p $@

test: $(TEST_BIN)
	@./$(TEST_BIN)

clean:
	rm -rf $(BUILD)

-include $(BUILD)/*.d
