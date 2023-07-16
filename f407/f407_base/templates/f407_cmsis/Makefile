CC=arm-none-eabi-gcc
CFLAGS=-mcpu=cortex-m4 -mthumb -nostdlib
CPPFLAGS=-DSTM32F407xx \
		 -I../vendor/CMSIS/Device/ST/STM32F4/Include \
		 -I../vendor/CMSIS/CMSIS/Core/Include
LINKER_FILE=link.ld
LDFLAGS=-T $(LINKER_FILE)
SOURCES = main.c startup.c

all: firmware.elf

firmware.elf: $(SOURCES)
	$(CC) $(CFLAGS) $(CPPFLAGS) $(LDFLAGS) $^ -o firmware.elf

firmware.bin: firmware.elf
	arm-none-eabi-objcopy -O binary $< $@	

flash: firmware.bin
	st-flash --reset write $< 0x8000000

clean:
	rm firmware.*


