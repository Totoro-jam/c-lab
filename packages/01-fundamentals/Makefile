# c-lab / 01-fundamentals
# Makefile 的最小好版本：自动依赖、可选 sanitizer、单一规则模式。

CC      ?= cc
STD     ?= -std=c11
WARN    := -Wall -Wextra -Wpedantic -Werror -Wformat=2
DBG     := -O0 -g
INCS    := -Iinclude
DEPGEN  := -MMD -MP

# 开 sanitizer:   make SANITIZE=1
ifdef SANITIZE
SAN := -fsanitize=address,undefined -fno-omit-frame-pointer
endif

CFLAGS  := $(STD) $(WARN) $(DBG) $(INCS) $(DEPGEN) $(SAN)
LDFLAGS := $(SAN)

BUILD   := build
LIB_SRC := src/calc.c
LIB_OBJ := $(BUILD)/calc.o

CLI_BIN := $(BUILD)/calc
TEST_BIN:= $(BUILD)/test_calc

.PHONY: all test clean

all: $(CLI_BIN) $(TEST_BIN)

$(CLI_BIN): $(BUILD)/main.o $(LIB_OBJ)
	$(CC) $(LDFLAGS) -o $@ $^

$(TEST_BIN): $(BUILD)/test_calc.o $(LIB_OBJ)
	$(CC) $(LDFLAGS) -o $@ $^

# 通用模式规则：build/foo.o <- src/foo.c
$(BUILD)/%.o: src/%.c | $(BUILD)
	$(CC) $(CFLAGS) -c -o $@ $<

# 测试编译规则：build/test_xxx.o <- tests/test_xxx.c
$(BUILD)/test_%.o: tests/test_%.c | $(BUILD)
	$(CC) $(CFLAGS) -c -o $@ $<

$(BUILD):
	mkdir -p $@

test: $(TEST_BIN)
	@./$(TEST_BIN)

clean:
	rm -rf $(BUILD)

# include 自动生成的依赖文件，使 .h 修改触发重编
-include $(BUILD)/*.d
