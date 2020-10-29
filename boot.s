.set MB_MAGIC,                      0x1badb002
.set MB_FLAGS,                      0x00010040


.set CODE_SEG,                          0x0008
.set DATA_SEG,                          0x0010


    	.section            ".boot.text", "ax"
        .code32
        .balign 16
mbStart:
        .long MB_MAGIC
        .long MB_FLAGS
        .long -MB_MAGIC - MB_FLAGS

        .long mbStart
        .long mbStart
        .long 0
        .long 0
        .long start32

start32:
        cli
        movl %ebx, %edi

        # enable 64-bit page table with CR4.PAE
        movl %cr4, %eax
        btsl $5, %eax
        movl %eax, %cr4

        # fill just enough of the page table
        # create a long mode page table
        movl $(pml4), %eax
        movl %eax, %cr3

        # enable long mode and SYSCALL
        movl $0xc0000080, %ecx
        rdmsr
        btsl $8, %eax
        btsl $0, %eax
        wrmsr

        lgdt (pGDT)
        lidt (pIDT)

        movl %cr0, %eax
        btsl $31, %eax
        movl %eax, %cr0

        # enable paging and jump to long mode
        ljmp $CODE_SEG, $(start64)

        .balign 16
        .code64


start64:
        movw $DATA_SEG, %ax
        movw %ax, %ss
        movw %ax, %ds
        movw %ax, %es
        movw %ax, %fs
        movw %ax, %gs
       # Push that address into IA32_LSTAR
        movq $0xC0000082, %rcx
        wrmsr
        # Load our segments into edx:eax
        mov $0, %eax
        mov $0x00180008, %edx
        #Push those segments into IA32_STAR
        movq $0xC0000081, %rcx
        wrmsr

        movl $0x12344345, %esi
        movl $(stackTop), %esp
        .balign 16
        .global hbreak_start
hbreak_start:
        jmp bootStart

# GDT and IDT
        .balign 4096
GDT_START:
        .quad   0       # NULL descriptor

GDT_CSK:
        .short  0xffff  # code segment limit - ignored
        .short  0       # code base low - ignored
        .byte   0       # code base middle - ignored
        .byte   0x9A    # code access
        .byte   0xaf    # granularity (L=1)
        .byte   0       # code base high - ignored

GDT_DSK:
        .short  0xffff  # code segment limit - ignored
        .short  0       # code base low - ignored
        .byte   0       # code base middle - ignored
        .byte   0x92    # code access
        .byte   0xcf    # granularity (L=1)
        .byte   0       # code base high - ignored
GDT_END:

        .word 0 # padding


pGDT:
        .word   GDT_END - GDT_START - 1
        .quad   GDT_START

        .word 0 #padding

pIDT:
        .word 0
        .quad 0


.align 4096 # tables MUST be aligned

# PML4
pml4:
        .quad   (pml3a + 0x0000000000000003)
        .fill   255, 8, 0
        .quad   (pml4 + 0x0000000000000003)
        .fill   254, 8, 0
        .quad   (pml3b + 0x0000000000000003)


# PML3
pml3a:
        .quad (pml2 + 0x0000000000000003)
        .fill 511, 8, 0

pml3b:
        .fill 510, 8, 0
        .quad (pml2 + 0x0000000000000003)
        .quad 0x0000000000000000

# identity-map lower 2MB
pml2:
        .quad (pml1 + 0x000000000000003)
        .fill 511, 8, 0

pml1:
        i = 0
        .rept 512
        .quad ( i + 0x0000000000000003)
        i = i + 4096
        .endr

        .balign 4096
stackBottom:
        i = 0
        .rept 512
        .quad ( i + 0xdeadbeeff00df00f)
        i = i + 4096
        .endr
stackTop:
        .quad 0
        .quad 0
        .quad 0
        .quad 0
        i = 0
        .rept 509
        .quad ( i + 0xb00ff00b55aa1234)
        i = i + 4096
        .endr
