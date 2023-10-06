CC=arm-none-eabi-gcc
CFLAGS=-mcpu=cortex-m4 -mthumb -nostdlib
CPPFLAGS=-DSTM32F407xx \
		 -I ../../vendor/CMSIS/Device/ST/STM32F4/Include \
		 -I ../../vendor/CMSIS/CMSIS/Core/Include
LINKER_FILE = ../general/link.ld
LDFLAGS = -T $(LINKER_FILE)
SOURCES = main.c ../general/startup.c ../../vendor/CMSIS/Device/ST/STM32F4/Source/Templates/system_stm32f4xx.c

all: firmware.elf

firmware.elf: $(SOURCES)
	$(CC) $(CFLAGS) $(CPPFLAGS) $(LDFLAGS) $^ -o firmware.elf

ocdflash: $(BIN)
	gdb-multiarch -x gdb.ctrl

openocd:
	/usr/bin/openocd -f /usr/share/openocd/scripts/interface/stlink.cfg -f /usr/share/openocd/scripts/target/stm32f4x.cfg

firmware.bin: firmware.elf
	arm-none-eabi-objcopy -O binary $< $@	

flash: firmware.bin
	st-flash --reset write $< 0x8000000

clean:
	rm firmware.*


