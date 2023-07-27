arm-none-eabi-gcc main.c startup.c -T link.ld -o firmware.elf -mcpu=cortex-m4 -mthumb -nostdlib -g
arm-none-eabi-objcopy -O binary firmware.elf firmware.bin
st-flash --reset write firmware.bin 0x8000000
