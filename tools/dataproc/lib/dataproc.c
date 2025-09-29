#include "dataproc/dataproc.h"

#include <assert.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

#include "dataproc/minijson.h"
#include "dataproc/validate.h"

static bool find_field(
    const dp_field_t *fields,
    mj_value_t *key,
    const dp_field_t **cursor
) {
    const dp_field_t *f = *cursor;

    const char  *k_beg, *k_end;
    const size_t k_len = mj_strcut(key, &k_beg, &k_end);

    while (f && f->key != NULL) {
        if (strlen(f->key) == k_len && memcmp(f->key, k_beg, k_len) == 0) break;
        f++;
    }

    // If the forward-search yields no result, check everything behind us.
    if (f->key == NULL) {
        f = &fields[0];
        while (f && f != *cursor) {
            if (strlen(f->key) == k_len && memcmp(f->key, k_beg, k_len) == 0) break;
            f++;
        }

        // If we wound up back where we started, then the key is not defined.
        if (f == *cursor) return false;
    }

    *cursor = f;
    return true;
}

bool process_json(mj_reader_t *json, const dp_field_t *fields, void *user) {
    assert(json);
    assert(fields);

    mj_value_t root = mj_read(json);
    if (root.type == MJ_ERROR) {
        report__parse_err(json);
        return false;
    }

    if (root.type != MJ_OBJECT) {
        report__value_err(
            json, &root,
            "expected document to contain an object, but found %s",
            mj_desc_type_article(root.type)
        );
        return false;
    }

    mj_value_t k  = { 0 };
    mj_value_t v  = { 0 };
    bool       ok = true;

    const dp_field_t *f = &fields[0];
    while (ok && mj_iter_object(json, &root, &k, &v)) {
        if (!find_field(fields, &k, &f)) continue;

        if (f->consume != NULL) {
            size_t size = strlen(f->key);
            char  *path = calloc(size + 2, sizeof(char));
            memcpy(path + 1, f->key, size);
            path[0] = '/';

            ok = f->consume(json, path, &v, user);
            free(path);
        }

        f++;
    }

    if (json->error) {
        report__parse_err(json);
        return false;
    }

    return ok;
}
