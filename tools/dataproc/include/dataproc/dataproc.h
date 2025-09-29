#ifndef DATAPROC_DATAPROC_H
#define DATAPROC_DATAPROC_H

#include "dataproc/minijson.h"

// A consumer for a key-value pair in an object-typed document.
//
// Implementations must return `true` if all input-constraints are satisfied;
// otherwise, they shall return `false`. The timing of this return is
// purposefully left undefined: a consumer may return `false` on the first
// constraint violation, or it may report as many as violations as possible
// before returning.
typedef bool (*dp_consumer)(
    mj_reader_t *reader,
    const char  *key,
    mj_value_t  *value,
    void        *user
);

// A mapping of a string-literal key to a function which consumes its associated
// value. The value of `key` shall be passed directly to the parameter of the
// same name in the consumer.
typedef struct dp_field_t {
    const char  *key;
    dp_consumer  consume;
} dp_field_t;

// Parse and process an object-typed JSON document.
//
// This routine iteratively processes fields according to the defined key-value
// associations. If a top-level key is encountered that is not recognized, then
// it is ignored.
//
// The value of `fields` must be an array of `dp_field_t`, with the final entry
// of the table initialized to all-zeroes. The value of `user` will be passed to
// consumers for processing by an implementation. For efficiency, this routine
// maintains an internal cursor for its position in `fields`, so it is
// advantageous to define entries in `fields` in the same order as they are
// expected to appear in an input document.
bool process_json(mj_reader_t *json, const dp_field_t *fields, void *user);

#endif // DATAPROC_DATAPROC_H
