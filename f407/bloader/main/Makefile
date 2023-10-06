CC = arm-none-eabi-gcc
CFLAGS = -mcpu=cortex-m4 -mthumb -Wall -g -O0
CPPFLAGS = -DSTM32F407xx \
		   -I ../../vendor/CMSIS/Device/ST/STM32F4/Include \
		   -I ../../perlib \
		   -I ../../vendor/CMSIS/CMSIS/Core/Include	\
		   -I ../general 
LINKER_FILE = ../general/link_main.ld
LDFLAGS = -T $(LINKER_FILE) -Wall -g -nostdlib
MAIN_SOURCES = main.c fsm.c
GENERAL_SOURCES =  ../general/startup.c ../general/kestring.c
STARTUP_SOURCES =  ../../vendor/CMSIS/Device/ST/STM32F4/Source/Templates/system_stm32f4xx.c
PERLIB_SOURCES = 	../../perlib/systick.c	\
					../../perlib/gpio.c		\
					../../perlib/nvic.c		\
					../../perlib/rcc.c		\
					../../perlib/usart.c	\
					../../perlib/tim.c		\
					../../perlib/tim_hd.c	\
					../../perlib/general.c

SOURCES = $(MAIN_SOURCES) $(GENERAL_SOURCES) $(STARTUP_SOURCES) $(PERLIB_SOURCES)

all: firmware.elf

firmware.elf: $(SOURCES)
	$(CC) $(CFLAGS) $(CPPFLAGS) $(LDFLAGS) $^ -o firmware.elf

firmware.bin: firmware.elf
	arm-none-eabi-objcopy -O binary $< $@

flash: firmware.bin
	st-flash --reset write $< 0x8008000

dfu: firmware.bin
	dfu-util -a 0 -s 0x08008000 -D $<

clean:
	rm firmware.*


