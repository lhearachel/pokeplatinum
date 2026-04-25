#include <assert.h>
#include <errno.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "libenum.h"

char* slurp_file(const char *filename) {
    FILE *f = fopen(filename, "rb");
    if (f == NULL) {
        fprintf(stderr, "enumproc: error: cannot read file: %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }

    fseek(f, 0, SEEK_END);
    long fsize = ftell(f);
    fseek(f, 0, SEEK_SET);

    if (fsize < 0) {
        fprintf(stderr, "enumproc: error: cannot tell file: %s\n", strerror(errno));
        goto error;
    }

    char *buf = calloc(fsize + 1, sizeof(*buf));
    if (buf == NULL) {
        fprintf(stderr, "enumproc: error: cannot allocate for file-read: %s\n", strerror(errno));
        goto error;
    }

    if (fread(buf, sizeof(*buf), fsize, f) != (size_t)fsize) {
        fprintf(stderr, "enumproc: error: unexpected end-of-file during read: %s\n", strerror(errno));
        goto error;
    }

    fclose(f);
    return buf;

error:
    fclose(f);
    exit(EXIT_FAILURE);
}

char* slurp_stdin(void) {
    char   *line   = NULL;
    size_t  size   = 0;
    ssize_t nread  = 0;

    size_t cap = 4096;
    char  *buf = calloc(cap, sizeof(*buf));
    size_t len = 0;

    while ((nread = getline(&line, &size, stdin)) != -1) {
        if (len + nread >= cap) {
            buf  = realloc(buf, cap * 2 * sizeof(*buf));
            cap *= 2;
        }

        memcpy(buf + len, line, nread);
        len     += nread;
        buf[len] = 0;
    }

    free(line);
    return buf;
}

bool isws(char c) {
    return c == ' ' || c == '\t' || c == '\r' || c == '\n';
}

int main(int argc, char *argv[]) {
    char *content = argc == 1 ? slurp_stdin() : slurp_file(argv[1]);
    char *endptr  = NULL;
    char *p       = content;

    while (*p) {
        while (isws(*p)) { fputc(*p, stdout); p++; }

        if (strncmp(p, "enum", 4) == 0) {
            enum_t parsed = enum_map_one(p, &endptr, LIBENUM_F_EVAL, NULL, NULL);
            if (*endptr != ';') goto put_line; // probably not a C enum

            for (size_t i = 0; i < parsed.size; i++) {
                member_t *member = &parsed.members[i];
                printf("#define %s %ld\n", member->symbol, member->value);
            }

            free(parsed.members);
            p = endptr;
            p = *p == ';'  ? p + 1 : p;
            p = *p == '\n' ? p + 1 : p;
        }
        else {
        put_line:
            if (!*p) break;
            do { fputc(*p, stdout); p++; } while (*p && *p != '\n');
        }
    }

    free(content);
    exit(EXIT_SUCCESS);
}
