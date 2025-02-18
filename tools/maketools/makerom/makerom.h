#ifndef MAKEROM_H
#define MAKEROM_H

#include "arena.h"
#include "cstring.h"
#include "global.h"
#include "romspec.h"

#define VARIABLE_COUNT 16

typedef struct {
    String name;
    String value;
} Variable;

extern Variable gVariables[VARIABLE_COUNT];
extern bool gVerbose;
extern String gSpecFileName;
extern RomSpec *gRomSpec;
extern Arena gArena;

void Die(char *fmt, ...);
void DebugPrint(char *fmt, ...);

#endif // MAKEROM_H
