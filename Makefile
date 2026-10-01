CC ?= cc
CFLAGS ?= -std=c11 -Wall -Wextra -Wpedantic -O2
INCLUDES := -Icore/include -I03-init/include -I04-service-manager/include -I08-capabilities/include -I09-device/include
CORE_SRC := core/src/power.c core/src/identity.c 03-init/src/init.c 04-service-manager/src/service_manager.c 08-capabilities/src/capability_registry.c 09-device/src/device_manager.c
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
