#include "romspec.h"

#include <stdlib.h>

#include "cstring.h"
#include "makerom.h"

static String sKeyArm9 = String("Arm9");
static String sKeyArm7 = String("Arm7");
static String sKeyProperty = String("Property");
static String sKeyRomSpec = String("RomSpec");

static String sKeyStatic = String("Static");
static String sKeyOverlayDefs = String("OverlayDefs");
static String sKeyOverlayTable = String("OverlayTable");
static String sKeyElf = String("Elf");
static String sKeyNef = String("Nef");
static String sKeyGameTitle = String("GameTitle");
static String sKeyGameCode = String("GameCode");
static String sKeyMakerCode = String("MakerCode");
static String sKeyRemasterVersion = String("RemasterVersion");
static String sKeySecureDelayType = String("SecureDelayType");
static String sKeyRomSize = String("RomSize");
static String sKeyPadRomToSize = String("PadRomToSize");
static String sKeySegment = String("Segment"); // TODO: What does this do?
static String sKeyRomRoot = String("RomRoot");
static String sKeyLocalRoot = String("LocalRoot");
static String sKeyFile = String("File");

static String sSegment = String_Z;
static String sRomRoot = String_Z;
static String sLocalRoot = String_Z;

// clang-format off
static String sRomSizeTable[] = {
    String("1Mb"),
    String("2Mb"),
    String("4Mb"),
    String("8Mb"),
    String("16Mb"),
    String("32Mb"),
    String("64Mb"),
    String("128Mb"),
    String("256Mb"),
    String("512Mb"),
    String("1Gb"),
    String("2Gb"),
    String("4Gb"),
    String("8Gb"),
    String("16Gb"),
    String("32Gb"),
    String_Z,
};

static struct { String key; u16 val; } sSecureDelayTypeTable[] = {
    { String("1TROM"), 0x0D7E },
    { String("MROM"),  0x051E },
    { String_Z,              0x0000 },
};
// clang-format on

static size_t sLineNumber = 1;

#define SWALLOW_WHITESPACE(s)                \
    while (IsSpace(*(s)->data)) {            \
        sLineNumber += (*(s)->data == '\n'); \
        String_Advance(s, 1);                \
    }

static bool TakePropertyKey(String *s, String key)
{
    DebugPrint("-- Checking for property key “%.*s”", (int)key.len, key.data, (int)key.len, s->data);
    if (!String_MatchesWord(*s, key)) {
        return false;
    }

    String_Advance(s, key.len + 1);
    SWALLOW_WHITESPACE(s);
    return true;
}

static bool TakeSectionHeader(String *s, String key)
{
    DebugPrint("-- Checking for section header “%.*s”", (int)key.len, key.data, (int)key.len, s->data);
    if (!String_MatchesWord(*s, key)) {
        return false;
    }

    String_Advance(s, key.len + 1);
    SWALLOW_WHITESPACE(s);
    if (*s->data != '{') {
        Die("Expected open-brace “{” at section start, but found “%c”", *s->data);
    }

    String_Advance(s, 1);
    return true;
}

typedef void (*ParseFunc)(String *spec);
static void ParseArm9Section(String *spec);
static void ParseArm7Section(String *spec);
static void ParsePropertySection(String *spec);
static void ParseRomSpecSection(String *spec);

void ParseSpec(String spec)
{
    SWALLOW_WHITESPACE(&spec);
    while (spec.len > 0 && *spec.data) {
        // '#' is the comment leader. In-line comments are not allowed (for simplicity).
        if (*spec.data == '#') {
            for (spec.data++, spec.len--; *spec.data != '\n'; spec.data++, spec.len--) {
            }
            continue;
        }

        ParseFunc sectionParser;
        if (TakeSectionHeader(&spec, sKeyArm9)) {
            sectionParser = ParseArm9Section;
        } else if (TakeSectionHeader(&spec, sKeyArm7)) {
            sectionParser = ParseArm7Section;
        } else if (TakeSectionHeader(&spec, sKeyProperty)) {
            sectionParser = ParsePropertySection;
        } else if (TakeSectionHeader(&spec, sKeyRomSpec)) {
            sectionParser = ParseRomSpecSection;
        } else {
            DebugPrint("No match, remaining chars: %u", spec.len);
            Die("Line %u: Expected one of “Arm9”, “Arm7”, “Property”, or “RomSpec”", sLineNumber);
            return; // Unreachable; only notifies the compiler that sectionParser does not go uninitialized.
        }

        sectionParser(&spec);
        SWALLOW_WHITESPACE(&spec);
    }

    DebugPrint("No more file data, exiting...");
}

