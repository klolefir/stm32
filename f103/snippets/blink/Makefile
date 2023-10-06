ELF2BIN = arm-none-eabi-objcopy
CC=arm-none-eabi-gcc

USR = $(USER)
PRJ_PATH = /home/$(USR)/projects/stm32/f103/snippets/blink
F103_PATH = /home/$(USR)/projects/stm32/f103
VENDOR_PATH = $(F103_PATH)/vendor
CMSIS_PATH = $(VENDOR_PATH)/cmsis
F103LIB_PATH = $(VENDOR_PATH)/stm32f1
OPTIMIZATION = -O0

CFLAGS = -Wall -g -mcpu=cortex-m4 -mthumb $(OPTIMIZATION)

MACRO = -D STM32F103x6
VENDOR_INC = -I $(CMSIS_PATH) -I $(F103LIB_PATH)/inc

INC = $(VENDOR_INC)

CPPFLAGS = $(MACRO) $(INC)

LINKER = $(F103_PATH)/general/link.ld
LDFLAGS = -Wall -g -nostdlib -T $(LINKER)

MAIN_SRC = $(PRJ_PATH)/main.c
SYSTEM_SRC = $(F103LIB_PATH)/src/system_stm32f1xx.c
STARTUP_SRC = $(F103_PATH)/general/startup.c
SRC = 	$(MAIN_SRC) \
		$(SYSTEM_SRC) \
		$(STARTUP_SRC)

ELF = firmware.elf
BIN = firmware.bin

OPENOCD = /usr/bin/openocd
OPENOCD_INT = /usr/share/openocd/scripts/interface/stlink.cfg
OPENOCD_TAR = /usr/share/openocd/scripts/target/stm32f1x.cfg
OPENOCD_PORT = 3333

GDB = gdb-multiarch
GDB_CONF = $(F103_PATH)/general/gdb.conf

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
