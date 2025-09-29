#include "dataproc/report.h"

#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "dataproc/ansiesc.h"

#define with_varargs(__param, __do) \
    do {                            \
        va_list args;               \
        va_start(args, fmt);        \
        __do;                       \
        va_end(args);               \
    } while (0)

//                       ┌─────── name of the file-stream
//                       │  ┌──── in-file line number where the error starts
//                       │  │  ┌─ in-line column number where the error starts
//                      ┏┪ ┏┪ ┏┪
#define FMT_TITLE "%s" "%s:%d:%d: " "%s" "error: " "%s"

//                       ┌──────────────────────────────── line number, left-padded with spaces to 4 digits
//                       │      ┌───────────────────────── section of line-preview before the error
//                       │      │           ┌───────────── erroneous section of line-preview
//                       │      │           │           ┌─ section of line-preview after the error
//                      ┏┷┓   ┏━┷┓        ┏━┷┓        ┏━┷┓
#define FMT_HIGHLIGHT " %4d | %.*s" "%s" "%.*s" "%s" "%.*s\n"

static inline void report_title(
    bool style,
    const char *filename,
    int line,
    int col,
    const char *fmt,
    va_list args
) {
    const char *STYLE_ON  = style ? STYLE_BOLD    : "";
    const char *STYLE_OFF = style ? STYLE_NONE    : "";
    const char *COLOR_ON  = style ? COLOR_RED     : "";
    const char *COLOR_OFF = style ? COLOR_DEFAULT : "";

    fprintf(
        stderr,
        FMT_TITLE,
        STYLE_ON, filename, line, col,
        COLOR_ON, /* error: */
        COLOR_OFF
    );
    vfprintf(stderr, fmt, args);
    fprintf( stderr, "%s\n", STYLE_OFF);
}

static inline void report_highlight(
    bool style,
    int line,
    const char *line_beg,
    const char *line_end,
    const char *beg,
    const char *end
) {
    int len_before = (int)(beg - line_beg);
    int len_value  = (int)(end - beg);
    int len_after  = (int)(line_end - end);

    const char *STYLE_ON  = style ? STYLE_BOLD COLOR_RED : "";
    const char *STYLE_OFF = style ? STYLE_NONE           : "";

    fprintf(
        stderr,
        FMT_HIGHLIGHT,
        line,
        len_before, line_beg,
        STYLE_ON,  len_value, beg,
        STYLE_OFF, len_after, end
    );
}

static inline void report_squiggle(bool style, int indent, int length) {
    int left  = length / 2;
    int right = left - ((length + 1) & 1);

    const char *STYLE_ON  = style ? STYLE_BOLD COLOR_RED : "";
    const char *STYLE_OFF = style ? STYLE_NONE           : "";

    fprintf(stderr, "      | %*s", indent, "");

    fputs(STYLE_ON, stderr);
    for (int i = 0; i < left; i++)  fputc('~', stderr);
    /*                           */ fputc('^', stderr);
    for (int i = 0; i < right; i++) fputc('~', stderr);

    fprintf(stderr, "%s\n", STYLE_OFF);
}

typedef struct errpos_t {
    const char *line_beg;
    const char *line_end;

    int line;
    int col;
} errpos_t;

static inline errpos_t locate(const char *beg, const char *cur) {
    errpos_t pos = {
        .line_beg = beg,
        .line_end = NULL,
        .line     = 1,
        .col      = 1,
    };

    const char *p;
    for (p = beg; p != cur; p++) {
        if       (*p == '\n')         { pos.line_beg = p + 1; pos.line++; pos.col = 0; }
        else if ((*p & 0xFF) >= 0xF0) { pos.col += 3; }
        else if ((*p & 0xFF) >= 0xE0) { pos.col += 2; }
        else if ((*p & 0xFF) >= 0xC0) { pos.col += 1; }

        pos.col++;
    }

    for (p = cur; *p && *p != '\n'; p++);
    pos.line_end = p;
    return pos;
}

void report(
    const char *filename,
    const char *data,
    const char *hl_beg,
    const char *hl_end,
    const char *fmt,
    ...
) {
    const char *no_color = getenv("NO_COLOR");
    const bool  style    = no_color == NULL || no_color[0] != '\0';

    errpos_t pos = locate(data, hl_beg);
    with_varargs(fmt, {
        report_title(style, filename, pos.line, pos.col, fmt, args);
        report_highlight(style, pos.line, pos.line_beg, pos.line_end, hl_beg, hl_end);
        report_squiggle(style, (int)(hl_beg - pos.line_beg), (int)(hl_end - hl_beg));
    });
}
