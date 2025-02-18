#include "makerom.h"

#include <setjmp.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "arena.h"
#include "cstring.h"
#include "global.h"
#include "romspec.h"

#define ROM_HEADER_SIZE        0x4000
#define NINTENDO_LOGO_CHECKSUM 0xCF56

Variable gVariables[VARIABLE_COUNT] = { 0 };
bool gVerbose = false;
String gSpecFileName = String_Z;
RomSpec *gRomSpec = NULL;
Arena gArena = { 0 };

// This contains the actual strings extracted from the romspec file. It is a continuous buffer,
// with individual strings pointed to from the outside and separated by their null-terminators.
static char sRomSpecStrings[16000000] = { 0 };

// These are kept as raw strings because their length never matters.
static char *sHeaderTemplatePath = NULL;
static char *sBannerPath = NULL;
static char *sWorkingDirectory = ".";

static void Usage(FILE *stream)
{
    fprintf(stream, "Usage: makerom [-v | --verbose] [OPTIONS] SPECFILE\n\n");
    fprintf(stream, "Options:\n");
    fprintf(stream, "  -v, --verbose         Emit verbose output during program operation.\n");
    fprintf(stream, "  -C, --directory DIR   Change the current working directory to DIR after\n");
    fprintf(stream, "                        parsing SPECFILE and before any further work is done.\n");
    fprintf(stream, "  -T, --template FILE   Take the contents of FILE as an initial state for the\n");
    fprintf(stream, "                        output ROM's header.\n");
    fprintf(stream, "  -B, --banner FILE     Take the contents of FILE as the banner data for the\n");
    fprintf(stream, "                        output ROM.\n");
    fprintf(stream, "  -D VAR=VALUE          Assign VALUE to an internal variable VAR. Variables\n");
    fprintf(stream, "                        used in the input SPECFILE will be substituted for\n");
    fprintf(stream, "                        assigned values at runtime.\n");
}

static void DieUsage(char *fmt, ...)
{
    va_list args;
    va_start(args, fmt);

    fprintf(stderr, "makerom: ");
    vfprintf(stderr, fmt, args);
    fprintf(stderr, "\n\n");
    Usage(stderr);

    va_end(args);
    exit(EXIT_FAILURE);
}

static void ParseVariableDef(String s)
{
    StringPair pair = String_Cut(s, '=');

    size_t i;
    for (i = 0; i < VARIABLE_COUNT; i++) {
        if (gVariables[i].name.data == NULL) {
            gVariables[i].name = pair.head;
            gVariables[i].value = pair.tail;
            DebugPrint("Variable: “%.*s” = “%.*s”",
                (int)gVariables[i].name.len,
                gVariables[i].name.data,
                (int)gVariables[i].value.len,
                gVariables[i].value.data);
            return;
        }

        if (String_Equals(gVariables[i].name, pair.head)) {
            DieUsage("Variable “%.*s” already defined", gVariables[i].name.len, gVariables[i].name.data);
        }
    }

    DieUsage("Too many variable definitions");
}

static void LoadHeaderTemplate(char *path)
{
    FILE *f = fopen(path, "rb");
    if (!f) {
        DieUsage("Could not open header template file “%s”", path);
        return;
    }
}

static void ParseArgv(int argc, char **argv)
{
    argc--, argv++;
    if (argc == 0 || strncmp(*argv, "-h", 2) == 0 || strncmp(*argv, "--help", 6) == 0) {
        Usage(stdout);
        exit(EXIT_SUCCESS);
    }

    if (strncmp(*argv, "-v", 2) == 0 || strncmp(*argv, "--verbose", 9) == 0) {
        argc--, argv++;
        gVerbose = true;
    }

    for (; argc > 0; argc--, argv++) {
        char *opt = *argv;
        if (opt[0] != '-') {
            break;
        }

        if (opt[1] == '-' && opt[2] == '\0') {
            argc--, argv++;
            break;
        }

        if (opt[1] == 'D') {
            opt = opt + 2;
            if (*opt == '\0') {
                argc--;
                argv++;
                opt = *argv;
            }

            ParseVariableDef(String(opt, strlen(opt)));
        } else if (opt[1] == 'T' || strncmp(opt + 1, "-template", 9) == 0) {
            argc--;
            argv++;
            sHeaderTemplatePath = *argv;
        } else if (opt[1] == 'B' || strncmp(opt + 1, "-banner", 7) == 0) {
            argc--;
            argv++;
            sBannerPath = *argv;
        } else if (opt[1] == 'C' || strncmp(opt + 1, "-directory", 10) == 0) {
            argc--;
            argv++;
            sWorkingDirectory = *argv;
        } else {
            DieUsage("Unrecognized option “%s”", opt);
        }
    }

    if (argc == 0) {
        DieUsage("Missing positional argument “SPECFILE”");
    }

    gSpecFileName = String(*argv, strlen(*argv));
    DebugPrint("Specfile: “%.*s”", (int)gSpecFileName.len, gSpecFileName.data);
}