static void TrimTrailingSpaces(String *s)
{
    while (s->len > 0 && IsSpace(s->data[s->len - 1])) {
        s->len--;
    }
}

static void StripQuotes(String *s)
{
    if (s->len > 0 && s->data[0] == '"') {
        if (s->data[s->len - 1] != '"') {
            Die("Line %u: Unterminated string literal; did you forget a closing-quote?", sLineNumber);
        }

        String_Advance(s, 1);
        s->len--;
    }
}

static String TakeStringValue(String *s)
{
    String value = { s->data, 0 };
    while (*s->data != '\n') {
        value.len++;
        String_Advance(s, 1);
    }

    TrimTrailingSpaces(&value);
    StripQuotes(&value);
    return value;
}

static String ExpandString(String *s)
{
    String value = TakeStringValue(s);

    size_t len = 0;
    char *buf = new (&gArena, char, 0); // Request 0 and update later.
    for (size_t i = 0; i < value.len; i++) {
        if (value.data[i] == '$' && value.data[i + 1] == '(') {
            i += 2;

            // Extract the variable name.
            int close = String_FindC(String(value.data + i, value.len - i), ')');
            if (close < 0) {
                Die("Line %u: Unterminated variable-substitution; did you forget a “)”?", sLineNumber);
                return String_Z;
            }
            String var = String(value.data + i, close);

            // Find matching variable name.
            size_t j;
            for (j = 0; j < VARIABLE_COUNT && gVariables[j].name.len > 0; j++) {
                if (String_Equals(gVariables[j].name, var)) {
                    // substitute the value of the variable into the arena buffer
                    memcpy(buf + len, gVariables[j].value.data, gVariables[j].value.len);
                    len += gVariables[j].value.len;
                    i += close + 1;
                    break;
                }
            }

            if (j == VARIABLE_COUNT || gVariables[j].name.len == 0) {
                // No matching variable found; die.
                Die("Line %u: Unrecognized variable name: “%.*s”", sLineNumber, (int)var.len, var.data);
            }
        } else {
            buf[len++] = value.data[i];
        }
    }

    buf[len++] = '\0';
    gArena.head += len;
    return String(buf, len);
}

static int ParseNumber(String *s)
{
    int pow = 1;
    int result = 0;
    for (int i = s->len - 1; i >= 0; i--) {
        if (s->data[i] < '0' || s->data[i] > '9') {
            Die("Line %u: Unexpected non-numeric character “%c”", sLineNumber, s->data[i]);
            return -1;
        }

        int n = s->data[i] - '0';
        result += (n * pow);
        pow *= 10;
    }

    return result;
}

static bool ParseBoolean(String *s)
{
    if (String_MatchesWord(*s, String("TRUE"))) {
        return true;
    }

    if (String_MatchesWord(*s, String("FALSE"))) {
        return false;
    }

    Die("Line %u: Unexpected non-boolean string value “%.*s”",
        sLineNumber,
        (int)s->len,
        s->data);
    return false;
}

static u16 ParseSecureDelayType(String *s)
{
    for (size_t i = 0; i < lengthof(sSecureDelayTypeTable); i++) {
        if (String_Equals(*s, sSecureDelayTypeTable[i].key)) {
            return sSecureDelayTypeTable[i].val;
        }
    }

    Die("Line %u: Unrecognized value “%.*s” for key “SecureDelayType”",
        sLineNumber,
        (int)s->len,
        s->data);
    return -1;
}

static u8 ParseRomSize(String *s)
{
    for (u8 i = 0; i < lengthof(sRomSizeTable); i++) {
        if (String_Equals(*s, sRomSizeTable[i])) {
            return i;
        }
    }

    Die("Line %u: Unrecognized value “%.*s” for key “RomSize”",
        sLineNumber,
        (int)s->len,
        s->data);
    return -1;
}

static File ParseFilePath(String *s)
{
    // parent directory path + basename
    StringPair entryPair = String_CutBack(*s, '/');
    String romParent;
    String localParent;

    // If the file-path has a parent directory, concatenate it with the current
    // ROM root and local root to produce a canonical parent for either case.
    if (entryPair.head.len > 0) {
        romParent = String_JoinA(sRomRoot, entryPair.head, '/', &gArena);
        localParent = String_JoinA(sLocalRoot, entryPair.head, '/', &gArena);
    } else {
        char *romParentBuf = new (&gArena, char, sRomRoot.len + 1);
        char *localParentBuf = new (&gArena, char, sLocalRoot.len + 1);
        romParent = String_Clone(sRomRoot, romParentBuf);
        localParent = String_Clone(sLocalRoot, localParentBuf);
    }

    char *basenameBuf = new (&gArena, char, entryPair.tail.len + 1);
    String basename = String_Clone(entryPair.tail, basenameBuf);
    return (File) {
        .rootLocal = localParent,
        .rootRom = romParent,
        .path = basename,
    };
}

