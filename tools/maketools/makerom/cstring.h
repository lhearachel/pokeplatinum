#ifndef CSTRING_H
#define CSTRING_H

#include <stddef.h>
#include <string.h>

#include "global.h"

typedef struct {
    char *data;
    size_t len;
} String;

typedef struct {
    String head;
    String tail;
} StringPair;

// clang-format off
#define String(...)            String_X(__VA_ARGS__, String_N, String_S, String_Z)(__VA_ARGS__)
#define String_X(a, b, c, ...) c
#define String_Z               (String) { 0 }
#define String_S(s)            (String) { s, lengthof(s) }
#define String_N(s, n)         (String) { s, n }
// clang-format on

bool String_Equals(String a, String b);
bool String_StartsWith(String s, String w);
StringPair String_Cut(String s, char delim);
void String_Advance(String *s, size_t n);

#endif // CSTRING_H
