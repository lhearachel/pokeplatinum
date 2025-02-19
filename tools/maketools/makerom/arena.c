#include "arena.h"

#include <stdlib.h>

Arena NewArena(u32 capacity)
{
    Arena a = { 0 };
    a.buf = calloc(capacity, sizeof(u8));
    a.capacity = a.buf ? capacity : 0;
    return a;
}

void *Alloc(Arena *a, u32 size, u32 align, u32 count)
{
    int padding = -a->head & (align - 1);
    int available = a->capacity - a->head - padding;
    if (available < 0 || count > available / size) {
        longjmp(a->jmp, 1);
    }

    void *p = a->buf + a->head + padding;
    a->head += padding + count * size;
    return p;
}
