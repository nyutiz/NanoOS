#include "../include/uart.h"
#include <stdarg.h>

uint64_t uart_base = 0;

void uart_init(const uint64_t base) {
    uart_base = base;
}

static volatile char* reg(const uint64_t offset) {
    return (volatile char*)(uart_base + offset);
}

#define UART_THR_OFFSET 0
#define UART_LSR_OFFSET 5
#define LSR_THRE (1 << 5)
#define LSR_DR   (1 << 0)

void uart_putc(const char c) {
    while ((*reg(UART_LSR_OFFSET) & LSR_THRE) == 0) {
        // attend
    }
    *reg(UART_THR_OFFSET) = c;
}

char uart_getc(void) {
    while ((*reg(UART_LSR_OFFSET) & LSR_DR) == 0) {
        // attend
    }
    return *reg(UART_THR_OFFSET);
}


void uart_puts(const char *s) {
    while (*s) {
        uart_putc(*s++);
    }
}

void uart_put_uint(unsigned int n, const int base) {
    char buf[32];
    int i = 0;

    if (n == 0) {
        uart_putc('0');
        return;
    }

    while (n > 0) {
        const int digit = n % base;
        buf[i++] = (digit < 10) ? ('0' + digit) : ('a' + digit - 10);
        n /= base;
    }

    // les chiffres sont générés à l'envers, on les affiche en sens inverse
    while (i > 0) {
        uart_putc(buf[--i]);
    }
}

void uart_put_int(int n) {
    if (n < 0) {
        uart_putc('-');
        n = -n;
    }
    uart_put_uint((unsigned int)n, 10);
}

void uart_gets(char *buf, const int max_len) {
    int i = 0;

    while (i < max_len - 1) {
        const char c = uart_getc();

        if (c == '\r' || c == '\n') {
            uart_putc('\n');
            break;
        }
        if (c == 0x7F || c == 0x08) {
            if (i > 0) {
                i--;
                uart_putc('\b');
                uart_putc(' ');
                uart_putc('\b');
            }
            continue;
        }


        uart_putc(c);
        buf[i++] = c;
    }

    buf[i] = '\0';
}

void uart_put_ulong(unsigned long n, const int base) {
    char buf[32];
    int i = 0;

    if (n == 0) {
        uart_putc('0');
        return;
    }

    while (n > 0) {
        const int digit = n % base;
        buf[i++] = (digit < 10) ? ('0' + digit) : ('a' + digit - 10);
        n /= base;
    }

    while (i > 0) {
        uart_putc(buf[--i]);
    }
}

void kprintf(const char *fmt, ...) {
    va_list args;
    va_start(args, fmt);

    while (*fmt) {
        if (*fmt == '%') {
            fmt++;
            switch (*fmt) {
                case 'd': {
                    const int val = va_arg(args, int);
                    uart_put_int(val);
                    break;
                }
                case 'x': {
                    const unsigned int val = va_arg(args, unsigned int);
                    uart_put_uint(val, 16);
                    break;
                }
                case 's': {
                    const char *s = va_arg(args, char*);
                    uart_puts(s);
                    break;
                }
                case 'c': {
                    const char c = (char)va_arg(args, int);
                    uart_putc(c);
                    break;
                }
                case '%': {
                    uart_putc('%');
                    break;
                }
                case 'p': {
                    void *ptr = va_arg(args, void*);
                    uart_puts("0x");
                    uart_put_ulong((uintptr_t)ptr, 16);
                    break;
                }
                default:
                    uart_putc('%');
                    uart_putc(*fmt);
            }
        } else {
            uart_putc(*fmt);
        }
        fmt++;
    }

    va_end(args);
}
