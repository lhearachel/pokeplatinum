#include "cstring.h"

#include <stdlib.h>

bool String_Equals(String a, String b)
{
    if (a.len != b.len) {
        return false;
    }

    for (size_t i = 0; i < a.len; i++) {
        if (a.data[i] != b.data[i]) {
            return false;
        }
    }

    return true;
}

bool String_StartsWith(String s, String w)
{
    if (s.len < w.len) {
        return false;
    }

    for (size_t i = 0; i < w.len; i++) {
        if (s.data[i] != w.data[i]) {
            return false;
        }
    }

    return true;
}

StringPair String_Cut(String s, char delim)
{
    String head = s;
    String tail = String_Z;

    for (size_t i = 0; i < head.len; i++) {
        if (head.data[i] == delim) {
            tail.data = head.data + i + 1;
            tail.len = head.len - i - 1;
            head.len = i;
            break;
        }
    }

    return (StringPair) { head, tail };
}

StringPair String_CutBack(String s, char delim)
{
    String head = String_Z;
    String tail = s;

    for (size_t i = tail.len - 1; i >= 0; i--) {
        if (tail.data[i] == delim) {
            head = String(s.data, i - 1);
            tail = String(s.data + i + 1, s.len - i - 1);
        }
    }

    return (StringPair) { head, tail };
}

String String_Join(String a, String b, char delim)
{
    size_t joinLen = a.len + b.len + 1;
    char *join = malloc(joinLen + 1);

    join[a.len] = delim;
    join[joinLen] = '\0';
    memcpy(join, a.data, a.len);
    memcpy(join + a.len + 1, b.data, b.len);

    return String(join, joinLen);
}

String String_Copy(String s)
{
    char *copy = malloc(s.len + 1);
    memcpy(copy, s.data, s.len);
    copy[s.len] = '\0';

    return String(copy, s.len);
}

void String_Advance(String *s, size_t n)
{
    s->data += n;
    s->len -= n;
}
