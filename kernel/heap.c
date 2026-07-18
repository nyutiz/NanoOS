#include <stdint.h>
#include <stddef.h>
#include "../include/liballoc.h"

// Définis ces deux symboles dans ton linker.ld (voir plus bas)
extern char _heap_start;
extern char _heap_end;

static uintptr_t heap_ptr = 0;

int liballoc_lock() {
    return 0; // pas de concurrence pour l'instant, un seul cœur/thread actif
}

int liballoc_unlock() {
    return 0;
}

void* liballoc_alloc(int pages) {
    if (heap_ptr == 0) {
        heap_ptr = (uintptr_t)&_heap_start;
    }

    size_t size = (size_t)pages * 4096; // vérifie PAGESIZE dans liballoc.c, souvent 4096

    if (heap_ptr + size > (uintptr_t)&_heap_end) {
        return NULL; // heap épuisé
    }

    void* ptr = (void*)heap_ptr;
    heap_ptr += size;
    return ptr;
}

int liballoc_free(void* ptr, int pages) {
    (void)ptr;
    (void)pages;
    // Bump allocator "v1" : on ne récupère jamais les pages.
    // liballoc gère déjà le split/coalesce à l'intérieur des pages,
    // ça suffit largement pour démarrer. Tu pourras brancher un vrai
    // allocateur de pages plus tard (celui que tu construis actuellement).
    return 0;
}