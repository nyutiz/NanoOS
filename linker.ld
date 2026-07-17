OUTPUT_ARCH("riscv")
ENTRY(_start)

/* QEMU virt machine charge le kernel à cette adresse */
BASE_ADDRESS = 0x80000000;

SECTIONS
{
    . = BASE_ADDRESS;

    .text : {
        *(.text.init)   /* Le code de boot.S en premier ! */
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
    _stack_top = . + 0x10000;  /* pile de 64 Ko */
}