static String ReadWholeFile(String fname)
{
    FILE *f = fopen(fname.data, "rb");
    if (!f) {
        Die("Could not read SPECFILE “%s”", gSpecFileName.data);
    }

    fseek(f, 0, SEEK_END);
    size_t fsize = ftell(f);
    rewind(f);

    String content = String(malloc(fsize + 1), fsize);
    if (!content.data) {
        Die("Could not allocate file stream for SPECFILE “%s”", gSpecFileName.data);
    }

    fread(content.data, 1, fsize, f);
    fclose(f);
    content.data[fsize] = '\0';

    return content;
}

/* static void InitHeader(void) */
/* { */
/*     Properties *properties = &gRomSpec->properties; */
/*     memcpy(sRomHeader + 0x00, properties->gameTitle.data, properties->gameTitle.len); */
/*     memcpy(sRomHeader + 0x0C, properties->gameCode.data, properties->gameCode.len); */
/*     memcpy(sRomHeader + 0x10, properties->makerCode.data, properties->makerCode.len); */
/*     // byte 0x12 -> Unit Code (00=NDS, 02=NDS+DSi, 03=DSi) */
/*     // byte 0x13 -> Encryption seed selection (00..07) */
/*     sRomHeader[0x14] = properties->romSize; // set to N, actual chip size is 128KB << N (roughly 1 Megabit) */
/*     // 0x15..0x1B -> reserved */
/*     // 0x1C -> reserved on NDS (used on DSi) */
/*     // 0x1D -> NDS region (0x00=global, 0x80=China, 0x40=Kora) */
/*     sRomHeader[0x1E] = properties->remasterVersion; */
/*     // 0x1F -> Autostart (bit 2, specifically; skips Press Button after Health and Safety screen) */
/*     // 0x20..0x5C -> offsets for stuff */
/*     // 0x60 -> port 0x040001A4 setting for normal commands (usually 0x00586000) */
/*     // 0x64 -> port 0x040001A4 setting for KEY1 commands (usually 0x001808F8) */
/*     // 0x68 -> banner offset */
/*     // 0x6C -> secure area CRC-16 (0x00000020..0x00007FFF) */
/*     sRomHeader[0x6E] = properties->secureDelay & 0xFF; */
/*     sRomHeader[0x6F] = (properties->secureDelay >> 8) & 0xFF; */
/*     // 0x70 -> ARM9 auto load list hook RAM address */
/*     // 0x74 -> ARM7 auto load list hook RAM address */
/*     // 0x78 -> Disable Secure Area flag */
/*     // 0x80 -> Total used ROM size */
/*     sRomHeader[0x84] = ROM_HEADER_SIZE & 0xFF; */
/*     sRomHeader[0x85] = (ROM_HEADER_SIZE >> 8) & 0xFF; */
/*     sRomHeader[0x86] = (ROM_HEADER_SIZE >> 16) & 0xFF; */
/*     sRomHeader[0x87] = (ROM_HEADER_SIZE >> 24) & 0xFF; */
/*     // 0x88 -> unknown */
/*     // 0x8C -> reserved */
/*     // 0x94 -> NAND end of ROM area */
/*     // 0x96 -> NAND start of RW area */
/*     // 0x98 -> reserved */
/*     // 0xB0 -> reserved */
/*     // 0xC0 -> Nintendo logo (compressed bitmap) */
/*     sRomHeader[0x15C] = NINTENDO_LOGO_CHECKSUM & 0xFF; */
/*     sRomHeader[0x15D] = (NINTENDO_LOGO_CHECKSUM >> 8) & 0xFF; */
/*     // 0x15E -> header CRC-16 (0x0000..0x015D) */
/*     // remaining is debug information or reserved areas */
/* } */

int main(int argc, char **argv)
{
    ParseArgv(argc, argv);

    String spec = ReadWholeFile(gSpecFileName);

    jmp_buf main;
    if (setjmp(main)) {
        Die("gArena signaled OOM");
    }

    gRomSpec = calloc(1, sizeof(RomSpec));
    gArena = (Arena) {
        .buf = (u8 *)sRomSpecStrings,
        .capacity = countof(sRomSpecStrings),
    };
    memcpy(gArena.jmp, main, sizeof(jmp_buf));

    ParseSpec(spec);
    free(spec.data);
    for (size_t i = 0; i < gRomSpec->numFiles; i++) {
        printf("%s/%s\n", gRomSpec->files[i].rootRom.data, gRomSpec->files[i].path.data);
    }
    /* InitHeader(); */

    free(gRomSpec);
    exit(EXIT_SUCCESS);
}

void Die(char *fmt, ...)
{
    va_list args;
    va_start(args, fmt);

    fprintf(stderr, "makerom: ");
    vfprintf(stderr, fmt, args);
    fprintf(stderr, "\n");

    va_end(args);
    exit(EXIT_FAILURE);
}

void DebugPrint(char *fmt, ...)
{
    if (gVerbose) {
        va_list args;
        va_start(args, fmt);
        printf("[DEBUG] ");
        vprintf(fmt, args);
        printf("\n");
        va_end(args);
    }
}
