arm-none-eabi-gcc main.c startup.c ../vendor/CMSIS/Device/ST/STM32F4/Source/Templates/system_stm32f4xx.c -T link.ld -I../vendor/CMSIS/Device/ST/STM32F4/Include -I../vendor/CMSIS/CMSIS/Core/Include -o blink.elf -mcpu=cortex-m4 -mthumb -nostdlib -g -DSTM32F407xx
arm-none-eabi-objcopy -O binary blink.elf blink.bin
st-flash --reset write blink.bin 0x8000000
