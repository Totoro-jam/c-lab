# c-lab / 12-popular-libs
#
# Minimal demos: SQLite (in-memory CRUD) + libcurl (HTTP GET).
# Requires: sqlite3, libcurl (both ship with macOS / most Linux distros).

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
LDFLAGS := $(SAN) -lsqlite3 -lcurl

BUILD   := build

LIB_SRCS := src/db_demo.c src/http_demo.c
LIB_OBJS := $(patsubst src/%.c,$(BUILD)/%.o,$(LIB_SRCS))

TEST_BIN := $(BUILD)/test_popular_libs

.PHONY: all test clean

all: $(TEST_BIN)

$(TEST_BIN): $(BUILD)/test_popular_libs.o $(LIB_OBJS)
	$(CC) -o $@ $^ $(LDFLAGS)

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
