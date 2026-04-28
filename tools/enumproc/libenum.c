#include "libenum.h"

#include <assert.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

static bool isws(char c) {
    return c == ' ' || c == '\t' || c == '\r' || c == '\n';
}

static char* skip(const char *str) {
    assert(str);

    for (;;) {
        while (isws(*str)) str++;

        if (str[0] == '/' && str[1] == '/') {
            str += 2;
            while (str[0] && str[0] != '\r' && str[0] != '\n') str++;
            continue;
        }

        if (str[0] == '/' && str[1] == '*') {
            const char *start = str;

            str += 2;
            while (str[0] && str[1] && !(str[0] == '*' && str[1] == '/')) str++;
            if    (!str[0] || !str[1]) { str = start; break; } // unterminated
            str += 2;
            continue;
        }

        break;
    }

    return (char *)str;
}

static inline bool is_alpha(char c) {
    return (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z');
}

static inline bool is_digit(char c) {
    return (c >= '0' && c <= '9');
}

static inline bool is_ident(char c) {
    return is_alpha(c) || is_digit(c) || c == '_';
}

/* ========================== EXPRESSION EVALUATOR ========================== */

typedef struct opstack opstack_t;
struct opstack {
    long   nums[16];
    size_t n_nums;

    int    opts[16];
    size_t n_opts;

    int    unas[16];
    size_t n_unas;
};

#define precedes(o1, o2)  opt_data[o2].prec < opt_data[o1].prec
#define congruent(o1, o2) opt_data[o2].prec == opt_data[o1].prec
#define is_left(opt)      opt_data[opt].left

#define top(stk, var)       stk.var[stk.n_##var - 1]
#define pop(stk, var)       stk.var[--stk.n_##var]
#define push(stk, var, val) stk.var[stk.n_##var++] = val

#define evalbinary(o)                                        \
    do {                                                     \
        assert(stk.n_nums >= 2);                             \
        stk.n_nums -= 2;                                     \
                                                             \
        long *p_args = &stk.nums[stk.n_nums];                \
        long  result = 0;                                    \
        switch (o) {                                         \
        case OP_MUL: result = p_args[0] *  p_args[1]; break; \
        case OP_DIV: result = p_args[0] /  p_args[1]; break; \
        case OP_MOD: result = p_args[0] %  p_args[1]; break; \
        case OP_ADD: result = p_args[0] +  p_args[1]; break; \
        case OP_SUB: result = p_args[0] -  p_args[1]; break; \
        case OP_LSH: result = p_args[0] << p_args[1]; break; \
        case OP_RSH: result = p_args[0] >> p_args[1]; break; \
        case OP_LST: result = p_args[0] <  p_args[1]; break; \
        case OP_LTE: result = p_args[0] <= p_args[1]; break; \
        case OP_GRT: result = p_args[0] >  p_args[1]; break; \
        case OP_GTE: result = p_args[0] >= p_args[1]; break; \
        case OP_DEQ: result = p_args[0] == p_args[1]; break; \
        case OP_NEQ: result = p_args[0] != p_args[1]; break; \
        case OP_BAN: result = p_args[0] &  p_args[1]; break; \
        case OP_XOR: result = p_args[0] ^  p_args[1]; break; \
        case OP_BOR: result = p_args[0] |  p_args[1]; break; \
        case OP_AND: result = p_args[0] && p_args[1]; break; \
        case OP_ORR: result = p_args[0] || p_args[1]; break; \
                                                             \
        default: assert(0 && "illegal op-type");             \
        }                                                    \
                                                             \
        push(stk, nums, result);                             \
    } while (0)

#define evalunary(o)                                         \
    do {                                                     \
        assert(stk.n_nums >= 1);                             \
                                                             \
        long res = pop(stk, nums);                           \
        switch (o) {                                         \
        case OP_POS: res = res < 0 ? -res : res; break;      \
        case OP_NEG: res = res > 0 ? -res : res; break;      \
        case OP_NOT: res = !res;                 break;      \
        case OP_BNT: res = ~res;                 break;      \
                                                             \
        default: assert(0 && "illegal op-type");             \
        }                                                    \
                                                             \
        push(stk, nums, res);                                \
    } while (0)

// ref: https://en.wikipedia.org/wiki/Shunting_yard_algorithm
long eval(char *expr, char **endptr, char *terminators, enum_t *scope) {
    enum {
        OP_LPA,
        OP_RPA,
        OP_POS,
        OP_NEG,
        OP_NOT,
        OP_BNT,
        OP_MUL,
        OP_DIV,
        OP_MOD,
        OP_ADD,
        OP_SUB,
        OP_LSH,
        OP_RSH,
        OP_LST,
        OP_LTE,
        OP_GRT,
        OP_GTE,
        OP_DEQ,
        OP_NEQ,
        OP_BAN,
        OP_XOR,
        OP_BOR,
        OP_AND,
        OP_ORR,
    };

    // ref: https://en.cppreference.com/w/c/language/operator_precedence.html
    static const struct { int prec; bool left; } opt_data[] = {
        [OP_LPA] = { .prec = 1,  .left = true  },
        [OP_RPA] = { .prec = 1,  .left = true  },
        [OP_POS] = { .prec = 2,  .left = false },
        [OP_NEG] = { .prec = 2,  .left = false },
        [OP_NOT] = { .prec = 2,  .left = false },
        [OP_BNT] = { .prec = 2,  .left = false },
        [OP_MUL] = { .prec = 3,  .left = true  },
        [OP_DIV] = { .prec = 3,  .left = true  },
        [OP_MOD] = { .prec = 3,  .left = true  },
        [OP_ADD] = { .prec = 4,  .left = true  },
        [OP_SUB] = { .prec = 4,  .left = true  },
        [OP_LSH] = { .prec = 5,  .left = true  },
        [OP_RSH] = { .prec = 5,  .left = true  },
        [OP_LST] = { .prec = 6,  .left = true  },
        [OP_LTE] = { .prec = 6,  .left = true  },
        [OP_GRT] = { .prec = 6,  .left = true  },
        [OP_GTE] = { .prec = 6,  .left = true  },
        [OP_DEQ] = { .prec = 7,  .left = true  },
        [OP_NEQ] = { .prec = 7,  .left = true  },
        [OP_BAN] = { .prec = 8,  .left = true  },
        [OP_XOR] = { .prec = 9,  .left = true  },
        [OP_BOR] = { .prec = 10, .left = true  },
        [OP_AND] = { .prec = 11, .left = true  },
        [OP_ORR] = { .prec = 12, .left = true  },
    };

    char *p = expr, *e = NULL;
    bool  u = true,  n = true;
    long  m = 1;
    int   o1, o2;

    opstack_t stk = { 0 };
    if (!terminators) terminators = "";

    while (*p) {
        bool done = false;
        for (char *c = terminators; *c && !done; c++) {
            if (*p == *c) done = true;
        }
        if (done) break;

        if (is_alpha(*p) || *p == '_') {
            if (!n) goto early_exit;
            char *s = p;
            do { p++; } while (is_ident(*p));

            char c = *p;
            *p     = '\0';

            member_t *match = NULL;
            for (size_t i = 0; i < scope->size && !match; i++) {
                if (strcmp(scope->members[i].symbol, s) == 0) {
                    match = &scope->members[i];
                }
            }
            *p = c;

            if (!match) goto early_exit; // undefined symbol

            push(stk, nums, match->value);
            while (stk.n_unas > 0) {
                o1 = pop(stk, unas);
                evalunary(o1);
            }

            p = skip(p);
            n = false;
            u = false;
            continue;
        }

        switch (*p) {
        case '0': case '1': case '2': case '3': case '4':
        case '5': case '6': case '7': case '8': case '9':
            if (!n) goto early_exit;
            push(stk, nums, strtol(p, &e, 0));
            while (stk.n_unas > 0) {
                o1 = pop(stk, unas);
                evalunary(o1);
            }

            p = e;
            n = false;
            u = false;
            break;

#define prepunary(opt)       do { push(stk, unas, opt); p++; n = true; } while (0)
#define procbinary(opt, len) do { o1 = opt; p += len; goto handle_binary; } while (0)

        case '+':
            if (u) prepunary(OP_POS);
            else   procbinary(OP_ADD, 1);
            break;

        case '-':
            if (u) prepunary(OP_NEG);
            else   procbinary(OP_SUB, 1);
            break;

        case '!':
            if (u) prepunary(OP_NOT);
            else {
                assert(p[1] == '=' && "invalid operator (expected !=)");
                procbinary(OP_NEQ, 2);
            }
            break;

        case '=':
            assert(p[1] == '=' && "invalid operator (expected ==)");
            procbinary(OP_DEQ, 2);
            break;

        case '*': procbinary(OP_MUL, 1);
        case '/': procbinary(OP_DIV, 1);
        case '%': procbinary(OP_MOD, 1);
        case '^': procbinary(OP_XOR, 1);
        case '~': prepunary(OP_BNT); break;

        case '<':
            if (p[1] == '<') procbinary(OP_LSH, 2);
            if (p[1] == '=') procbinary(OP_LTE, 2);
            procbinary(OP_LST, 1);

        case '>':
            if (p[1] == '>') procbinary(OP_RSH, 2);
            if (p[1] == '=') procbinary(OP_GTE, 2);
            procbinary(OP_GRT, 1);

        case '&':
            if (p[1] == '&') procbinary(OP_AND, 2);
            procbinary(OP_BAN, 1);

        case '|':
            if (p[1] == '|') procbinary(OP_ORR, 2);
            procbinary(OP_BOR, 1);

        handle_binary:
            o2 = top(stk, opts);
            while (stk.n_opts > 0 && o2 != OP_LPA
                && (precedes(o1, o2) || (congruent(o1, o2) && is_left(o1)))) {
                o2 = pop(stk, opts);
                evalbinary(o2);
            }

            u = true;
            n = true;
            push(stk, opts, o1);
            break;

#undef procbinary
#undef prepunary

        case '(': push(stk, opts, OP_LPA); p++; u = true; n = true; break;
        case ')':
            o2 = -1;
            while (stk.n_opts > 0 && (o2 = pop(stk, opts)) != OP_LPA) {
                evalbinary(o2);
            }

            if (o2 != OP_LPA) goto early_exit;
            long v = pop(stk, nums) * m;
            push(stk, nums, v);

            while (stk.n_unas > 0) {
                o1 = pop(stk, unas);
                evalunary(o1);
            }

            n = false;
            u = false;
            p++;
            break;

        default: goto early_exit;
        }

        p = skip(p);
    }

early_exit:
    while (stk.n_opts > 0) {
        o2 = pop(stk, opts);
        assert(o2 != OP_LPA && "unclosed parentheses");
        evalbinary(o2);
    }

    while (stk.n_unas > 0) {
        o1 = pop(stk, unas);
        evalunary(o1);
    }

    if (endptr) *endptr = p;
    return stk.nums[0];
}

/* ========================= MAPPING IMPLEMENTATION ========================= */

static int membcmp(const void *lhs, const void *rhs) {
    const member_t *l = lhs;
    const member_t *r = rhs;

    if (l->symbol == NULL && r->symbol == NULL) return 0;
    if (l->symbol == NULL) return  1;
    if (r->symbol == NULL) return -1;

    return strcmp(l->symbol, r->symbol);
}

static int enumcmp(const void *lhs, const void *rhs) {
    const enum_t *l = lhs;
    const enum_t *r = rhs;

    if (l->ident == NULL && r->ident == NULL) return 0;
    if (l->ident == NULL) return  1;
    if (r->ident == NULL) return -1;

    return strcmp(l->ident, r->ident);
}

#define setup_member()                                                   \
    do {                                                                 \
        if (result.size >= cap) {                                        \
            size_t    new = 2 * cap;                                     \
            member_t *tmp = realloc(result.members, new * sizeof(*tmp)); \
            if (tmp == NULL) return_error(LIBENUM_R_ALLOC);              \
            cap            = new;                                        \
            result.members = tmp;                                        \
        }                                                                \
                                                                         \
        if (scope && scope->size >= s_cap) {                             \
            size_t    new = 2 * s_cap;                                   \
            member_t *tmp = realloc(scope->members, new * sizeof(*tmp)); \
            if (tmp == NULL) return_error(LIBENUM_R_ALLOC);              \
            s_cap          = new;                                        \
            scope->members = tmp;                                        \
        }                                                                \
                                                                         \
        if (!is_alpha(*p) && *p != '_') return_error(LIBENUM_R_NOMEMB);  \
        curr         = &result.members[result.size];                     \
        curr->symbol = p;                                                \
        do { p++; } while (is_ident(*p));                                \
                                                                         \
        e = p;                                                           \
        p = skip(p);                                                     \
    } while (0)

#define close_member()                                                     \
    do {                                                                   \
        *e = '\0';                                                         \
        if (flags & LIBENUM_F_UNIQ) {                                      \
            enum_t *check = scope ? scope : &result;                       \
            for (size_t i = 0; i < check->size; i++) {                     \
                if (strcmp(check->members[i].symbol, curr->symbol) == 0) { \
                    p = curr->symbol;                                      \
                    return_error(LIBENUM_R_MEMB_EXISTS);                   \
                }                                                          \
            }                                                              \
        }                                                                  \
        if (scope) scope->members[scope->size++] = *curr;                  \
        result.size++;                                                     \
        p = skip(p);                                                       \
    } while (0)

#define nextpow2_64(v) \
    do {               \
        v--;           \
        v |= v >> 1;   \
        v |= v >> 2;   \
        v |= v >> 4;   \
        v |= v >> 8;   \
        v |= v >> 16;  \
        v |= v >> 32;  \
        v++;           \
    } while (0)

static void merge(enum_t *dst, const enum_t *src, size_t *p_cap) {
    while (dst->size + src->size >= *p_cap) {
        *p_cap      *= 2;
        dst->members = realloc(dst->members, *p_cap * sizeof(*dst->members));
    }

    for (size_t i = 0; i < src->size; i++) {
        dst->members[dst->size++] = src->members[i];
    }
}

#define return_error(code) do { result.errc = code; goto cleanup_err; } while (0)

enum_t enum_map_one(char *str, char **endptr, unsigned flags, const char *filter, enum_t *scope) {
    assert(str);

    enum { STATE_DECL, STATE_READ, STATE_DONE } state = STATE_DECL;
    enum_t result = { .ident = LIBENUM_GLOBAL_ID, .members = NULL, .size = 0 };

    size_t s_cap = 128; // 128 becomes 256
    if (scope && scope->size) s_cap = scope->size;
    nextpow2_64(s_cap);

    enum_t *p_scope  = scope ? scope : &result;
    long    next     = 0;
    size_t  cap      = 256;
    char   *p        = skip(str);
    size_t  s_filter = filter ? strlen(filter) : 0;
    while (*p && state < STATE_DONE) {
        if (state == STATE_DECL) {
            if (strncmp(p, "enum", sizeof("enum") - 1) != 0
                || ((p += 4, !isws(*p) && *p != '{')))
            {
                return_error(LIBENUM_R_DECL);
            }

            p = skip(p);
            if (is_alpha(*p) || *p == '_') { // enum has an identifier
                result.ident = p;
                do { p++; } while (is_ident(*p));
            }

            char *e = p;
            p       = skip(p);

            if (*p != '{') return_error(LIBENUM_R_DECL);
            *e    = '\0';
            p     = skip(p + 1);
            state = STATE_READ;

            result.members = calloc(cap, sizeof(*result.members));
            continue;
        }

        if (*p == '}' || *p == ';') {
            if (result.size == 0) return_error(LIBENUM_R_EMPTY);

            state = STATE_DONE;
            p     = skip(p + 1);
            break;
        }

        member_t *curr;
        char     *e;

        setup_member();

        switch (*p) {
        case '}': state       = STATE_DONE; // fall-through
        case ',': curr->value = next++;     break;

        case '=': {
            if (flags & LIBENUM_F_EVAL) {
                curr->value = eval(skip(p + 1), &p, ",}", p_scope);
                next        = curr->value + 1;

                if (*p != '}' && *p != ',') return_error(LIBENUM_R_TOKEN);
            } else {
                p          = skip(p + 1);
                curr->expr = p;
                while (*p && *p != ',' && *p != '}') p++;
                *p = '\0';
                p++;
            }
            break;
        }

        default: return_error(LIBENUM_R_TOKEN);
        }

        p++;

        // This check occurs here because we need `next` to be updated.
        if (!filter || strncmp(curr->symbol, filter, s_filter) == 0) {
            close_member();
        }
    }

    if (flags & LIBENUM_F_SORT) {
        qsort(result.members, result.size, sizeof(*result.members), membcmp);
    }

    if (endptr) *endptr = p;
    result.flags        = flags;
    return result;

cleanup_err:
    if (endptr) *endptr = p;
    free(result.members);
    result.members = NULL;
    result.size    = 0;
    return result;
}

enum_t enum_map_cpp(char *str, char **endptr, unsigned flags, const char *filter, enum_t *scope) {
    assert(str);

    size_t cap    = 256;
    char  *p      = skip(str);
    enum_t result = {
        .ident   = LIBENUM_GLOBAL_ID,
        .members = calloc(cap, sizeof(*result.members)),
        .size    = 0,
    };

    enum_t *p_scope  = scope ? scope : &result;
    size_t  s_cap    = 128; // 128 becomes 256
    size_t  s_filter = filter ? strlen(filter) : 0;
    if (scope && scope->size) s_cap = scope->size;
    nextpow2_64(s_cap);

    while (*p && strncmp(p, "#define", sizeof("#define") - 1) == 0) {
        char     *e;
        member_t *curr;
        p = skip(p + 7);

        setup_member();

        // If it's a bare #define with no substitution, then skip it.
        char *l = e;
        while (*l && *l != '\n') l++;
        if (l < p) continue;

        if (flags & LIBENUM_F_EVAL) {
            curr->value = eval(p, &p, "\n", p_scope);
        } else {
            curr->expr = p;
            while (*p && *p != '\n') p++;
            *p = '\0';
            p++;
        }

        if (!filter || strncmp(curr->symbol, filter, s_filter) == 0) {
            close_member();
        }
    }

    if (flags & LIBENUM_F_SORT) {
        qsort(result.members, result.size, sizeof(*result.members), membcmp);
    }

    if (endptr) *endptr = p;
    result.flags        = flags;
    return result;

cleanup_err:
    if (endptr) *endptr = p;
    free(result.members);
    result.members = NULL;
    result.size    = 0;
    return result;
}

#undef return_error
#define return_error(code) do { if (o_size) *o_size = code; goto cleanup_err; } while (0)

enum_t* enum_map_all(char *str, char **endptr, unsigned flags, const char *filter, size_t *o_size) {
    assert(str);

    size_t  r_cap  = 256;
    size_t  s_cap  = 256;
    size_t  size   = 0;
    char   *p      = skip(str);
    enum_t *result = calloc(r_cap, sizeof(*result));
    enum_t  scope  = {
        .ident   = LIBENUM_GLOBAL_ID,
        .members = calloc(s_cap, sizeof(*scope.members)),
        .size    = 0,
    };

    while (*p) {
        enum_t one = { 0 };
        if (strncmp(p, "enum", 4) == 0) {
            one = enum_map_one(p, &p, flags, filter, &scope);
        }
        else if (strncmp(p, "#define", 7) == 0 && isws(p[7])) {
            one = enum_map_cpp(p, &p, flags, filter, &scope);
        }
        else {
            do { p++; } while (*p && *p != '\n');
            p = skip(p);
            continue;
        }

        if (one.errc != LIBENUM_R_OK) return_error(one.errc);
        if (one.size == 0) continue;
        if (one.ident[0] != '\0' && (flags & LIBENUM_F_UNIQ)) {
            for (size_t i = 0; i < size; i++) {
                if (strcmp(result[i].ident, one.ident) == 0) {
                    return_error(LIBENUM_R_ENUM_EXISTS);
                }
            }
        }

        while (scope.size + one.size >= s_cap) {
            member_t *tmp = realloc(scope.members, 2 * s_cap * sizeof(*tmp));
            if (tmp == NULL) return_error(LIBENUM_R_ALLOC);
            s_cap        *= 2;
            scope.members = tmp;
        }
        for (size_t i = 0; i < one.size; i++) {
            scope.members[scope.size] = one.members[i];
            scope.size++;
        }

        if (size >= r_cap) {
            enum_t *tmp = realloc(result, 2 * r_cap * sizeof(*tmp));
            if (tmp == NULL) return_error(LIBENUM_R_ALLOC);
            r_cap *= 2;
            result = tmp;
        }

        result[size++] = one;
    }

    enum_t *final  = result;
    size_t  f_size = size;
    if (size > 0) {
        size_t cap = 256;
        final      = calloc(r_cap, sizeof(*final));
        f_size     = 1;
        final[0]   = (enum_t){
            .ident   = LIBENUM_GLOBAL_ID,
            .members = calloc(cap, sizeof(*final[0].members)),
            .size    = 0,
            .flags   = flags,
        };
        for (size_t i = 0; i < size; i++) {
            if (result[i].ident[0] == '\0') {
                merge(&final[0], &result[i], &cap);
                free(result[i].members);
            }
            else final[f_size++] = result[i];
        }

        free(result);

        if (flags & LIBENUM_F_SORT) {
            qsort(final[0].members, final[0].size, sizeof(member_t), membcmp);
        }
    }

    if (endptr) *endptr = p;
    if (o_size) *o_size = f_size;
    if (flags & LIBENUM_F_SORT) {
        qsort(&final[1], f_size - 1, sizeof(enum_t), enumcmp);
    }

    free(scope.members);
    return final;

cleanup_err:
    if (endptr) *endptr = p;
    if (o_size) *o_size = 0;

    free(scope.members);
    for (size_t i = 0; i < size; i++) free(result[i].members);
    free(result);
    return NULL;
}

const char* enum_errs(unsigned errc) {
    switch (errc) {
    case LIBENUM_R_OK:          return "(ok)";
    case LIBENUM_R_ALLOC:       return "allocation failure";
    case LIBENUM_R_DECL:        return "invalid C enum declaration";
    case LIBENUM_R_EMPTY:       return "resulting enum has no members";
    case LIBENUM_R_TOKEN:       return "unexpected token";
    case LIBENUM_R_NOMEMB:      return "expected a member-definition";
    case LIBENUM_R_MEMB_EXISTS: return "member identifier re-defined";
    case LIBENUM_R_ENUM_EXISTS: return "enum identifier re-defined";

    default: return "unknown result-code";
    }
}
