# c-lab / 08-build-systems
#
# This Makefile demonstrates proper build system practices:
#  - Separate compilation
#  - Automatic dependency tracking (-MMD -MP)
#  - Static library creation (ar)
#  - Pattern rules
#  - .PHONY targets
#  - Configurable flags
#  - Parallel-safe (make -j)

CC      ?= cc
AR      ?= ar
STD     ?= -std=c11
WARN    := -Wall -Wextra -Wpedantic -Werror -Wformat=2
DBG     := -O0 -g
INCS    := -Iinclude
DEPGEN  := -MMD -MP

ifdef SANITIZE
SAN := -fsanitize=address,undefined -fno-omit-frame-pointer
endif

ifdef RELEASE
OPT := -O2 -DNDEBUG
DBG :=
endif

CFLAGS  := $(STD) $(WARN) $(DBG) $(OPT) $(INCS) $(DEPGEN) $(SAN)
LDFLAGS := $(SAN)

BUILD   := build

# Library sources
LIB_SRCS := src/mathlib.c src/strlib.c
LIB_OBJS := $(patsubst src/%.c,$(BUILD)/%.o,$(LIB_SRCS))
LIB_A    := $(BUILD)/libdemo.a

# Main binary
MAIN_BIN := $(BUILD)/demo

# Test binary
TEST_BIN := $(BUILD)/test_build

.PHONY: all test clean lib info

all: $(MAIN_BIN) $(TEST_BIN)

# Static library
lib: $(LIB_A)

$(LIB_A): $(LIB_OBJS)
	$(AR) rcs $@ $^

# Main binary links against the static library
$(MAIN_BIN): $(BUILD)/main.o $(LIB_A)
	$(CC) $(LDFLAGS) -o $@ $(BUILD)/main.o -L$(BUILD) -ldemo

# Test binary links object files directly
$(TEST_BIN): $(BUILD)/test_build.o $(LIB_OBJS)
	$(CC) $(LDFLAGS) -o $@ $^

# Pattern rules
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

# Show computed variables (useful for debugging the Makefile itself)
info:
	@echo "CC      = $(CC)"
	@echo "CFLAGS  = $(CFLAGS)"
	@echo "LDFLAGS = $(LDFLAGS)"
	@echo "LIB_OBJS= $(LIB_OBJS)"

# Include auto-generated dependency files
-include $(BUILD)/*.d
