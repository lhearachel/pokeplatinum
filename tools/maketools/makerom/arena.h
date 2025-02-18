#ifndef MEMORY_H
#define MEMORY_H

#include <setjmp.h>

#include "global.h"

typedef struct {
    u8 *buf;
    u32 capacity;
    u32 head;
    jmp_buf jmp;
} Arena;

// clang-format off
#define new(...)              newX(__VA_ARGS__, newN, newT)(__VA_ARGS__)
#define newX(a, b, c, d, ...) d
#define newT(a, T)            (T *)Alloc(a, sizeof(T), alignof(T), 1)
#define newN(a, T, n)         (T *)Alloc(a, sizeof(T), alignof(T), n)
// clang-format on

Arena NewArena(u32 capacity);
void *Alloc(Arena *a, u32 size, u32 align, u32 count);

#endif // MEMORY_H
