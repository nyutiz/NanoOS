OUTPUT_ARCH("riscv")
ENTRY(_start)

/* QEMU virt machine charge le kernel à cette adresse */
BASE_ADDRESS = 0x80000000;

SECTIONS
{
    . = BASE_ADDRESS;

    .text : {
        *(.text.init)
        *(.text .text.*)
    }

    .rodata : {
        *(.rodata .rodata.*)
    }

    .data : {
        *(.data .data.*)
    }

    .bss : {
        _bss_start = .;
        *(.bss .bss.*)
        *(COMMON)
        _bss_end = .;
    }

    . = ALIGN(4096);
    .heap (NOLOAD) : {
        _heap_start = .;
        . += 16M;
        _heap_end = .;
    }

    . = ALIGN(4096);
    _stack_top = . + 0x10000;
}