static void ParseArm9Section(String *spec)
{
    DebugPrint("Entered Arm9 section...");
    SWALLOW_WHITESPACE(spec);
    while (*spec->data != '}') {
        if (*spec->data == '\0') {
            Die("Unexpected end-of-stream while parsing Arm9 section");
        }

        String *target = NULL;
        String *targetKey = NULL;
        if (TakePropertyKey(spec, sKeyStatic)) {
            gRomSpec->binary9.pathStatic = ExpandString(spec);
            target = &gRomSpec->binary9.pathStatic;
            targetKey = &sKeyStatic;
        } else if (TakePropertyKey(spec, sKeyOverlayDefs)) {
            gRomSpec->binary9.pathOverlayDefs = ExpandString(spec);
            target = &gRomSpec->binary9.pathOverlayDefs;
            targetKey = &sKeyOverlayDefs;
        } else if (TakePropertyKey(spec, sKeyOverlayTable)) {
            gRomSpec->binary9.pathOverlayTable = ExpandString(spec);
            target = &gRomSpec->binary9.pathOverlayTable;
            targetKey = &sKeyOverlayTable;
        } else if (TakePropertyKey(spec, sKeyElf) || TakePropertyKey(spec, sKeyNef)) {
            gRomSpec->binary9.pathElf = ExpandString(spec); // Not important to keep, apparently
            target = &gRomSpec->binary9.pathElf;
            targetKey = &sKeyElf;
        } else {
            Die("Line %u: Unrecognized property key", sLineNumber);
        }

        DebugPrint("-- Key “%.*s” = “%.*s”",
            (int)targetKey->len,
            targetKey->data,
            (int)target->len,
            target->data);
        SWALLOW_WHITESPACE(spec);
    }

    String_Advance(spec, 1);
    DebugPrint("Exiting Arm9 section...");
}

static void ParseArm7Section(String *spec)
{
    DebugPrint("Entered Arm7 section...");
    SWALLOW_WHITESPACE(spec);
    while (*spec->data != '}') {
        if (*spec->data == '\0') {
            Die("Unexpected end-of-stream while parsing Arm7 section");
        }

        String *target = NULL;
        String *targetKey = NULL;
        if (TakePropertyKey(spec, sKeyStatic)) {
            gRomSpec->binary7.pathStatic = ExpandString(spec);
            target = &gRomSpec->binary7.pathStatic;
            targetKey = &sKeyStatic;
        } else if (TakePropertyKey(spec, sKeyOverlayDefs)) {
            gRomSpec->binary7.pathOverlayDefs = ExpandString(spec);
            target = &gRomSpec->binary7.pathOverlayDefs;
            targetKey = &sKeyOverlayDefs;
        } else if (TakePropertyKey(spec, sKeyOverlayTable)) {
            gRomSpec->binary7.pathOverlayTable = ExpandString(spec);
            target = &gRomSpec->binary7.pathOverlayTable;
            targetKey = &sKeyOverlayTable;
        } else if (TakePropertyKey(spec, sKeyElf) || TakePropertyKey(spec, sKeyNef)) {
            gRomSpec->binary7.pathElf = ExpandString(spec);
            target = &gRomSpec->binary7.pathElf;
            targetKey = &sKeyElf;
        } else {
            Die("Line %u: Unrecognized property key", sLineNumber);
        }

        DebugPrint("-- Key “%.*s” = “%.*s”",
            (int)targetKey->len,
            targetKey->data,
            (int)target->len,
            target->data);
        SWALLOW_WHITESPACE(spec);
    }

    String_Advance(spec, 1);
    DebugPrint("Exiting Arm7 section...");
}

