OUTPUT_FORMAT(elf32-x86-64)
SECTIONS
{
	. = 1M;

	.boot.text BLOCK(4K) : ALIGN(4K) { 
		*(.boot.text)
	}

	.text BLOCK(4K) : ALIGN(4K) { 
		*(.text)
	}

	.rodata BLOCK(4K) : ALIGN(4K) {
		*(.rodata)
	}

	.data BLOCK(4K) : ALIGN(4K) {
		*(.data)
	}

	.bss BLOCK(4K) : ALIGN(4K) {
		*(COMMON)
		*(.bss)
	}
}
