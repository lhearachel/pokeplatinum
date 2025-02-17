#ifndef MAKEROM_H
#define MAKEROM_H

#include "cstring.h"
#include "global.h"
#include "romspec.h"

#define VARIABLE_COUNT 16

typedef struct {
    String name;
    String value;
} Variable;

typedef struct {
    void *mem;
    size_t start;
    size_t offset;
    size_t capacity;
} Memory;

extern Variable gVariables[VARIABLE_COUNT];
extern bool gVerbose;
extern String gSpecFileName;
extern RomSpec *gRomSpec;

void Die(char *fmt, ...);
void DebugPrint(char *fmt, ...);

#endif // MAKEROM_H
