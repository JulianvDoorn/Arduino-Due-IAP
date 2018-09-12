.PHONY: all clean help

lib.o: lib.c lib.h
	@arm-none-eabi-gcc -c lib.c -Os -Ideps --std=c11 -fPIC -mcpu=cortex-m3 -mthumb -fomit-frame-pointer -march=armv7-m -fno-exceptions

lib-resolved.o: lib.o link-driver.ld
	@arm-none-eabi-ld lib.o -o lib-resolved.o -T link-driver.ld

lib.bin: lib-resolved.o
	@arm-none-eabi-objcopy lib-resolved.o lib.bin -O binary


view-lib.bin: lib.bin
	@xxd -e lib.bin

view-lib.o: lib.o
	@arm-none-eabi-objdump -d lib.o

view-lib-resolved.o: lib-resolved.o
	@arm-none-eabi-objdump -d lib-resolved.o

view-main: main
	@arm-none-eabi-objdump -d main

help:
	@echo "available target: all view-lib.bin view-lib.o"

main: main.o lib.o
	@arm-none-eabi-gcc -o main main.c hal.o -flto -ffreestanding -nostdlib -flto

all: lib.bin

clean:
	@rm -f lib.o
	@rm -f lib.bin
	@rm -f main
	@rm -f main.o
	@rm -f lib-resolved.o