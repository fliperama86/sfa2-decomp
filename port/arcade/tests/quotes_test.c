/* Driver of test_arcade_quotes.py and quotes.py --compare-c.
 *   quotes_test table                      print the linked table (arcade_quotes): `centre line_height count module_slot blob_slot`, then a line
 *                                          per fighter: `file kind text offsets offset_count lists lists_room streams streams_room segment
 *                                          segment_room`
 *   quotes_test build FILE KIND TEXT OFFSETS COUNT LISTS LROOM STREAMS SROOM SEGMENT SROOM CENTRE HEIGHT PROGRAM GFX MODULE OUTDIR
 *                                          arcade_quotes_build for one fighter described by the numbers, with the program, graphics and module
 *                                          read from the files; prints `ok SEGMENT_SIZE` and writes OUTDIR/module, OUTDIR/changed and
 *                                          OUTDIR/segment (SEGMENT_SIZE bytes), or prints `refused MESSAGE` and exits 3
 * Guard bytes after the module, the changed mask and the segment room are checked: a write past them exits 4. Numbers may be written with 0x.
 * Any other failure exits 1. */
#include "../quotes.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define GUARD 64u

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

static int dump(const char *dir, const char *name, const void *p, size_t n)
{
    char path[1024];
    FILE *f;
    int ok;
    snprintf(path, sizeof path, "%s/%s", dir, name);
    f = fopen(path, "wb");
    if (!f) return -1;
    ok = fwrite(p, 1, n, f) == n;
    return fclose(f) == 0 && ok ? 0 : -1;
}

static unsigned long num(const char *s)
{
    return strtoul(s, NULL, 0);
}

static int guard_ok(const uint8_t *p, size_t n)
{
    size_t i;
    for (i = 0; i < GUARD; i++)
        if (p[n + i] != 0xa5) return 0;
    return 1;
}

static int build_cmd(char **a)
{
    struct arcade_quote_fighter f;
    struct arcade_quotes q;
    size_t psize = 0, tsize = 0, msize = 0;
    uint8_t *program = slurp(a[13], &psize), *tiles = slurp(a[14], &tsize), *mod_in = slurp(a[15], &msize);
    uint8_t *module = NULL, *changed = NULL, *segment = NULL;
    uint32_t seg_size = 0;
    char err[256];
    int rc, status = 1;
    if (!program || !tiles || !mod_in) goto out;
    memset(&f, 0, sizeof f);
    f.file = a[0];
    f.kind = (uint16_t)num(a[1]);
    f.text = (uint32_t)num(a[2]);
    f.offsets = (uint32_t)num(a[3]);
    f.offset_count = (uint16_t)num(a[4]);
    f.lists = (uint32_t)num(a[5]);
    f.lists_room = (uint32_t)num(a[6]);
    f.streams = (uint32_t)num(a[7]);
    f.streams_room = (uint32_t)num(a[8]);
    f.segment = (uint32_t)num(a[9]);
    f.segment_room = (uint32_t)num(a[10]);
    memset(&q, 0, sizeof q);
    q.module_slot = 1;
    q.blob_slot = 0x10;
    q.centre = (int16_t)num(a[11]);
    q.line_height = (uint16_t)num(a[12]);
    q.fighters = &f;
    q.count = 1;
    module = malloc(msize + GUARD);
    changed = calloc(msize + GUARD, 1);
    segment = malloc((size_t)f.segment_room + GUARD);
    if (!module || !changed || !segment) goto out;
    memcpy(module, mod_in, msize);
    memset(module + msize, 0xa5, GUARD);
    memset(changed + msize, 0xa5, GUARD);
    memset(segment, 0xee, f.segment_room);
    memset(segment + f.segment_room, 0xa5, GUARD);
    rc = arcade_quotes_build(&q, &f, program, psize, tiles, tsize, module, changed, msize, segment, &seg_size, err, sizeof err);
    if (!guard_ok(module, msize) || !guard_ok(changed, msize) || !guard_ok(segment, f.segment_room)) {
        printf("a write past an end\n");
        status = 4;
    } else if (rc) {
        printf("refused %s\n", err);
        status = 3;
    } else if (seg_size > f.segment_room) {
        printf("segment size %u over its room\n", (unsigned)seg_size);
        status = 4;
    } else if (dump(a[16], "module", module, msize) || dump(a[16], "changed", changed, msize) || dump(a[16], "segment", segment, seg_size)) {
        status = 1;
    } else {
        printf("ok %u\n", (unsigned)seg_size);
        status = 0;
    }
out:
    free(program);
    free(tiles);
    free(mod_in);
    free(module);
    free(changed);
    free(segment);
    return status;
}

int main(int argc, char **argv)
{
    if (argc == 2 && !strcmp(argv[1], "table")) {
        unsigned i;
        printf("%d %u %u %u %u\n", arcade_quotes.centre, arcade_quotes.line_height, arcade_quotes.count, arcade_quotes.module_slot, arcade_quotes.blob_slot);
        for (i = 0; i < arcade_quotes.count; i++) {
            const struct arcade_quote_fighter *f = &arcade_quotes.fighters[i];
            printf("%s %u %#x %#x %u %#x %u %#x %u %#x %u\n", f->file, f->kind, f->text, f->offsets, f->offset_count, f->lists, f->lists_room,
                   f->streams, f->streams_room, f->segment, f->segment_room);
        }
        return 0;
    }
    if (argc == 19 && !strcmp(argv[1], "build"))
        return build_cmd(argv + 2);
    fprintf(stderr, "usage: see the head of this file\n");
    return 1;
}
