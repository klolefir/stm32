arm-none-eabi-gcc main.c startup.c -T link.ld -o blink.elf -mcpu=cortex-m4 -mthumb -nostdlib -g
arm-none-eabi-objcopy -O binary blink.elf blink.bin
st-flash --reset write blink.bin 0x8000000
