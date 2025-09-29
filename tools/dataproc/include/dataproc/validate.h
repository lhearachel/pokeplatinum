#ifndef DATAPROC_VALIDATE_H
#define DATAPROC_VALIDATE_H

#include <stdbool.h>
#include <stdlib.h>

#include "dataproc/minijson.h"
#include "dataproc/report.h"
#include "dataproc/stdutil.h"

#define report__parse_err(__r) \
    do {                       \
        report(                \
            (__r)->name,       \
            (__r)->data,       \
            (__r)->cur,        \
            (__r)->cur + 1,    \
            "%s",              \
            (__r)->error       \
        );                     \
    } while (0)

#define report__range_err(__r, __beg, __end, __fmt, ...) \
    do {                                                 \
        report(                                          \
            (__r)->name,                                 \
            (__r)->data,                                 \
            __beg,                                       \
            __end,                                       \
            __fmt,                                       \
            ##__VA_ARGS__                                \
        );                                               \
    } while (0)

#define report__value_err(__r, __v, __fmt, ...) \
    do {                                        \
        report(                                 \
            (__r)->name,                        \
            (__r)->data,                        \
            (__v)->beg,                         \
            (__v)->end,                         \
            __fmt,                              \
            ##__VA_ARGS__                       \
        );                                      \
    } while (0)

static inline bool is_boolean(
    const char *path,
    const mj_reader_t *r,
    const mj_value_t *v,
    bool *b
) {
    if (v->type != MJ_BOOL_TRUE && v->type != MJ_BOOL_FALSE) {
        report__value_err(
            r, v,
            "expected %s to be either true or false, but got %s",
            path, mj_desc_type_article(v->type)
        );

        return false;
    }

    *b = v->type == MJ_BOOL_TRUE;
    return true;
}

static inline bool is_int_base10(
    const char *path,
    const mj_reader_t *r,
    const mj_value_t *v,
    long *i
) {
    if (v->type != MJ_NUMBER) {
        report__value_err(
            r, v,
            "expected %s to be a base-10 integer, but got %s",
            path, mj_desc_type_article(v->type)
        );
        return false;
    }

    char *endptr;
    *i = strtol(v->beg, &endptr, 10);
    if (endptr != v->end) {
        const char *desc;
        switch (*endptr) {
        case 'b': case 'B': desc = "a base-2 integer";         break;
        case 'o': case 'O': desc = "a base-8 integer";         break;
        case 'x': case 'X': desc = "a base-16 integer";        break;
        case '.':           desc = "a floating-point number";  break;
        default:            desc = "an invalid number format"; break;
        }

        report__value_err(
            r, v,
            "expected %s to be a base-10 integer, but got %s",
            path, desc
        );
        return false;
    }

    return true;
}

static inline bool is_int_in_range(
    const char *path,
    mj_reader_t *r,
    mj_value_t *v,
    long out,
    long min, long max
) {
    if (out < min || out > max) {
        report__value_err(
            r, v,
            "expected %s to be in the range [%ld..%ld], but got %ld",
            path, min, max, out
        );
        return false;
    }

    return true;
}

static inline bool is_string(const char *path, mj_reader_t *r, mj_value_t *v) {
    if (v->type != MJ_STRING) {
        report__value_err(
            r, v,
            "expected %s to be a string, but got %s",
            path, mj_desc_type_article(v->type)
        );
        return false;
    }

    return true;
}

static inline bool is_array(const char *path, mj_reader_t *r, mj_value_t *v) {
    if (v->type != MJ_ARRAY) {
        report__value_err(
            r, v,
            "expected %s to be an array, but got %s",
            path, mj_desc_type_article(v->type)
        );
        return false;
    }

    return true;
}

