#include "dataproc/minijson.h"

#include <stdbool.h>
#include <stddef.h>
#include <string.h>

static bool mj__is_number_cont(char c) {
    return (c >= '0' && c <= '9')
        ||  c == 'e' || c == 'E' || c == '.' || c == '-' || c == '+';
}

static bool mj__match_four(char *cur, char *end, char expect[4]) {
    if (cur + 4 >= end) return false;
    return strncmp(cur, expect, 4) == 0;
}

mj_value_t mj_read(mj_reader_t *r) {
    mj_value_t res;
    if (r->error) goto read_err;
    if (r->cur == r->end) { r->error = "unexpected eof"; goto read_err; }

    while (r->cur != r->end) {
        if      (*r->cur ==  ' ') r->cur++;
        else if (*r->cur == '\t') r->cur++;
        else if (*r->cur == '\r') r->cur++;
        else if (*r->cur == '\n') r->cur++;
        else break;
    }

    res.beg = r->cur;
    switch (*r->cur) {
    case ':': res.type = MJ_NAMESEP;  r->cur++; break;
    case ',': res.type = MJ_VALUESEP; r->cur++; break;

    case '-': case '0': case '1': case '2': case '3': case '4':
    case '5': case '6': case '7': case '8': case '9':
        res.type = MJ_NUMBER;
        while (r->cur != r->end && mj__is_number_cont(*r->cur)) { r->cur++; }
        break;

    case '"':
        // NOTE: string-type values "include" their opening and closing quotes;
        // to extract the inner-value, use beg+1 and end-1 as the bounds.
        res.type = MJ_STRING;
        r->cur++;
        for (;;) {
            if ( r->cur == r->end) { r->error = "unclosed string"; goto read_err; }
            if (*r->cur ==    '"') { r->cur++; break; }
            if (*r->cur ==   '\\') { r->cur++; }
            if ( r->cur != r->end) { r->cur++; }
        }
        break;

    case '{': case '[':
        res.type = (*r->cur == '{') ? MJ_OBJECT : MJ_ARRAY;
        res.depth = ++r->depth;
        r->cur++;
        break;

    case '}': case ']':
        res.type = (*r->cur == '}') ? MJ_OBJEND : MJ_ARREND;
        if (--r->depth < 0) {
            r->error = (*r->cur == '}') ? "stray '}'" : "stray ']'";
            goto read_err;
        }
        break;

    case 'n': case 't': case 'f':
        res.type = (*r->cur == 'n') ? MJ_NULL
                 : (*r->cur == 't') ? MJ_BOOL_TRUE : MJ_BOOL_FALSE;
        if (mj__match_four(r->cur,     r->end,  "null")) { r->cur += 4; break; }
        if (mj__match_four(r->cur,     r->end,  "true")) { r->cur += 4; break; }
        if (mj__match_four(r->cur + 1, r->end,  "alse")) { r->cur += 5; break; }
        // fallthrough

    default:
        r->error = "invalid token";
        goto read_err;
    }

    res.end = r->cur;
    return res;

read_err:
    return (mj_value_t) {
        .type = MJ_ERROR,
        .beg = r->cur,
        .end = r->cur,
    };
}

// Discard values still in the reader until we are at the desired depth. This is
// useful for iterating past other objects and arrays.
static void mj__discard_until(mj_reader_t *r, int depth) {
    mj_value_t val;
    val.type = MJ_NULL;
    while (r->depth != depth && val.type != MJ_ERROR) {
        val = mj_read(r);
        if (val.type == MJ_OBJEND || val.type == MJ_ARREND) r->cur++;
    }
}

