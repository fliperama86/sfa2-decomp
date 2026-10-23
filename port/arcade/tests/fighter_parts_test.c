/* Driver of test_arcade_fighter_parts_c.py: synthetic programs and graphics and the generated descriptor table only.
 *   fighter_parts_test palettes PROG SPEC OUT        build a palette chunk; OUT gets the 2048 bytes
 *   fighter_parts_test portrait PROG TILES SPEC OUT  build a portrait page; OUT gets the 32768 bytes
 *   fighter_parts_test shared PROG TILES OUT         build the shared page
 *   fighter_parts_test table                         sanity of the checked-in descriptor table
 * SPEC (palettes) is text: "fighter layout split keep_hex ramp_count" then base_hex and ramp_count start words in hex.
 * SPEC (portrait) is text: "fighter shadow_hex flags drop_x drop_y".
 * Exit status: 0 built, 1 refused (a line "error: ..." on stdout), 2 the page stays the disc's, 3 a setup failure. */
#include "../fighter_parts.h"
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

static int dump(const char *path, const void *p, size_t n)
{
    FILE *f = fopen(path, "wb");
    if (!f || fwrite(p, 1, n, f) != n) return -1;
    return fclose(f);
}

static int palettes(const char *prog_path, const char *spec_path, const char *out_path)
{
    size_t size = 0;
    uint8_t *prog = slurp(prog_path, &size), out[ARCADE_PARTS_PALETTE_BYTES];
    FILE *spec = fopen(spec_path, "r");
    struct arcade_parts_file f;
    uint16_t ramps[ARCADE_PARTS_RAMPS];
    unsigned fighter, layout, split, count, i, v;
    unsigned long long keep;
    unsigned long base;
    uint64_t kept = 0;
    char err[256];
    int rc = 3;
    if (!prog || !spec || fscanf(spec, "%u %u %u %llx %u %lx", &fighter, &layout, &split, &keep, &count, &base) != 6) goto done;
    memset(ramps, 0xff, sizeof ramps);
    for (i = 0; i < count; i++) {
        if (i >= ARCADE_PARTS_RAMPS || fscanf(spec, "%x", &v) != 1) goto done;
        ramps[i] = (uint16_t)v;
    }
    memset(&f, 0, sizeof f);
    f.file = "TEST.PAC";
    f.fighter = (uint8_t)fighter;
    f.layout = (uint8_t)layout;
    f.split = (uint8_t)split;
    f.palette_chunks = 1;
    f.base = (uint32_t)base;
    f.keep = keep;
    f.ramps = count ? ramps : NULL;
    if (arcade_parts_palettes(prog, size, &f, out, &kept, err, sizeof err)) {
        printf("error: %s\n", err);
        rc = 1;
        goto done;
    }
    printf("kept %llx\n", (unsigned long long)kept);
    rc = dump(out_path, out, sizeof out) ? 3 : 0;
done:
    free(prog);
    if (spec) fclose(spec);
    return rc;
}

static int portrait(const char *prog_path, const char *tiles_path, const char *spec_path, const char *out_path)
{
    size_t size = 0, tsize = 0, i;
    uint8_t *prog = slurp(prog_path, &size), *tiles = slurp(tiles_path, &tsize), *out = malloc(ARCADE_PARTS_PAGE_BYTES);
    FILE *spec = fopen(spec_path, "r");
    struct arcade_parts_file f;
    unsigned fighter, shadow, flags;
    int dx, dy, rc = 3, got;
    char err[256];
    if (!prog || !tiles || !spec || !out || fscanf(spec, "%u %x %u %d %d", &fighter, &shadow, &flags, &dx, &dy) != 5) goto done;
    memset(&f, 0, sizeof f);
    f.file = "TEST.PAC";
    f.fighter = (uint8_t)fighter;
    f.shadow = (uint16_t)shadow;
    f.flags = (uint8_t)flags;
    f.drop_x = (int8_t)dx;
    f.drop_y = (int8_t)dy;
    memset(out, 0xa5, ARCADE_PARTS_PAGE_BYTES);
    got = arcade_parts_portrait(prog, size, tiles, tsize, &f, out, err, sizeof err);
    if (got < 0) {
        printf("error: %s\n", err);
        rc = 1;
    } else if (got == 1) {
        for (i = 0; i < ARCADE_PARTS_PAGE_BYTES; i++)
            if (out[i] != 0xa5) {
                printf("wrote into a page that stays the disc's\n");
                break;
            }
        rc = 2;
    } else {
        rc = dump(out_path, out, ARCADE_PARTS_PAGE_BYTES) ? 3 : 0;
    }
done:
    free(prog);
    free(tiles);
    free(out);
    if (spec) fclose(spec);
    return rc;
}

