/* Driver of test_arcade_program_more.py: synthetic programs and the generated tables only.
 *   program_more_test effects PROGRAM SPEC OUTDIR   build the effects of a made-up descriptor from a made-up arcade program
 *   program_more_test tables                        sanity of the checked-in effects descriptor and relocation table
 *   program_more_test relocs                        the relocation writer's order, bounds and byte order
 *   program_more_test pieces                        the layout letters 'n' and 'e' of program.c
 * SPEC is text: "address positions_size lists_size gap columns rows segment_count drop_count", then "address count bank" per
 * segment, then "record cell" per drop. OUTDIR gets data.bin and known.bin. */
#include "../program_more.h"
#include "../program.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
    snprintf(path, sizeof path, "%s/%s", dir, name);
    f = fopen(path, "wb");
    if (!f || (n && fwrite(p, 1, n, f) != n)) return -1;
    return fclose(f);
}

static int effects(const char *prog_path, const char *spec_path, const char *dir)
{
    size_t size = 0;
    uint8_t *prog = slurp(prog_path, &size);
    FILE *spec = fopen(spec_path, "r");
    struct arcade_effects e;
    struct arcade_fighter_segment *segs = NULL;
    struct arcade_effects_drop *drops = NULL;
    struct arcade_effects_out out;
    unsigned long address, ps, ls, gap, cols, rows, ns, nd, i;
    char err[256];
    int rc = 3;

    if (!prog || !spec || fscanf(spec, "%lu %lu %lu %lu %lu %lu %lu %lu", &address, &ps, &ls, &gap, &cols, &rows, &ns, &nd) != 8) goto done;
    segs = calloc(ns ? ns : 1, sizeof *segs);
    drops = calloc(nd ? nd : 1, sizeof *drops);
    if (!segs || !drops) goto done;
    for (i = 0; i < ns; i++) {
        unsigned long a, c, b;
        if (fscanf(spec, "%lu %lu %lu", &a, &c, &b) != 3) goto done;
        segs[i].address = (uint32_t)a;
        segs[i].count = (uint16_t)c;
        segs[i].bank = (uint32_t)b;
    }
    for (i = 0; i < nd; i++) {
        unsigned long a, c;
        if (fscanf(spec, "%lu %lu", &a, &c) != 2) goto done;
        drops[i].record = (uint32_t)a;
        drops[i].cell = (uint16_t)c;
    }
    memset(&e, 0, sizeof e);
    e.address = (uint32_t)address;
    e.positions_size = (uint32_t)ps;
    e.lists_size = (uint32_t)ls;
    e.gap_size = (uint32_t)gap;
    e.picture_columns = (uint16_t)cols;
    e.picture_rows = (uint16_t)rows;
    e.segments = segs;
    e.segment_count = (unsigned)ns;
    e.drops = drops;
    e.drop_count = (unsigned)nd;
    if (arcade_effects_build(prog, size, &e, &out, err, sizeof err) != 0) {
        printf("error: %s\n", err);
        if (out.data || out.known || out.size) printf("output left allocated\n");
        rc = 1;
        goto done;
    }
    if (dump(dir, "data.bin", out.data, out.size) || dump(dir, "known.bin", out.known, out.size)) rc = 3;
    else rc = 0;
    arcade_effects_free(&out);
    assert(out.data == NULL && out.known == NULL);
done:
    free(prog);
    free(segs);
    free(drops);
    if (spec) fclose(spec);
    return rc;
}

static int tables(void)
{
    const struct arcade_effects *e = &arcade_effects;
    unsigned i, n = 0;
    uint32_t last = 0;
    assert(arcade_relocs_valid(arcade_relocs, arcade_reloc_count) == 0);
    assert(arcade_reloc_count > 1000);
    assert(e->address == 0x80172a48u && e->address + e->positions_size == 0x80173c9cu);
    assert(e->address >= ARCADE_PS1_DATA_START && e->address + e->positions_size + e->lists_size <= ARCADE_PS1_DATA_END);
    assert(e->segment_count > 0 && e->segments && e->drops);
    for (i = 0; i < e->segment_count; i++) {
        assert(e->segments[i].address && !(e->segments[i].address & 1) && e->segments[i].count > 0);
        assert((e->segments[i].bank & 0xffffu) == 0 && e->segments[i].bank <= 0x30000u);
        n += e->segments[i].count;
    }
    assert(n == 216);
    for (i = 0; i < e->drop_count; i++) {
        assert(i == 0 || e->drops[i].record > e->drops[i - 1].record || (e->drops[i].record == e->drops[i - 1].record && e->drops[i].cell > e->drops[i - 1].cell));
        last = e->drops[i].record;
    }
    assert(last);
    assert(e->gap_size < e->lists_size && e->picture_columns * e->picture_rows == 336);
    return 0;
}

