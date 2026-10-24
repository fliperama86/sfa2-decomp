/* Driver of sounds.c (port/tools/test_arcade_sounds.py): it links whatever table it is given.
 *   find BANK START             "code0 code1 method" of the cell, exit 1 when there is none
 *   bank FILE SLOT              the bank's id (0: none)
 *   count                       "banks N sounds M"
 *   plan HEADFILE FILESIZE NAME the chunks to build: "kind at size param body bank count" a line, then "skipped N"
 *   write HEADFILE FILESIZE NAME OUTDIR   writes every chunk of the plan to OUTDIR/<at>
 *   vab FILE BODY               the library's open (sdk/libsnd/vs_vh.c) on the header in FILE: "ok id ps total" or "fail"
 *   sep FILE COUNT              the library's walk of a sequence file (sdk/libsnd/sepinit.c): "ok bytes" or "fail" */
#include "sounds.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static unsigned char *slurp(const char *path, unsigned *n)
{
    FILE *f = fopen(path, "rb");
    unsigned char *b;
    long size;
    if (!f) exit(2);
    fseek(f, 0, SEEK_END);
    size = ftell(f);
    fseek(f, 0, SEEK_SET);
    b = malloc((size_t)size + 1);
    if (!b || fread(b, 1, (size_t)size, f) != (size_t)size) exit(2);
    fclose(f);
    *n = (unsigned)size;
    return b;
}

static unsigned le16(const unsigned char *p) { return (unsigned)p[0] | (unsigned)p[1] << 8; }
static unsigned le32(const unsigned char *p) { return le16(p) | le16(p + 2) << 16; }

/* sdk/libsnd/vs_vh.c SsVabOpenHeadWithMode, on a header in memory: what it reads and checks. The sum is the bytes it expects the body to be. */
static int vab_open(const unsigned char *h, unsigned size, unsigned *id, unsigned *ps_out, unsigned long *total_out)
{
    unsigned ver, ps, vs, max, i, table;
    unsigned long total = 0;
    if (size < 0x20 || h[1] != 'B' || h[2] != 'A' || h[3] != 'V') return -1;
    ver = le32(h + 4);
    max = h[0] == 'p' && ver >= 5 ? 0x80u : 0x40u;
    ps = le16(h + 0x12);
    vs = h[0x16];
    if (ps > max) return -1;
    table = 0x20u + 16u * max + (ps << 9);          /* the table of sample sizes follows the tone records */
    if (table + 512u > size) return -1;
    for (i = 0; i < 256; i++)
        if (vs >= i) total += (unsigned long)le16(h + table + 2u * i) * (ver >= 5 ? 8u : 4u);
    if (total > 0x80000ul) return -1;
    for (i = 0; i < max; i++)
        (void)h[0x20 + 16u * i];                     /* the tone count of each program: read */
    *id = le16(h + 8);
    *ps_out = ps;
    *total_out = total;
    return 0;
}

/* sdk/libsnd/sepinit.c _SsInitSoundSep: bytes the sequence at p takes, or -1. */
static int sep_one(const unsigned char *p, unsigned avail, unsigned index)
{
    unsigned used = 0, len, tempo;
    if (index == 0) {
        if (p[0] == 'S' || p[0] == 'p') {
            if (p[5] != 0) return -1;
            p += 8;
            used += 8;
        }
    } else {
        p += 2;
        used += 2;
    }
    if (avail < used + 11u + 4u) return -1;
    if (!(p[0] << 8 | p[1])) return -1;              /* resolution 0 divides by zero */
    tempo = (unsigned)p[2] << 16 | (unsigned)p[3] << 8 | p[4];
    if (tempo == 0) return -1;                       /* 60000000 % tempo */
    len = (unsigned)p[7] << 24 | (unsigned)p[8] << 16 | (unsigned)p[9] << 8 | p[10];
    used += 11;
    return (int)(used + len);
}

int main(int argc, char **argv)
{
    unsigned i;
    if (argc >= 4 && !strcmp(argv[1], "find")) {
        const struct arcade_sound *a = arcade_sound_find((unsigned)atoi(argv[2]), (unsigned)atoi(argv[3]));
        if (!a) return 1;
        printf("%u %u %u\n", a->code0, a->code1, a->method);
        return 0;
    }
    if (argc >= 4 && !strcmp(argv[1], "bank")) {
        printf("%u\n", arcade_sound_bank_id(argv[2], (unsigned)atoi(argv[3])));
        return 0;
    }
    if (argc >= 2 && !strcmp(argv[1], "count")) {
        printf("banks %u sounds %u\n", arcade_sound_bank_count, arcade_sound_count);
        for (i = 1; i < arcade_sound_count; i++) {   /* sorted and unique, and every bank id is a bank */
            const struct arcade_sound *a = &arcade_sounds[i - 1], *b = &arcade_sounds[i];
            if (!(a->bank < b->bank || (a->bank == b->bank && a->start < b->start))) { printf("unsorted at %u\n", i); return 1; }
        }
        for (i = 0; i < arcade_sound_count; i++)
            if (!arcade_sounds[i].bank || arcade_sounds[i].bank > arcade_sound_bank_count) { printf("bad bank at %u\n", i); return 1; }
        return 0;
    }
    if (argc >= 5 && (!strcmp(argv[1], "plan") || !strcmp(argv[1], "write"))) {
        unsigned hn, count, skipped;
        unsigned char *head = slurp(argv[2], &hn);
        struct arcade_sound_chunk c[ARCADE_SOUND_CHUNKS];
        count = arcade_sound_plan(head, hn, (unsigned)strtoul(argv[3], NULL, 0), argv[4], c, ARCADE_SOUND_CHUNKS, &skipped);
        for (i = 0; i < count; i++) {
            if (!strcmp(argv[1], "plan")) printf("%u %u %u 0x%08x %u %u %u\n", c[i].kind, c[i].at, c[i].size, c[i].param, c[i].body, c[i].bank, c[i].count);
            else {
                char path[512];
                unsigned char *b = malloc(c[i].size);
                FILE *f;
                if (argc < 6 || !b) return 2;
                arcade_sound_write(&c[i], b);
                snprintf(path, sizeof path, "%s/%u", argv[5], c[i].at);
                f = fopen(path, "wb");
                if (!f || fwrite(b, 1, c[i].size, f) != c[i].size) return 2;
                fclose(f);
                free(b);
            }
        }
        if (!strcmp(argv[1], "plan")) printf("skipped %u\n", skipped);
        free(head);
        return 0;
    }
    if (argc >= 4 && !strcmp(argv[1], "vab")) {
        unsigned n, id, ps;
        unsigned long total;
        unsigned char *h = slurp(argv[2], &n);
        if (vab_open(h, n, &id, &ps, &total) != 0) { printf("fail\n"); free(h); return 0; }
        free(h);
        printf("ok %u %u %lu %s\n", id, ps, total, total == strtoul(argv[3], NULL, 0) ? "sum-equals-body" : "sum-differs");
        return 0;
    }
    if (argc >= 4 && !strcmp(argv[1], "sep")) {
        unsigned n, count = (unsigned)atoi(argv[3]), at = 0;
        unsigned char *b = slurp(argv[2], &n);
        for (i = 0; i < count; i++) {
            int used = sep_one(b + at, n - at, i);
            if (used < 0 || at + (unsigned)used > n) { printf("fail %u\n", i); free(b); return 0; }
            at += (unsigned)used;
        }
        printf("ok %u\n", at);
        free(b);
        return 0;
    }
    return 2;
}
