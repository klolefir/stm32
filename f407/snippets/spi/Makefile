CC = arm-none-eabi-gcc
CFLAGS = -mcpu=cortex-m4 -mthumb -Wall -g -O0
CPPFLAGS = -DSTM32F407xx \
		   -I ../../vendor/CMSIS/Device/ST/STM32F4/Include \
		   -I ../../vendor/CMSIS/CMSIS/Core/Include \
		   -I ../../perlib \
		   -I ../general
LINKER_FILE = ../general/link.ld
LDFLAGS = -T $(LINKER_FILE) -Wall -g -nostdlib
SOURCES = main.c ../general/startup.c ../../vendor/CMSIS/Device/ST/STM32F4/Source/Templates/system_stm32f4xx.c ../general/kestring.c
SOURCES += ../../perlib/*

OPENOCD = /usr/bin/openocd
OPENOCD_INT = /usr/share/openocd/scripts/interface/stlink.cfg
OPENOCD_TAR = /usr/share/openocd/scripts/target/stm32f4x.cfg
OPENOCD_PORT = 3333

GDB = gdb-multiarch
GDB_CONF = ~/projects/gd32/e103/snippets/general/gdb.conf

all: firmware.elf

firmware.elf: $(SOURCES)
	$(CC) $(CFLAGS) $(CPPFLAGS) $(LDFLAGS) $^ -o firmware.elf

firmware.bin: firmware.elf
	arm-none-eabi-objcopy -O binary $< $@	

ocdflash: $(BIN)
	$(GDB) -x $(GDB_CONF)

flash: firmware.bin
	st-flash --reset write $< 0x8000000

dfu: firmware.bin
	dfu-util -a 0 -s 0x08000000:leave -D $<

clean:
	rm firmware.*


