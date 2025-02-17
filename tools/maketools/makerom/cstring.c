#include "cstring.h"

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

void String_Advance(String *s, size_t n)
{
    s->data += n;
    s->len -= n;
}
