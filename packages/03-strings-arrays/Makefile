# c-lab / 03-strings-arrays

CC      ?= cc
STD     ?= -std=c11
WARN    := -Wall -Wextra -Wpedantic -Werror -Wformat=2
DBG     := -O0 -g
INCS    := -Iinclude
DEPGEN  := -MMD -MP

ifdef SANITIZE
SAN := -fsanitize=address,undefined -fno-omit-frame-pointer
endif

CFLAGS  := $(STD) $(WARN) $(DBG) $(INCS) $(DEPGEN) $(SAN)
LDFLAGS := $(SAN)

BUILD   := build

LIB_SRCS := src/safestr.c src/csv.c
LIB_OBJS := $(patsubst src/%.c,$(BUILD)/%.o,$(LIB_SRCS))

TEST_BIN := $(BUILD)/test_strings

.PHONY: all test clean

all: $(TEST_BIN)

$(TEST_BIN): $(BUILD)/test_strings.o $(LIB_OBJS)
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
