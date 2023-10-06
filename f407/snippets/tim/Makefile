ELF2BIN = arm-none-eabi-objcopy
CC = /usr/bin/arm-none-eabi-gcc

USR = $(USER)
PRJ_PATH = /home/$(USR)/projects/stm32/f407/projects/floppy
FIRMWARE_PATH = $(PRJ_PATH)/bin
STM32_PATH = /home/$(USR)/projects/stm32/f407
PERLIB_PATH = $(STM32_PATH)/perlib
KELIB_PATH = $(STM32_PATH)/kelib
VENDOR_PATH = $(STM32_PATH)/vendor/cmsis
STM32LIB_PATH = $(VENDOR_PATH)/stm32f4x
OPTIMIZATION = -O0

CFLAGS = -Wall -g -mcpu=cortex-m4 -mthumb $(OPTIMIZATION)

MACRO = -D STM32F407xx
MAIN_INC 	=	-I ./
VENDOR_INC 	= 	-I $(STM32LIB_PATH)/inc \
				-I $(VENDOR_PATH)
PERLIB_INC 	= 	-I $(PERLIB_PATH)
KELIB_INC 	= 	-I $(KELIB_PATH)

CPPFLAGS = 	$(MACRO) $(MAIN_INC) $(VENDOR_INC) $(PERLIB_INC) $(KELIB_INC)

LINKER_FILE = $(STM32_PATH)/snippets/general/link.ld 
LDFLAGS = -Wall -g -nostdlib -T $(LINKER_FILE)

MAIN_SRC 	=	$(PRJ_PATH)/main.c
SYSTEM_SRC 	= 	$(STM32LIB_PATH)/src/system_stm32f4xx.c
STARTUP_SRC = 	$(STM32_PATH)/snippets/general/startup.c
PERLIB_SRC = 	$(PERLIB_PATH)/usart.c \
				$(PERLIB_PATH)/gpio.c \
				$(PERLIB_PATH)/nvic.c \
				$(PERLIB_PATH)/general.c \
				$(PERLIB_PATH)/tim.c \
				$(PERLIB_PATH)/tim_hd.c \
				$(PERLIB_PATH)/rcc.c \
				$(PERLIB_PATH)/systick.c
KELIB_SRC 	= 	$(KELIB_PATH)/kestring.c

SOURCES = $(MAIN_SRC) $(SYSTEM_SRC) $(STARTUP_SRC) $(PERLIB_SRC) $(KELIB_SRC)

ELF = firmware.elf
BIN = firmware.bin
LOAD_ADDR = 0x08000000

OPENOCD = /usr/bin/openocd
OPENOCD_INT = /usr/share/openocd/scripts/interface/stlink.cfg
OPENOCD_TAR = /usr/share/openocd/scripts/target/stm32f4x.cfg
OPENOCD_PORT = 3333

GDB = gdb-multiarch
GDB_CONF = $(STM32_PATH)/snippets/general/gdb.conf

STFLASH = st-flash

DFU = dfu-util

all: $(BIN)

$(ELF): $(SOURCES)
	$(CC) $(CFLAGS) $(CPPFLAGS) $(LDFLAGS) $^ -o $@

$(BIN): $(ELF)
	$(ELF2BIN) -O binary $< $@

stflash: $(BIN)
	$(STFLASH) --reset write $< $(LOAD_ADDR)

dfuflash: $(BIN)
	$(DFU) -a 0 -s $(LOAD_ADDR) -D $<

ocdflash: $(BIN)
	$(GDB) -x $(GDB_CONF)

openocd:
	$(OPENOCD) -f $(OPENOCD_INT) -f $(OPENOCD_TAR)


debug: $(ELF)
	$(GDB) -iex "tar ext :$(OPENOCD_PORT)" $^

clean:
	rm $(ELF) $(BIN)