static int relocs(void)
{
    uint8_t ram[32];
    struct arcade_reloc r[3] = {{ARCADE_PS1_DATA_START, 0x80118cd4u}, {ARCADE_PS1_DATA_START + 8, 0x801e9f44u}, {ARCADE_PS1_DATA_START + 28, 0x1u}};
    memset(ram, 0xaa, sizeof ram);
    assert(arcade_relocs_valid(r, 3) == 0);
    assert(arcade_relocs_apply(r, 3, ram, sizeof ram) == 0);
    assert(ram[0] == 0xd4 && ram[1] == 0x8c && ram[2] == 0x11 && ram[3] == 0x80);
    assert(ram[4] == 0xaa && ram[7] == 0xaa);
    assert(ram[8] == 0x44 && ram[9] == 0x9f && ram[10] == 0x1e && ram[11] == 0x80);
    assert(ram[28] == 1 && ram[29] == 0 && ram[30] == 0 && ram[31] == 0);
    /* a word that does not fit is not written, the others are */
    memset(ram, 0xaa, sizeof ram);
    assert(arcade_relocs_apply(r, 3, ram, 31) == -1);
    assert(ram[28] == 0xaa && ram[0] == 0xd4);
    r[2].location = ARCADE_PS1_DATA_START - 4;
    assert(arcade_relocs_apply(r + 2, 1, ram, sizeof ram) == -1);
    /* order, alignment, bounds */
    r[2].location = ARCADE_PS1_DATA_START + 28;
    assert(arcade_relocs_valid(r, 3) == 0);
    r[1].location = ARCADE_PS1_DATA_START;
    assert(arcade_relocs_valid(r, 3) == -1);                      /* not increasing */
    r[1].location = ARCADE_PS1_DATA_START + 6;
    assert(arcade_relocs_valid(r, 3) == -1);                      /* not aligned */
    r[1].location = ARCADE_PS1_DATA_END;
    assert(arcade_relocs_valid(r, 2) == -1);                      /* past the end */
    r[1].location = ARCADE_PS1_DATA_END - 4;
    assert(arcade_relocs_valid(r, 2) == 0);
    return 0;
}

static int pieces(void)
{
    unsigned char prog[64], out[64];
    struct arcade_piece p = {ARCADE_PS1_DATA_START, 8, 4, "n", "t"};
    struct arcade_piece t[2];
    unsigned i;
    for (i = 0; i < sizeof prog; i++) prog[i] = (unsigned char)(i * 7 + 1);
    for (i = 0; i < 20; i += 2) prog[i] = 0;                       /* 16-bit values that fit a byte */
    /* 'n': eight bytes from eight 16-bit values (16 arcade bytes) */
    assert(arcade_piece_build(prog, sizeof prog, &p, out) == 0);
    for (i = 0; i < 8; i++) assert(out[i] == prog[4 + 2 * i + 1]);
    prog[4 + 2 * 3] = 1;                                             /* a value that does not fit */
    assert(arcade_piece_build(prog, sizeof prog, &p, out) == -1);
    prog[4 + 2 * 3] = 0;
    p.source = 52; assert(arcade_piece_build(prog, sizeof prog, &p, out) == -1);   /* 16 arcade bytes run past the end */
    p.source = 48; for (i = 48; i < 64; i += 2) prog[i] = 0; assert(arcade_piece_build(prog, sizeof prog, &p, out) == 0);   /* ends at the end */
    /* 'e': a 32-bit little-endian value from each arcade byte */
    p.units = "e"; p.size = 12; p.source = 6;
    assert(arcade_piece_build(prog, sizeof prog, &p, out) == 0);
    for (i = 0; i < 3; i++) assert(out[4 * i] == prog[6 + i] && !out[4 * i + 1] && !out[4 * i + 2] && !out[4 * i + 3]);
    p.source = 62; p.size = 8; assert(arcade_piece_build(prog, sizeof prog, &p, out) == 0);    /* 2 arcade bytes, the last two */
    p.source = 62; p.size = 12; assert(arcade_piece_build(prog, sizeof prog, &p, out) == -1);  /* 3 arcade bytes, one past the end */
    p.size = 6; assert(arcade_piece_build(prog, sizeof prog, &p, out) == -1);                  /* not a whole number of 4-byte fields */
    /* mixed with the old letters: b e n twice is 14 PS1 bytes from 2 + 1 + 2 = 5 arcade bytes, twice */
    p.units = "ben"; p.size = 14; p.source = 20;
    for (i = 20; i < 30; i++) prog[i] = (unsigned char)(0x40 + i);
    prog[23] = 0; prog[28] = 0;                                                  /* the high bytes of the two narrowed values */
    assert(arcade_piece_build(prog, sizeof prog, &p, out) == 0);
    assert(out[0] == prog[20] && out[1] == prog[21]);                            /* b */
    assert(out[2] == prog[22] && out[3] == 0 && out[4] == 0 && out[5] == 0);     /* e */
    assert(out[6] == prog[24]);                                                  /* n */
    assert(out[7] == prog[25] && out[8] == prog[26]);                            /* b again */
    assert(out[9] == prog[27] && !out[10] && !out[11] && !out[12]);              /* e */
    assert(out[13] == prog[29]);                                                 /* n */
    /* the table checker takes the letters, and a size the letters do not fill is refused */
    t[0] = (struct arcade_piece){ARCADE_PS1_DATA_START, 8, 4, "e", "a"};
    t[1] = (struct arcade_piece){ARCADE_PS1_DATA_START + 8, 6, 4, "n", "b"};
    assert(arcade_pieces_valid(t, 2) == 0);
    t[1].size = 6; t[1].units = "e";
    assert(arcade_pieces_valid(t, 2) == -1);                       /* 6 is not a whole number of 4-byte fields */
    return 0;
}

int main(int argc, char **argv)
{
    if (argc == 5 && strcmp(argv[1], "effects") == 0) return effects(argv[2], argv[3], argv[4]);
    if (argc == 2 && strcmp(argv[1], "tables") == 0) return tables();
    if (argc == 2 && strcmp(argv[1], "relocs") == 0) return relocs();
    if (argc == 2 && strcmp(argv[1], "pieces") == 0) return pieces();
    fprintf(stderr, "usage: program_more_test effects PROGRAM SPEC OUTDIR | tables | relocs | pieces\n");
    return 2;
}
