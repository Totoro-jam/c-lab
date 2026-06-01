# c-lab root Makefile — run all chapters in one command

CHAPTERS := $(sort $(wildcard packages/*/))

.PHONY: all test clean

all:
	@for dir in $(CHAPTERS); do \
	  echo "\n=== $$dir ==="; \
	  $(MAKE) -C $$dir all || exit 1; \
	done

test:
	@fail=0; \
	for dir in $(CHAPTERS); do \
	  echo "\n=== $$dir ==="; \
	  $(MAKE) -C $$dir test || fail=1; \
	done; \
	exit $$fail

clean:
	@for dir in $(CHAPTERS); do \
	  $(MAKE) -C $$dir clean; \
	done
