MEMORY
{
	FLASH (rx):	ORIGIN = 0x08008000, LENGTH = 480K
	SRAM (rwx): ORIGIN = 0x20000000, LENGTH = 128K
}

SECTIONS
{
	.isr_vector :
	{
		KEEP(*(.isr_vector))
	} >FLASH

	.text :
	{
		. = ALIGN(4);
		*(.text)
		*(.rodata)

		. = ALIGN(4);
		_etext = .;
	} >FLASH

	.data :
	{
		. = ALIGN(4);
		_sdata = .;
		*(.data)
		. = ALIGN(4);
		_edata = .;
	} >SRAM AT> FLASH

	.bss :
	{
		. = ALIGN(4);
		_sbss = .;
		*(.bss)
		
		. = ALIGN(4);
		_ebss = .;
	} >SRAM
}
