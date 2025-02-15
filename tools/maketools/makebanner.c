#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BANNER_SIZE 0x840

#define ICON_BITMAP_SIZE    0x200
#define ICON_BITMAP_OFFSET  0x20
#define ICON_PALETTE_SIZE   0x20
#define ICON_PALETTE_OFFSET 0x220

#define U16_TITLE_LEN   0x80 // 128 characters
#define TITLE_OFFSET_JP 0x240
#define TITLE_OFFSET_EN 0x340
#define TITLE_OFFSET_FR 0x440
#define TITLE_OFFSET_DE 0x540
#define TITLE_OFFSET_IT 0x640
#define TITLE_OFFSET_ES 0x740

static uint8_t banner[BANNER_SIZE] = { 0 };

static void usage(FILE *f)
{
    fprintf(f, "Usage: makebanner [OPTIONS] OUTFILE\n\n");
    fprintf(f, "Write a “ROM banner” file for use on the Nintendo DS boot menu.\n\n");
    fprintf(f, "Options:\n");
    fprintf(f, "  --icon-bitmap  BITMAP    Read the file at BITMAP as a 32x32 bitmap of 8x8\n");
    fprintf(f, "                           tiles in row-major order with 4-bit color depth.\n");
    fprintf(f, "                           This bitmap will be used for the ROM's icon on the\n");
    fprintf(f, "                           DS boot menu.\n");
    fprintf(f, "  --icon-palette PALETTE   Read the file at PALETTE as a 16-color 15-bit\n");
    fprintf(f, "                           indexed palette. This palette will be used to color\n");
    fprintf(f, "                           the ROM's icon on the DS boot menu.\n");
    fprintf(f, "  --title TITLE            The ROM's title. Assumed to be passed as a UTF-8\n");
    fprintf(f, "                           encoded string of Unicode code points in the range\n");
    fprintf(f, "                           0000..FFFF. Code points requiring at least 17-bits\n");
    fprintf(f, "                           are not supported by the output specification, and\n");
    fprintf(f, "                           the title string must not exceed 128 characters.\n");
    fprintf(f, "  --developer DEVELOPER    The ROM's developer. Interpreted identically to the\n");
    fprintf(f, "                           --title option, and follows the same rules. The\n");
    fprintf(f, "                           interpreted value will be appended to the title,\n");
    fprintf(f, "                           separated by a line-feed character (0x000A).\n");
}

static void die(const char *fmt, ...)
{
    va_list va_args;
    va_start(va_args, fmt);

    fputs("makebanner: ", stderr);
    vfprintf(stderr, fmt, va_args);
    fputc('\n', stderr);

    va_end(va_args);
    exit(EXIT_FAILURE);
}

static void die_usage(const char *fmt, ...)
{
    va_list va_args;
    va_start(va_args, fmt);

    fputs("makebanner: ", stderr);
    vfprintf(stderr, fmt, va_args);
    fputc('\n', stderr);
    fputc('\n', stderr);

    va_end(va_args);
    usage(stderr);
    exit(EXIT_FAILURE);
}

static void read_file(const char *fin_name, unsigned char *buf, size_t length)
{
    FILE *fin = fopen(fin_name, "rb");
    if (!fin) {
        die("could not open file to read: %s", fin_name);
    }

    fread(buf, 1, length, fin);
    fclose(fin);
}

static size_t decode_utf8(uint16_t *dst, const unsigned char *src)
{
    int len = 0;
    for (; src[0]; dst++, src++, len++) {
        if (!(src[0] & 0x80)) {
            *dst = src[0];
        } else if ((src[0] & 0xE0) == 0xC0) {
            *dst = ((src[0] & 0x1F) << 6)
                | ((src[1] & 0x3F) << 0);
            src++;
        } else if ((src[0] & 0xF0) == 0xE0) {
            *dst = ((src[0] & 0x0F) << 12)
                | ((src[1] & 0x3F) << 6)
                | ((src[2] & 0x3F) << 0);
            src++;
            src++;
        } else if ((src[0] & 0xF8) == 0xF0) {
            die_usage("unsupported 4-byte encoding detected");
        } else {
            die_usage("character 0x%X does not have a valid UTF-8 byte-marker", *src);
        }
    }

    return len;
}

