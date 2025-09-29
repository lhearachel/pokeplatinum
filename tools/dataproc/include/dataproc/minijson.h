// Adapted from github.com/rxi/sj.h
//
// This code implements a SAX-style parser for the JSON specification described
// by RFC 8259. Only minimal validations are performed during the parse.

#ifndef DATAPROC_MINIJSON_H
#define DATAPROC_MINIJSON_H

#include <stdbool.h>
#include <stddef.h>
#include <string.h>

typedef enum mj_type_t {
    MJ_NONE,
    MJ_ERROR,

    MJ_ARRAY,
    MJ_OBJECT,
    MJ_NUMBER,
    MJ_STRING,
    MJ_BOOL_FALSE,
    MJ_BOOL_TRUE,
    MJ_NULL,

    MJ_NAMESEP,  // internal type for ':'
    MJ_VALUESEP, // internal type for ','
    MJ_OBJEND,   // internal type for '}'
    MJ_ARREND,   // internal type for ']'

    MJ_MAXTYPE,  // internal boundary-type
} mj_type_t;

typedef struct mj_reader_t {
    char *data;
    char *cur;
    char *end;
    int   depth;
    char *error;
    char *name;
} mj_reader_t;

typedef struct mj_value_t {
    mj_type_t type;
    char     *beg;
    char     *end;
    int       depth;
} mj_value_t;

typedef struct mj_pos_t {
    const char *line_beg;
    const char *line_end;

    int line;
    int col;
} mj_pos_t;

// Create a reader for a given JSON payload.
static inline mj_reader_t mj_new_reader(char *data, size_t len, char *name) {
    return (mj_reader_t) {
        .data = data,
        .cur  = data,
        .end  = data + len,
        .name = name,
    };
}

// Read a single value. Container-type values (arrays and objects) will stop at
// their opening-brace; further processing of their contents must be done using
// the `iter` functions below.
mj_value_t mj_read(mj_reader_t *r);

// Iterates over the contents of an array. The first call to this function for
// a given array should pass `val->type` as `MJ_NONE`.
//
// - If the end of the array is found, returns `false`.
// - If invalid JSON is encountered, returns `false` and populates `r->error`
//   with an appropriate message.
// - Otherwise, returns `true` and populates `val` with the next array element.
bool mj_iter_array(mj_reader_t *r, mj_value_t *arr, mj_value_t *val);

// Iterates over the contents of an object. The first call to this function for
// a given object should pass `val->type` as `MJ_NONE`.
//
// - If the end of the object is found, returns `false`.
// - If invalid JSON is encountered, returns `false` and populates `r->error`
//   with an appropriate message.
// - Otherwise, returns `true` and populates `key` with the next member-key and
//   `val` with its corresponding value.
bool mj_iter_object(mj_reader_t *r, mj_value_t *obj, mj_value_t *key, mj_value_t *val);

// Get a descriptive static-string for the given type.
const char* mj_desc_type(mj_type_t type);

// Get a descriptive static-string for the given type with a leading article.
const char* mj_desc_type_article(mj_type_t type);

// Return `true` if this type is a JSON value-type.
static inline bool mj_is_value(mj_type_t t) {
    return t >= MJ_ARRAY && t <= MJ_NULL;
}

// Extract the inner content of a string-typed value and return its length.
static inline size_t mj_strcut(
    mj_value_t *v,
    const char **p_beg,
    const char **p_end
) {
    *p_beg = v->beg + 1;
    *p_end = v->end - 1;
    return (size_t)(*p_end - *p_beg);
}

// Return the length of a string-typed value.
static inline size_t mj_strlen(mj_value_t *v) {
    const char *beg, *end;
    return mj_strcut(v, &beg, &end);
}

// Return `true` if the given value matches an expected string-literal.
static inline bool mj_strequ(mj_value_t *v, const char *s) {
    const char  *beg, *end;
    const size_t len = mj_strcut(v, &beg, &end);

    return strlen(s) == len && memcmp(s, beg, len) == 0;
}

#endif // DATAPROC_MINIJSON_H