bool mj_iter_array(mj_reader_t *r, mj_value_t *arr, mj_value_t *val) {
    mj__discard_until(r, arr->depth);

    *val = mj_read(r);
    if (val->type != MJ_NONE) { // handle commas for multi-entry arrays
        if (val->type == MJ_VALUESEP) {
            *val = mj_read(r);
            if (val->type == MJ_ARREND) { r->error = "unexpected end-of-array"; return false; }
        }
    }

    if (val->type == MJ_ERROR)    return false;
    if (val->type == MJ_ARREND)   { r->cur++; return false; }
    if (val->type == MJ_VALUESEP) { r->error = "missing value for array entry"; return false; }
    if (val->type == MJ_NAMESEP)  { r->error = "stray ':'"; return false; }
    if (val->type == MJ_OBJEND)   { r->error = "stray '}'"; return false; }

    return true;
}

bool mj_iter_object(mj_reader_t *r, mj_value_t *obj, mj_value_t *key, mj_value_t *val) {
    mj_value_t sep;
    mj__discard_until(r, obj->depth);

    *key = mj_read(r);
    if (val->type != MJ_NONE) { // handle commas for multi-member objects
        if (key->type == MJ_VALUESEP) {
            *key = mj_read(r);
            if (key->type == MJ_OBJEND) { r->error = "unexpected end-of-object"; return false; }
        }
    }

    if (key->type == MJ_ERROR)    return false;
    if (key->type == MJ_OBJEND)   { r->cur++; return false; }
    if (key->type == MJ_ARREND)   { r->error = "stray ']'"; return false; }
    if (key->type == MJ_NAMESEP)  { r->error = "stray ':'"; return false; }
    if (key->type == MJ_VALUESEP) { r->error = "stray ','"; return false; }
    if (key->type != MJ_STRING)   { r->error = "unexpected non-string name for object member"; return false; }

    sep = mj_read(r);
    if (sep.type == MJ_ERROR)    return false;
    if (sep.type == MJ_OBJEND)   { r->error = "unexpected end-of-object"; return false; }
    if (sep.type == MJ_ARREND)   { r->error = "stray ']'"; return false; }
    if (sep.type == MJ_VALUESEP) { r->error = "missing value for object member"; return false; }
    if (sep.type != MJ_NAMESEP)  { r->error = "missing ':' after name for object member"; return false; }

    *val = mj_read(r);
    if (val->type == MJ_ERROR)    return false;
    if (val->type == MJ_OBJEND)   { r->error = "unexpected end-of-object"; return false; }
    if (val->type == MJ_ARREND)   { r->error = "stray ']'"; return false; }
    if (val->type == MJ_NAMESEP)  { r->error = "stray ':'"; return false; }
    if (val->type == MJ_VALUESEP) { r->error = "missing value for object member"; return false; }

    return true;
}

#define descriptor(__article, __desc) { __article " " __desc, __desc }

static const struct {
    const char *w_article;
    const char *wo_article;
} type_descriptors[] = {
    [MJ_NONE]       = descriptor("an", "empty value"),
    [MJ_ERROR]      = descriptor("an", "error value"),
    [MJ_ARRAY]      = descriptor("an", "array"),
    [MJ_OBJECT]     = descriptor("an", "object"),
    [MJ_NUMBER]     = descriptor("a",  "number"),
    [MJ_STRING]     = descriptor("a",  "string"),
    [MJ_BOOL_FALSE] = descriptor("a",  "false-literal"),
    [MJ_BOOL_TRUE]  = descriptor("a",  "true-literal"),
    [MJ_NULL]       = descriptor("a",  "null-literal"),
    [MJ_NAMESEP]    = descriptor("a",  "name-separator (':')"),
    [MJ_VALUESEP]   = descriptor("a",  "value-separator (',')"),
    [MJ_OBJEND]     = descriptor("an", "object-terminator ('}')"),
    [MJ_ARREND]     = descriptor("an", "array-terminator (']')"),
};

const char* mj_desc_type(mj_type_t type) {
    if (type < MJ_NONE || type >= MJ_MAXTYPE) return NULL;
    return type_descriptors[type].wo_article;
}

const char* mj_desc_type_article(mj_type_t type) {
    if (type < MJ_NONE || type >= MJ_MAXTYPE) return NULL;
    return type_descriptors[type].w_article;
}
