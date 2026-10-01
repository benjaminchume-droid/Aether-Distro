CC ?= cc
CFLAGS ?= -std=c11 -Wall -Wextra -Wpedantic -O2
INCLUDES := -Icore/include
CORE_SRC := core/src/power.c core/src/identity.c
CORE_OBJ := $(CORE_SRC:.c=.o)

.PHONY: all test clean
all: test
test: build/test_power
	./build/test_power
build/test_power: $(CORE_OBJ) tests/core/test_power.c
	@mkdir -p build
	$(CC) $(CFLAGS) $(INCLUDES) $(CORE_OBJ) tests/core/test_power.c -o $@
core/src/%.o: core/src/%.c
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@
clean:
	rm -rf build core/src/*.o