static int shared(const char *prog_path, const char *tiles_path, const char *out_path)
{
    size_t size = 0, tsize = 0;
    uint8_t *prog = slurp(prog_path, &size), *tiles = slurp(tiles_path, &tsize), *out = malloc(ARCADE_PARTS_PAGE_BYTES);
    char err[256];
    int rc = 3;
    if (!prog || !tiles || !out) goto done;
    if (arcade_parts_shared(prog, size, tiles, tsize, out, err, sizeof err)) {
        printf("error: %s\n", err);
        rc = 1;
    } else {
        rc = dump(out_path, out, ARCADE_PARTS_PAGE_BYTES) ? 3 : 0;
    }
done:
    free(prog);
    free(tiles);
    free(out);
    return rc;
}

#define CHECK(cond, ...) do { if (!(cond)) { printf("table: " __VA_ARGS__); printf("\n"); bad++; } } while (0)

static int table(void)
{
    unsigned i, j, bad = 0;
    CHECK(arcade_parts_count == 45, "%u rows, not 45", arcade_parts_count);
    for (i = 0; i < arcade_parts_count; i++) {
        const struct arcade_parts_file *f = &arcade_parts_files[i];
        size_t len = strlen(f->file), pn = strlen(f->pages), ps = 0;
        CHECK(len == 8 || len == 9, "%s: file name", f->file);
        CHECK(!strncmp(f->file, "PL", 2) && !strcmp(f->file + len - 4, ".PAC"), "%s: file name", f->file);
        for (j = 0; j < i; j++) CHECK(strcmp(f->file, arcade_parts_files[j].file), "%s twice", f->file);
        CHECK(f->fighter <= 0x12, "%s: fighter %u", f->file, f->fighter);
        CHECK(f->layout <= ARCADE_PARTS_FIVES, "%s: layout", f->file);
        CHECK(f->split >= 1 && f->split <= 32, "%s: split %u", f->file, f->split);
        CHECK(f->palette_chunks >= 1 && f->palette_chunks <= 2, "%s: palette chunks", f->file);
        CHECK(f->base % 32 == 0 && f->base < 0x300000u, "%s: base %#x", f->file, (unsigned)f->base);
        CHECK(pn >= 1 && pn <= 3, "%s: pages '%s'", f->file, f->pages);
        for (j = 0; j < pn; j++) {
            CHECK(f->pages[j] == 'P' || f->pages[j] == 'V', "%s: page kind", f->file);
            ps += f->pages[j] == 'P';
        }
        CHECK(ps >= 1, "%s: no portrait page", f->file);
        CHECK(f->shadow < 0x1000u, "%s: shadow %#x", f->file, f->shadow);
        CHECK(f->flags <= ARCADE_PARTS_PAGE_DISC, "%s: flags", f->file);
        CHECK((f->drop_x == -1) == (f->drop_y == -1) && f->drop_x >= -1 && f->drop_x < 16 && f->drop_y >= -1 && f->drop_y < 16,
              "%s: dropped cell", f->file);
        if (f->ramps)
            for (j = 0; j < ARCADE_PARTS_RAMPS; j++)
                CHECK(f->ramps[j] == ARCADE_PARTS_NO_RAMP || (f->ramps[j] < 0x1000u && !((f->keep >> (j < 16 ? 2 * j + 33 : j == 16 ? 60 : 62)) & 1)),
                      "%s: ramp %u", f->file, j);
    }
    if (!bad) printf("ok\n");
    return bad ? 1 : 0;
}

int main(int argc, char **argv)
{
    if (argc == 5 && !strcmp(argv[1], "palettes")) return palettes(argv[2], argv[3], argv[4]);
    if (argc == 6 && !strcmp(argv[1], "portrait")) return portrait(argv[2], argv[3], argv[4], argv[5]);
    if (argc == 5 && !strcmp(argv[1], "shared")) return shared(argv[2], argv[3], argv[4]);
    if (argc == 2 && !strcmp(argv[1], "table")) return table();
    fprintf(stderr, "usage: see the comment\n");
    return 3;
}
