#ifndef DATAPROC_REPORT_H
#define DATAPROC_REPORT_H

// Emit a GCC-style error report.
//
// - This routine will automatically emit a new-line after the report's title
//   line. That is, a client should NOT terminate `fmt` with `\n`.
// - Line and column numbers within the file-stream are computed from the start
//   of the error (the value of `err_beg`) and loosely adhere to the UTF-8
//   schema; this computation does NOT validate the encoding.
// - A preview of the file-stream is included in the report, limited to the line
//   on which the error occurs. This preview highlights the erroneous token in
//   bold red and also prints a squiggly underline beneath the token.
// - Internally, this routine checks the value of the `NO_COLOR` environment
//   variable. If this variable is present and is set to anything other than an
//   empty-string, all highlighting and styling will be removed from the output.
//   For details, refer to `https://no-color.org/`.
__attribute__((format(printf, 5, 6)))
void report(
    const char *filename,
    const char *data,
    const char *err_beg,
    const char *err_end,
    const char *fmt,
    ...
);

#endif // DATAPROC_REPORT_H
