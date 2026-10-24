/* Driver of test_arcade_texts.py and texts.py --compare-c.
 *   texts_test table                                 print the linked table (arcade_text_modules): `file slot size count`, then a line
 *                                                    `offset source chars size form` per string
 *   texts_test row INDEX PROGRAM OUT                 module INDEX of the linked table: arcade_text_module_valid, then every string built
 *                                                    from the program file; OUT gets the strings one after the other (0xee for one
 *                                                    refused); prints a character a string: `.` built, `x` refused
 *   texts_test string PROGRAM FORM SOURCE CHARS SIZE OUT   arcade_text_build of one string; prints `ok` and writes SIZE bytes, or
 *                                                    `refused` (exit 3)
 *   texts_test valid SPEC                            arcade_text_module_valid of the module the file describes: a line
 *                                                    `file slot size count`, then a line `offset source chars size form` per string;
 *                                                    prints `valid` (exit 0) or `invalid` (exit 3)
 *   texts_test nulls                                 prints `ok` when NULL arguments are refused by both functions
 * Numbers may be written with 0x. Any other failure exits with 1. */
#include "../texts.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_STRINGS 4096

static uint8_t *slurp(const char *path, size_t *n)
{
    FILE *f = fopen(path, "rb");
    uint8_t *p;
    long len;
    if (!f) return NULL;
    fseek(f, 0, SEEK_END);
    len = ftell(f);
    fseek(f, 0, SEEK_SET);
    p = malloc((size_t)len + 1);
    if (!p || fread(p, 1, (size_t)len, f) != (size_t)len) {
        fclose(f);
        free(p);
        return NULL;
    }
    fclose(f);
    *n = (size_t)len;
    return p;
}

static int dump(const char *path, const void *p, size_t n)
{
    FILE *f = fopen(path, "wb");
    int ok;
    if (!f) return -1;
    ok = fwrite(p, 1, n, f) == n;
    return fclose(f) == 0 && ok ? 0 : -1;
}

static unsigned long num(const char *s)
{
    return strtoul(s, NULL, 0);
}

static int row_cmd(unsigned long index, const char *program_path, const char *out_path)
{
    const struct arcade_text_module *m;
    size_t program_size = 0, total = 0, at = 0;
    uint8_t *program, *out;
    char marks[MAX_STRINGS + 1];
    unsigned k;
    int status = 1;
    if (index >= arcade_text_module_count) return 1;
    m = &arcade_text_modules[index];
    if (arcade_text_module_valid(m)) {
        printf("invalid\n");
        return 3;
    }
    if (m->count > MAX_STRINGS) return 1;
    program = slurp(program_path, &program_size);
    for (k = 0; k < m->count; k++) total += m->strings[k].size;
    out = program ? malloc(total + 1u) : NULL;
    if (out) {
        for (k = 0; k < m->count; k++) {
            const struct arcade_text_string *s = &m->strings[k];
            uint8_t *piece = malloc((size_t)s->size + 1u);
            if (!piece) goto done;
            if (arcade_text_build(program, program_size, s, piece)) {
                memset(out + at, 0xee, s->size);
                marks[k] = 'x';
            } else {
                memcpy(out + at, piece, s->size);
                marks[k] = '.';
            }
            free(piece);
            at += s->size;
        }
        marks[m->count] = 0;
        if (!dump(out_path, out, total)) {
            printf("%s\n", marks);
            status = 0;
        }
    }
done:
    free(program);
    free(out);
    return status;
}

static int read_spec(const char *path, struct arcade_text_module *m, struct arcade_text_string *strings, char *file)
{
    FILE *f = fopen(path, "r");
    unsigned long slot, size, count, k;
    if (!f) return -1;
    if (fscanf(f, "%63s %lu %lu %lu", file, &slot, &size, &count) != 4 || count > MAX_STRINGS) goto bad;
    for (k = 0; k < count; k++) {
        unsigned long o, a, c, s, form;
        if (fscanf(f, "%lu %lu %lu %lu %lu", &o, &a, &c, &s, &form) != 5) goto bad;
        strings[k].offset = (uint32_t)o;
        strings[k].source = (uint32_t)a;
        strings[k].chars = (uint16_t)c;
        strings[k].size = (uint16_t)s;
        strings[k].form = (uint8_t)form;
    }
    fclose(f);
    m->file = file;
    m->slot = (uint16_t)slot;
    m->size = (uint32_t)size;
    m->strings = strings;
    m->count = (unsigned)count;
    return 0;
bad:
    fclose(f);
    return -1;
}

int main(int argc, char **argv)
{
    if (argc == 2 && !strcmp(argv[1], "table")) {
        unsigned i, k;
        for (i = 0; i < arcade_text_module_count; i++) {
            const struct arcade_text_module *m = &arcade_text_modules[i];
            printf("%s %u %u %u\n", m->file, m->slot, (unsigned)m->size, m->count);
            for (k = 0; k < m->count; k++) {
                const struct arcade_text_string *s = &m->strings[k];
                printf("%u %u %u %u %u\n", (unsigned)s->offset, (unsigned)s->source, s->chars, s->size, s->form);
            }
        }
        return 0;
    }
    if (argc == 5 && !strcmp(argv[1], "row")) return row_cmd(num(argv[2]), argv[3], argv[4]);
    if (argc == 8 && !strcmp(argv[1], "string")) {
        size_t program_size = 0;
        uint8_t *program = slurp(argv[2], &program_size), *out;
        struct arcade_text_string s;
        int status = 1;
        s.form = (uint8_t)num(argv[3]);
        s.source = (uint32_t)num(argv[4]);
        s.chars = (uint16_t)num(argv[5]);
        s.size = (uint16_t)num(argv[6]);
        out = program ? malloc((size_t)s.size + 1u) : NULL;   /* exactly the string's size is usable, as the contract says */
        if (out) {
            out = realloc(out, s.size ? s.size : 1u);
            if (arcade_text_build(program, program_size, &s, out)) {
                printf("refused\n");
                status = 3;
            } else if (!dump(argv[7], out, s.size)) {
                printf("ok\n");
                status = 0;
            }
        }
        free(program);
        free(out);
        return status;
    }
    if (argc == 3 && !strcmp(argv[1], "valid")) {
        struct arcade_text_module m;
        struct arcade_text_string strings[MAX_STRINGS];
        char file[64];
        if (read_spec(argv[2], &m, strings, file)) return 1;
        if (arcade_text_module_valid(&m)) {
            printf("invalid\n");
            return 3;
        }
        printf("valid\n");
        return 0;
    }
    if (argc == 2 && !strcmp(argv[1], "nulls")) {
        uint8_t program[8] = {0}, out[8];
        struct arcade_text_string s = {0, 0, 1, 4, ARCADE_TEXT_PLAIN};
        struct arcade_text_module m = {"X.PAC", 0, 16, NULL, 1};
        int ok = arcade_text_build(NULL, 8, &s, out) && arcade_text_build(program, 8, NULL, out) && arcade_text_build(program, 8, &s, NULL) &&
                 arcade_text_module_valid(NULL) && arcade_text_module_valid(&m);
        m.file = NULL;
        m.count = 0;
        ok = ok && arcade_text_module_valid(&m);
        printf(ok ? "ok\n" : "FAILED\n");
        return ok ? 0 : 3;
    }
    fprintf(stderr, "usage: see the head of this file\n");
    return 1;
}
