#include <stdint.h>

#include "../include/uart.h"
#include "../lib/libfdt/libfdt.h"
#include "../include/liballoc.h"

extern void rust_main(void);

extern char _heap_start;
extern char _heap_end;

void rust_print(const char* s, const size_t len){
    for(size_t i=0;i<len;i++)
        uart_putc(s[i]);
}

void kmain(const uint64_t* dtb_ptr) {
    const int uart_node = fdt_node_offset_by_compatible(dtb_ptr, -1, "ns16550a");
    int len;
    const uint32_t *reg = fdt_getprop(dtb_ptr, uart_node, "reg", &len);
    const uint64_t uart_addr = ((uint64_t)fdt32_to_cpu(reg[0]) << 32) | fdt32_to_cpu(reg[1]);

    uart_init(uart_addr);

    kprintf("UART initialized at the adress %p\n", (void*)uart_addr);

    kprintf("Setup of the Memory allocator...\n");
    kprintf("Check up heap : \n");
    kprintf("       start : %p\n", (void*)&_heap_start);
    kprintf("       end : %p\n", (void*)&_heap_end);

    const long diff = &_heap_end - &_heap_start;

    kprintf("Difference : %d\n", diff);

    kprintf("Jumping to rust ! \n");

    rust_main();


    //char input[64];
    //kprintf("Adresse du buffer : %p\n", &input);
    //while (1) {
    //    kprintf("> ");
    //    uart_gets(input, sizeof(input));
    //    kprintf("Tu as tapé : %s\n", input);
    //}
}

// pour fermer qemu, ctrl+A puis X
