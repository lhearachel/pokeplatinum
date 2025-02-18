#ifndef CSTRING_H
#define CSTRING_H

#include <stddef.h>
#include <string.h>

#include "arena.h"
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
StringPair String_CutBack(String s, char delim);

// Join the strings S and T, separating them with the specified delimiter.
// This function performs a dynamic allocation, and the calling client is
// responsible for freeing it.
String String_Join(String s, String t, char delim);

// Join the strings S and T, separating them with the specified delimiter.
// This variant allocates the necessary buffer space on the given arena.
String String_JoinA(String s, String t, char delim, Arena *a);

// Copy the string S. This function performs a dynamic allocation, and the
// calling client is responsible for freeing it.
String String_Copy(String s);

// Clone the string S into the requested buffer, then wrap the buffer into
// a new String struct.
String String_Clone(String s, char *buf);

void String_Advance(String *s, size_t n);

#endif // CSTRING_H