static inline bool is_array_of_size(
    const char *path,
    mj_reader_t *r,
    mj_value_t  *arr,
    mj_value_t  *last_read,
    size_t      *i,
    size_t       expect
) {
    const char *err_beg = NULL;
    const char *err_end = NULL;

    if      (*i < expect) { err_beg = arr->beg; err_end = last_read->end + 1; }
    else if (mj_is_value(last_read->type)) {
        mj_value_t tmp = *last_read;
        do { (*i)++; } while (mj_iter_array(r, arr, &tmp));

        err_beg = last_read->beg;
        err_end = tmp.end - 1;
    }

    if (err_beg != NULL) {
        report__range_err(
            r, err_beg, err_end,
            "expected %s to be an array of length %zu, but found %zu %s",
            path, expect, *i, *i == 1 ? "entry" : "entries"
        );

        return false;
    }

    return true;
}

static inline bool is_object(const char *path, mj_reader_t *r, mj_value_t *v) {
    if (v->type != MJ_OBJECT) {
        report__value_err(
            r, v,
            "expected %s to be an object, but got %s",
            path, mj_desc_type_article(v->type)
        );
        return false;
    }

    return true;
}

static inline bool get_int_in_range(
    const char  *parent,
    const char  *key,
    mj_reader_t *r,
    mj_value_t  *v,
    long         expect_min,
    long         expect_max,
    long        *out_n
) {
    char *path   = join_path(parent, key);
    bool  result = is_int_base10(path, r, v, out_n)
        && is_int_in_range(path, r, v, *out_n, expect_min, expect_max);

    free(path);
    return result;
}

static inline bool get_string(
    const char   *parent,
    const char   *key,
    mj_reader_t  *r,
    mj_value_t   *v,
    const char  **out_beg,
    const char  **out_end,
    size_t       *out_len
) {
    char *path    = join_path(parent, key);
    bool  result  = is_string(path, r, v);
         *out_len = mj_strcut(v, out_beg, out_end);

    free(path);
    return result;
}

static inline bool get_boolean(
    const char  *parent,
    const char  *key,
    mj_reader_t *r,
    mj_value_t  *v,
    bool        *out_b
) {
    char *path   = join_path(parent, key);
    bool  result = is_boolean(path, r, v, out_b);

    free(path);
    return result;
}

typedef struct string_t {
    const char *data;
    size_t      size;
} string_t;

typedef struct lookup_entry_t {
    const long  value;
    const char *key_string;
} lookup_entry_t;

static inline int compare_enum_keys(const void *p_key, const void *p_memb) {
    const string_t       *key  = p_key;
    const lookup_entry_t *memb = p_memb;

    return strncmp(key->data, memb->key_string, key->size); // TODO: check for length match
}

#define find_enum(__T, __val)   \
    bsearch(                    \
        &(__val),               \
        lookup__ ## __T,        \
        lengthof__ ## __T,      \
        sizeof(entry__ ## __T), \
        compare_enum_keys       \
    )                           \

#define get_enum_by_key(__T, __parent, __i, __r, __v, __target)                     \
    ({                                                                              \
        const char *__beg, *__end;                                                  \
        size_t      __len;                                                          \
                                                                                    \
        char *__path  = join_path(__parent, __i);                                   \
        bool __result = get_string(NULL, __path, __r, __v, &__beg, &__end, &__len); \
                                                                                    \
        if (__result) {                                                             \
            string_t        __value = { .data = __beg, .size = __len };             \
            entry__ ## __T *__match = find_enum(__T, __value);                      \
                                                                                    \
            __result = __match != NULL;                                             \
            if (__result) {                                                         \
                *(__target) = (__typeof__(*(__target)))(__match)->value;            \
            } else {                                                                \
                report__value_err(                                                  \
                    __r, __v,                                                       \
                    "expected %s to be a member of enum %s, but got %.*s",          \
                    __path, #__T, (int)(__len), __beg                               \
                );                                                                  \
            }                                                                       \
        }                                                                           \
                                                                                    \
        free(__path);                                                               \
        __result; /* GNU extension: implicitly "returns" this value. */             \
    })

#endif // DATAPROC_VALIDATE_H
