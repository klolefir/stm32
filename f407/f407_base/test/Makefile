CC=arm-none-eabi-gcc
CFLAGS=-mcpu=cortex-m4 -mthumb -nostdlib -g -O0
CPPFLAGS=-DSTM32F407xx \
		 -I../vendor/CMSIS/Device/ST/STM32F4/Include \
		 -I../vendor/CMSIS/CMSIS/Core/Include \
		 -I ../../perlib
LINKER_FILE=link.ld
LDFLAGS=-T $(LINKER_FILE) -g
SOURCES = main.c startup.c ../vendor/CMSIS/Device/ST/STM32F4/Source/Templates/system_stm32f4xx.c ../../perlib/gpio.c
SOURCES += uart.c

all: firmware.elf

firmware.elf: $(SOURCES)
	$(CC) $(CFLAGS) $(CPPFLAGS) $(LDFLAGS) $^ -o firmware.elf

firmware.bin: firmware.elf
	arm-none-eabi-objcopy -O binary $< $@	

flash: firmware.bin
	st-flash --reset write $< 0x8000000

clean:
	rm firmware.*


