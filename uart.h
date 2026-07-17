#ifndef UART_H
#define UART_H

#include <stdint.h>

extern uint64_t uart_base;   // déclaration seulement, pas de valeur ici

void uart_init(uint64_t base);
void uart_putc(char c);
char uart_getc(void);
void uart_puts(const char *s);
void uart_gets(char *buf, int max_len);
void kprintf(const char *fmt, ...);

#endif