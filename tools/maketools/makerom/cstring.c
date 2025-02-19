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

    for (int i = tail.len - 1; i >= 0; i--) {
        if (tail.data[i] == delim) {
            head = String(s.data, i);
            tail = String(s.data + i + 1, s.len - i - 1);
            break;
        }
    }

    return (StringPair) { head, tail };
}

static String _JoinInternal(String s, String t, char delim, char *join, size_t joinLen)
{
    join[s.len] = delim;
    join[joinLen] = '\0';
    memcpy(join, s.data, s.len);
    memcpy(join + s.len + 1, t.data, t.len);

    return String(join, joinLen);
}

String String_Join(String s, String t, char delim)
{
    size_t joinLen = s.len + t.len + 1;
    char *join = malloc(joinLen + 1);
    return _JoinInternal(s, t, delim, join, joinLen);
}

String String_JoinA(String s, String t, char delim, Arena *a)
{
    size_t joinLen = s.len + t.len + 1;
    char *join = new (a, char, joinLen + 1);
    return _JoinInternal(s, t, delim, join, joinLen);
}

String String_Copy(String s)
{
    char *copy = malloc(s.len + 1);
    memcpy(copy, s.data, s.len);
    copy[s.len] = '\0';

    return String(copy, s.len);
}

String String_Clone(String s, char *buf)
{
    memcpy(buf, s.data, s.len);
    buf[s.len] = '\0';
    return String(buf, s.len);
}

void String_Advance(String *s, size_t n)
{
    s->data += n;
    s->len -= n;
}

// Lowercase the character, if it is an uppercase alphabetic.
static char TryLower(char c)
{
    return (c >= 'A' && c <= 'Z') ? c + ('a' - 'A') : c;
}

int String_CompareIgnoreCase(String s, String t)
{
    size_t len = s.len < t.len ? s.len : t.len;
    for (size_t i = 0; i < len; i++) {
        char sc = TryLower(s.data[i]);
        char tc = TryLower(t.data[i]);
        if (sc != tc) {
            return sc - tc;
        }
    }

    if (s.len < t.len) {
        return -1;
    } else if (s.len > t.len) {
        return 1;
    } else {
        return 0;
    }
}
