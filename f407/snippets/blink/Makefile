ELF2BIN = arm-none-eabi-objcopy
CC=arm-none-eabi-gcc

USR = $(USER)
PRJ_PATH = /home/$(USR)/projects/stm32/f407/snippets/blink
F407_PATH = /home/$(USR)/projects/stm32/f407
VENDOR_PATH = $(F407_PATH)/vendor
CMSIS_PATH = $(VENDOR_PATH)/cmsis
F407LIB_PATH = $(VENDOR_PATH)/stm32f4x
PERLIB_PATH = $(F407_PATH)/perlib
OPTIMIZATION = -O0

CFLAGS = -Wall -g -mcpu=cortex-m4 -mthumb $(OPTIMIZATION)

MACRO = -D STM32F407xx
VENDOR_INC = 	-I $(CMSIS_PATH) \
				-I $(F407LIB_PATH)/inc
PERLIB_INC = -I $(PERLIB_PATH)

INC = 	$(VENDOR_INC) \
		$(PERLIB_INC)

CPPFLAGS = $(MACRO) $(INC)

LINKER = $(F407_PATH)/general/link.ld
LDFLAGS = -Wall -g -nostdlib -T $(LINKER)

MAIN_SRC = $(PRJ_PATH)/main.c
SYSTEM_SRC = $(F407LIB_PATH)/src/system_stm32f4xx.c
PERLIB_SRC = $(PERLIB_PATH)/gpio.c \
			 $(PERLIB_PATH)/rcc.c \
			 $(PERLIB_PATH)/nvic.c \
			 $(PERLIB_PATH)/tim.c \
			 $(PERLIB_PATH)/tim_hd.c \
			 $(PERLIB_PATH)/general.c
STARTUP_SRC = $(F407_PATH)/general/startup.c
SRC = 	$(MAIN_SRC) \
		$(SYSTEM_SRC) \
		$(STARTUP_SRC) \
		$(PERLIB_SRC)

ELF = firmware.elf
BIN = firmware.bin

OPENOCD = /usr/bin/openocd
OPENOCD_INT = /usr/share/openocd/scripts/interface/stlink.cfg
OPENOCD_TAR = /usr/share/openocd/scripts/target/stm32f4x.cfg
OPENOCD_PORT = 3333

GDB = gdb-multiarch
GDB_CONF = $(F407_PATH)/general/gdb.conf

all: $(BIN)

$(ELF): $(SRC)
	$(CC) $(CFLAGS) $(CPPFLAGS) $(LDFLAGS) $^ -o $@

$(BIN): $(ELF)
	$(ELF2BIN) -O binary $< $@

openocd:
	$(OPENOCD) -f $(OPENOCD_INT) -f $(OPENOCD_TAR)

ocdflash: $(BIN)
	$(GDB) -x $(GDB_CONF)

debug: $(ELF)
	$(GDB) -iex "tar ext :$(OPENOCD_PORT)" $^


clean:
	rm $(ELF) $(BIN)
