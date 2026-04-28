#ifndef LIBENUM_H
#define LIBENUM_H

#include <stddef.h>

#define LIBENUM_GLOBAL_ID ""

#define LIBENUM_F_EVAL (1 << 0) // Evaluate integer constant expressions
#define LIBENUM_F_SORT (1 << 1) // Sort the final outputs by symbol-names
#define LIBENUM_F_UNIQ (1 << 2) // Ensure that all identifiers are unique

enum {
    LIBENUM_R_OK = 0,
    LIBENUM_R_ALLOC,
    LIBENUM_R_DECL,
    LIBENUM_R_EMPTY,
    LIBENUM_R_TOKEN,
    LIBENUM_R_NOMEMB,
    LIBENUM_R_MEMB_EXISTS,
    LIBENUM_R_ENUM_EXISTS,
};

typedef struct member_t member_t;
struct member_t {
    union {
        long  value;
        char *expr;
    };
    char *symbol;
};

typedef struct enum_t enum_t;
struct enum_t {
    char     *ident;
    char     *pool;
    member_t *members;
    size_t    size;
    unsigned  flags;
    unsigned  errc;
};

// Map a single C-style `enum` from the start of a modifiable input string.
//
// If `endptr` is not `NULL`, then `*endptr` will point to the first unprocessed
// character in `str` on return.
//
// If `scope` is not `NULL`, then its member-values will be available for
// substitution when evaluating integer constant expressions.
//
// When `flags` is given with `LIBENUM_F_EVAL`, integer constant expressions
// will be evaluated, and each member of the output mapping shall use `value`.
// Otherwise, the expression will be stored as-is in `expr`.
//
// When `flags` is given with `LIBENUM_F_SORT`, the resulting members will be
// sorted in-order according to their symbol-names. Note that without the use of
// of `LIBENUM_F_EVAL`, this will destroy the successor-context of members; the
// value of any given member will thus be indeterminate.
//
// If `filter` is not `NULL`, then its content will be used to select only those
// `enum` members which contain `filter` as a prefix for their symbol-name.
//
// If an error occurs during parsing, then the returned struct will specify its
// `members` as `NULL` rather than an empty list.
enum_t enum_map_one(char *str, char **endptr, unsigned flags, const char *filter, enum_t *scope);

// Map C preprocessor `#define`s from the start of a modifiable input string.
//
// If `endptr` is not `NULL`, then `*endptr` will point to the first unprocessed
// character in `str` on return.
//
// If `scope` is not `NULL`, then its member-values will be available for
// substitution when evaluating integer constant expressions.
//
// When `flags` is given with `LIBENUM_F_EVAL`, integer constant expressions
// will be evaluated, and each member of the output mapping shall use `value`.
// Otherwise, the expression will be stored as-is in `expr`.
//
// When `flags` is given with `LIBENUM_F_SORT`, the resulting members will be
// sorted in-order according to their symbol-names. Note that without the use of
// of `LIBENUM_F_EVAL`, this will destroy the successor-context of members; the
// value of any given member will thus be indeterminate.
//
// If `filter` is not `NULL`, then its content will be used to select only those
// `#define`s for C preprocessor symbols which contain `filter` as a prefix.
//
// If an error occurs during parsing, then the returned struct will specify its
// `members` as `NULL` rather than an empty list.
enum_t enum_map_cpp(char *str, char **endptr, unsigned flags, const char *filter, enum_t *scope);

// Map all C preprocessor `#define`s and C-style `enum`s from the start of an
// input string. The returned address will be a list of `enum_t` entries; all
// preprocessor entries and `enum`s without identifiers will be merged into the
// same element of this list and marked with `LIBENUM_GLOBAL_ID`, which will
// always be the first element of the list.
//
// The length of the output list shall be returned in `*o_size`. When an error
// occurs while parsing the string, the returned list will be `NULL`.
//
// When `flags` is given with `LIBENUM_F_SORT`, all other output mappings will
// be sorted lexicographically according to their identifiers. The input `flags`
// are otherwise passed to the individual parsing functions as-is.
//
// When this routine returns `NULL`, if `o_size != NULL`, `*o_size` will be set
// to a value describing the error.
enum_t* enum_map_all(char *str, char **endptr, unsigned flags, const char *filter, size_t *o_size);

// Return a human-readable string describing a result-code. This string is
// statically allocated and does not need to be `free`'d.
const char* enum_errs(unsigned errc);

#endif // LIBENUM_H
