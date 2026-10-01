CC ?= cc
CFLAGS ?= -std=c11 -Wall -Wextra -Wpedantic -O2
INCLUDES := -Icore/include -I03-init/include -I04-service-manager/include -I08-capabilities/include -I09-device/include
CORE_SRC := core/src/power.c core/src/identity.c core/src/types.c core/src/ipc.c core/src/event_bus.c core/src/capability_registry.c 03-init/src/init.c 04-service-manager/src/service_manager.c 08-capabilities/src/capability_registry.c 09-device/src/device_manager.c
CORE_OBJ := $(CORE_SRC:.c=.o)

.PHONY: all test foundation-test service-test aether-init initramfs clean
foundation-test: $(CORE_OBJ) tests/core/test_foundation.c
	@mkdir -p build
	$(CC) $(CFLAGS) $(INCLUDES) $(CORE_OBJ) tests/core/test_foundation.c -o build/test_foundation
	./build/test_foundation
all: test

aether-init: $(CORE_OBJ)
	@mkdir -p build
	$(CC) $(CFLAGS) -static $(INCLUDES) 03-init/src/main.c $(CORE_OBJ) -o build/aether-init

service-test: $(CORE_OBJ)
	@mkdir -p build
	$(CC) $(CFLAGS) $(INCLUDES) 04-service-manager/src/service_manager.c tests/service/test_service_manager.c $(CORE_OBJ) -o build/test_service_manager
	./build/test_service_manager

initramfs: aether-init
	sh build/initramfs/build.sh
test: build/test_power
	./build/test_power
build/test_power: $(CORE_OBJ) tests/core/test_power.c
	@mkdir -p build
	$(CC) $(CFLAGS) $(INCLUDES) $(CORE_OBJ) tests/core/test_power.c -o $@
core/src/%.o: core/src/%.c
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@
clean:
	rm -rf build core/src/*.o
