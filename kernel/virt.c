#include <stdint.h>

#define QEMU_TEST_ADDR 0x100000

#define FINISHER_FAIL  0x3333
#define FINISHER_PASS  0x5555
#define FINISHER_RESET 0x7777

extern inline void qemu_poweroff(void) {
    volatile uint32_t *finisher = (volatile uint32_t*)QEMU_TEST_ADDR;
    *finisher = FINISHER_PASS;
    while (1) { }
}

extern inline void qemu_reboot(void) {
    volatile uint32_t *finisher = (volatile uint32_t*)QEMU_TEST_ADDR;
    *finisher = FINISHER_RESET;
    while (1) { }
}

extern inline void qemu_fail(const uint16_t exit_code) {
    volatile uint32_t *finisher = (volatile uint32_t*)QEMU_TEST_ADDR;
    *finisher = ((uint32_t)exit_code << 16) | FINISHER_FAIL;
    while (1) { }
}