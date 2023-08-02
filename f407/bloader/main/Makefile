CC = arm-none-eabi-gcc
CFLAGS = -mcpu=cortex-m4 -mthumb -Wall -g -O0 
CPPFLAGS = -DSTM32F407xx \
		   -I ../../vendor/CMSIS/Device/ST/STM32F4/Include \
		   -I ../../perlib \
		   -I ../../vendor/CMSIS/CMSIS/Core/Include/
LINKER_FILE = ../general/link_main.ld
LDFLAGS = -T $(LINKER_FILE) -Wall -g -nostdlib
SOURCES = main.c ../general/startup.c ../../vendor/CMSIS/Device/ST/STM32F4/Source/Templates/system_stm32f4xx.c
SOURCES += 	../../perlib/systick.c	\
			../../perlib/gpio.c		\
			../../perlib/nvic.c		\
			../../perlib/rcc.c		\
			../../perlib/usart.c	\
			../../perlib/tim.c		\
			../../perlib/tim_hd.c	\
			../../perlib/general.c

all: firmware.elf

firmware.elf: $(SOURCES)
	$(CC) $(CFLAGS) $(CPPFLAGS) $(LDFLAGS) $^ -o firmware.elf

firmware.bin: firmware.elf
	arm-none-eabi-objcopy -O binary $< $@

flash: firmware.bin
	st-flash --reset write $< 0x8000000

dfu: firmware.bin
	dfu-util -a 0 -s 0x08008000 -D $<

clean:
	rm firmware.*


