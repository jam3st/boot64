ENTRY(_start)
SECTIONS
{
 . = 1M;
   .text : { *(.boot.text)
        *(.text) }
    . = ALIGN(4K);
    .data : { *(.data) }
    .bss : { *(.bss) }
}