static void copy_u16_buf_le(const uint16_t *buf, size_t offset, size_t length)
{
    if (offset + (length * sizeof(uint16_t)) > BANNER_SIZE) {
        die("cannot copy u16 buffer of length %d to banner offset 0x%X", length, offset);
    }

    uint8_t *banner_p = banner + offset;
    for (size_t i = 0; i < length; i++) {
        *(banner_p++) = buf[i] & 0xFF;
        *(banner_p++) = buf[i] >> 8;
    }
}

static uint16_t calc_crc16(uint8_t *data, size_t length, uint16_t crc)
{
    static uint16_t crc_table[16] = {
        0x0000,
        0xCC01,
        0xD801,
        0x1400,
        0xF001,
        0x3C00,
        0x2800,
        0xE401,
        0xA001,
        0x6C00,
        0x7800,
        0xB401,
        0x5000,
        0x9C01,
        0x8801,
        0x4400,
    };

    uint16_t x = 0;
    uint16_t y;
    uint16_t bit = 0;
    uint8_t *end = data + length;
    while (data < end) {
        if (bit == 0) {
            x = data[0] | (data[1] << 8);
        }

        y = crc_table[crc & 15];
        crc >>= 4;
        crc ^= y;

        y = crc_table[(x >> bit) & 15];
        crc ^= y;
        bit += 4;

        if (bit == 16) {
            data += 2;
            bit = 0;
        }
    }

    return crc;
}

int main(int argc, char **argv)
{
    if (argc == 1 || strcmp(argv[1], "--help") == 0) {
        usage(stdout);
        exit(EXIT_SUCCESS);
    }

    uint16_t title[U16_TITLE_LEN] = { 0 };
    uint16_t developer[U16_TITLE_LEN] = { 0 };
    size_t title_len = 0;
    size_t developer_len = 0;

    for (int i = 1; i < argc - 1; i++) {
        if (strcmp(argv[i], "--icon-bitmap") == 0) {
            read_file(argv[++i], banner + ICON_BITMAP_OFFSET, ICON_BITMAP_SIZE);
        } else if (strcmp(argv[i], "--icon-palette") == 0) {
            read_file(argv[++i], banner + ICON_PALETTE_OFFSET, ICON_PALETTE_SIZE);
        } else if (strcmp(argv[i], "--title") == 0) {
            title_len = decode_utf8(title, (const unsigned char *)argv[++i]);
        } else if (strcmp(argv[i], "--developer") == 0) {
            developer_len = decode_utf8(developer, (const unsigned char *)argv[++i]);
        } else if (argv[i][0] == '-') {
            die("unrecognized option: %s", argv[i]);
        }
    }

    if (developer_len > 0) {
        title[title_len++] = 0x000A;
        memcpy(title + title_len, developer, sizeof(uint16_t) * developer_len);
    }

    banner[0x00] = 0x01; // version 1
    copy_u16_buf_le(title, TITLE_OFFSET_JP, U16_TITLE_LEN);
    copy_u16_buf_le(title, TITLE_OFFSET_EN, U16_TITLE_LEN);
    copy_u16_buf_le(title, TITLE_OFFSET_FR, U16_TITLE_LEN);
    copy_u16_buf_le(title, TITLE_OFFSET_DE, U16_TITLE_LEN);
    copy_u16_buf_le(title, TITLE_OFFSET_IT, U16_TITLE_LEN);
    copy_u16_buf_le(title, TITLE_OFFSET_ES, U16_TITLE_LEN);

    uint16_t crc16 = calc_crc16(banner + 0x20, BANNER_SIZE - 0x20, 0xFFFF);
    banner[0x02] = crc16 & 0xFF;
    banner[0x03] = (crc16 >> 8);

    const char *fout_name = argv[argc - 1];
    FILE *fout = fopen(fout_name, "wb");
    if (!fout) {
        die("could not open file to write: %s", fout_name);
    }

    fwrite(banner, 1, BANNER_SIZE, fout);
    fclose(fout);

    exit(EXIT_SUCCESS);
}
