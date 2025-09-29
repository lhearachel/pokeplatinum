#ifndef DATAPROC_STDUTIL_H
#define DATAPROC_STDUTIL_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static inline char* join_path(const char *parent, const char *child) {
    char *path = NULL;

    if (parent != NULL) {
        size_t size = snprintf(path, 0, "%s/%s", parent, child) + 1;

        path = calloc(size, sizeof(char));
        snprintf(path, size, "%s/%s", parent, child);
    } else {
        size_t size = strlen(child) + 1;

        path = calloc(size, sizeof(char));
        memcpy(path, child, size);
    }

    return path;
}

typedef struct file_t {
    char  *data;
    size_t size;
} file_t;

static inline file_t slurp(const char *filename) {
    FILE *infile = fopen(filename, "rb");
    if (infile == NULL) {
        perror("cannot open data file");
        exit(EXIT_FAILURE);
    }

    fseek(infile, 0, SEEK_END);
    long fsize = ftell(infile);
    fseek(infile, 0, SEEK_SET);

    if (fsize == 0) {
        puts("no data in input file; stop.");
        exit(EXIT_SUCCESS);
    }

    if (fsize < 0) {
        perror("invalid data-file size");
        fclose(infile);
        exit(EXIT_FAILURE);
    }

    char *fdata = malloc(fsize + 1);
    if (fdata == NULL) {
        perror("allocation failure for file content");
        fclose(infile);
        exit(EXIT_FAILURE);
    }

    fdata[fsize] = '\0';
    fread(fdata, 1, fsize, infile);
    fclose(infile);

    return (file_t) { .data = fdata, .size = fsize };
}

#endif // DATAPROC_STDUTIL_H