static void ParsePropertySection(String *spec)
{
    DebugPrint("Entered Property section...");
    SWALLOW_WHITESPACE(spec);
    while (*spec->data != '}') {
        if (*spec->data == '\0') {
            Die("Unexpected end-of-stream while parsing Property section");
        }

        String *target = NULL;
        String *targetKey = NULL;
        if (TakePropertyKey(spec, sKeyGameTitle)) {
            gRomSpec->properties.gameTitle = ExpandString(spec);
            if (gRomSpec->properties.gameTitle.len > 12) {
                DebugPrint("-- Value for “GameTitle” = “%.*s” is longer than 12 characters; truncating...",
                    (int)gRomSpec->properties.gameTitle.len,
                    gRomSpec->properties.gameTitle.data);
                gRomSpec->properties.gameTitle.len = 12;
            }

            target = &gRomSpec->properties.gameTitle;
            targetKey = &sKeyGameTitle;
        } else if (TakePropertyKey(spec, sKeyGameCode)) {
            gRomSpec->properties.gameCode = ExpandString(spec);
            if (gRomSpec->properties.gameCode.len > 4) {
                DebugPrint("-- Value for “GameCode” = “%.*s” is longer than 4 characters; truncating...",
                    (int)gRomSpec->properties.gameCode.len,
                    gRomSpec->properties.gameCode.data);
                gRomSpec->properties.gameCode.len = 4;
            }

            target = &gRomSpec->properties.gameCode;
            targetKey = &sKeyGameCode;
        } else if (TakePropertyKey(spec, sKeyMakerCode)) {
            gRomSpec->properties.makerCode = ExpandString(spec);
            if (gRomSpec->properties.makerCode.len > 2) {
                DebugPrint("-- Value for “MakerCode” = “%.*s” is longer than 2 characters; truncating...",
                    (int)gRomSpec->properties.makerCode.len,
                    gRomSpec->properties.makerCode.data);
                gRomSpec->properties.makerCode.len = 2;
            }

            target = &gRomSpec->properties.makerCode;
            targetKey = &sKeyMakerCode;
        } else if (TakePropertyKey(spec, sKeyRemasterVersion)) {
            String tmp = TakeStringValue(spec);
            gRomSpec->properties.remasterVersion = ParseNumber(&tmp);
            DebugPrint("-- Key “RemasterVersion” = %u", gRomSpec->properties.remasterVersion);
        } else if (TakePropertyKey(spec, sKeySecureDelayType)) {
            String tmp = TakeStringValue(spec);
            gRomSpec->properties.secureDelay = ParseSecureDelayType(&tmp);
            DebugPrint("-- Key “RomSpeedType” = “%.*s”", (int)tmp.len, tmp.data);
        } else if (TakePropertyKey(spec, sKeyRomSize)) {
            String tmp = TakeStringValue(spec);
            gRomSpec->properties.romSize = ParseRomSize(&tmp);
            DebugPrint("-- Key “RomSize” = “%.*s”", (int)tmp.len, tmp.data);
        } else if (TakePropertyKey(spec, sKeyPadRomToSize)) {
            String tmp = TakeStringValue(spec);
            gRomSpec->properties.padToSize = ParseBoolean(&tmp);
            DebugPrint("-- Key “RomFootPadding” = %s", gRomSpec->properties.padToSize ? "true" : "false");
        } else {
            Die("Line %u: Unrecognized property key", sLineNumber);
        }

        if (targetKey) {
            DebugPrint("-- Key “%.*s” = “%.*s”",
                (int)targetKey->len,
                targetKey->data,
                (int)target->len,
                target->data);
        }
        SWALLOW_WHITESPACE(spec);
    }

    String_Advance(spec, 1);
    DebugPrint("Exiting Property section...");
}

static void ParseRomSpecSection(String *spec)
{
    DebugPrint("Entered RomSpec section...");
    SWALLOW_WHITESPACE(spec);
    while (*spec->data != '}') {
        if (*spec->data == '\0') {
            Die("Unexpected end-of-stream while parsing RomSpec section");
        }

        String *target = NULL;
        String *targetKey = NULL;
        if (TakePropertyKey(spec, sKeySegment)) {
            sSegment = TakeStringValue(spec);
            target = &sSegment;
            targetKey = &sKeySegment;
        } else if (TakePropertyKey(spec, sKeyRomRoot)) {
            sRomRoot = TakeStringValue(spec);
            if (sRomRoot.data[sRomRoot.len - 1] == '/') {
                sRomRoot.len--; // Trim the trailing '/'.
            }

            target = &sRomRoot;
            targetKey = &sKeyRomRoot;
        } else if (TakePropertyKey(spec, sKeyLocalRoot)) {
            sLocalRoot = TakeStringValue(spec);
            target = &sLocalRoot;
            targetKey = &sKeyLocalRoot;
        } else if (TakePropertyKey(spec, sKeyFile)) {
            // TODO: Expand variables in File directive
            String path = TakeStringValue(spec);
            gRomSpec->files[gRomSpec->numFiles] = ParseFilePath(&path);
            target = &gRomSpec->files[gRomSpec->numFiles].path;
            targetKey = &sKeyFile;
            gRomSpec->numFiles++;
        }

        DebugPrint("-- Key “%.*s” = “%.*s”",
            (int)targetKey->len,
            targetKey->data,
            (int)target->len,
            target->data);
        SWALLOW_WHITESPACE(spec);
    }

    String_Advance(spec, 1);
    DebugPrint("Exiting RomSpec section...");
}
