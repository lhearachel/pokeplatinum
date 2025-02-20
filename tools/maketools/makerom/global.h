#ifndef GLOBAL_H
#define GLOBAL_H

#include <stdint.h>

typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef unsigned int bool;

#define false 0
#define true  1

#define alignof(T)  _Alignof(T)
#define countof(a)  (sizeof(a) / sizeof(*(a)))
#define lengthof(s) (countof(s) - 1)

#endif // GLOBAL_H
