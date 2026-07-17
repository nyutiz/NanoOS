#pragma once
#include <stddef.h>

int memcmp(const void *cs, const void *ct, size_t count);
void *memmove(void *dest, const void *src, size_t count);
void *memchr(const void *s, int c, size_t n);
size_t strlen(const char *s);
void *memcpy(void *dest, const void *src, size_t count);
size_t strnlen(const char *s, size_t count);
char *strrchr(const char *s, int c);