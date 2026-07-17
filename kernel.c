#include <stdint.h>

#include "uart.h"
#include "lib/libfdt/libfdt.h"


void kmain(const uint64_t* dtb_ptr) {
    const int uart_node = fdt_node_offset_by_compatible(dtb_ptr, -1, "ns16550a");
    int len;
    const uint32_t *reg = fdt_getprop(dtb_ptr, uart_node, "reg", &len);
    const uint64_t uart_addr = ((uint64_t)fdt32_to_cpu(reg[0]) << 32) | fdt32_to_cpu(reg[1]);

    uart_init(uart_addr);
    kprintf("UART initialisée a l'adresse %p\n", (void*)uart_addr);

    kprintf("Hello depuis mon kernel RISC-V !\n");

    char input[64];
    kprintf("Adresse du buffer : %p\n", &input);
    kprintf("Jumping to rust ! \n");


    while (1) {
        kprintf("> ");
        uart_gets(input, sizeof(input));
        kprintf("Tu as tapé : %s\n", input);
    }

}

// pour fermer qemu, ctrl+A puis X
/*
extern void rust_main(void);
void rust_print(const char* s, const size_t len)
{
    for(size_t i=0;i<len;i++)
        uart_putc(s[i]);
}
 */