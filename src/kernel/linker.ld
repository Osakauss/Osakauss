OUTPUT_FORMAT(elf64-x86-64)
ENTRY(main)

SECTIONS
{

    . = 0xffffffff80200000;

    .stivale2hdr : {
        KEEP(*(.stivale2hdr))
    }

    .text : {
        *(.text*)
    }
    .rodata : {
        *(.rodata*)
    }
    .data : {
        *(.data*)
    }

    .bss : {
        *(COMMON)
        *(.bss*)
    }
    end = .; _end = .; __end = .;
}