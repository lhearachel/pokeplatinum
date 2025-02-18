#ifndef ROMSPEC_H
#define ROMSPEC_H

#include "cstring.h"
#include "global.h"

typedef struct {
    String pathStatic;
    String pathOverlayDefs;
    String pathOverlayTable;
    String pathElf;
} Arm;

typedef struct {
    String gameTitle;
    String gameCode;
    String makerCode;
    u8 remasterVersion;
    u8 romSize;
    u16 secureDelay;
    bool padToSize;
} Properties;

typedef struct {
    String rootLocal;
    String rootRom;
    String path;
} File;

typedef struct {
    Arm binary9;
    Arm binary7;
    Properties properties;
    File files[ROMSPEC_FILESYS_ENTRIES];
    size_t numFiles;
} RomSpec;

void ParseSpec(String spec);

#endif // ROMSPEC_H
