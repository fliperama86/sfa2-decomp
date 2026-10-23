#include "arcade.h"
#include "cd.h"
#include "port.h"
#include "../arcade/fighter.h"
#include "../arcade/fighter_parts.h"
#include "../arcade/program.h"
#include "../arcade/rom.h"
#include "../arcade/tilepack.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_FILES 512
#define MAX_RANGES 512

static struct arcade_rom rom;
static int enabled;
static struct port_disc_file files[MAX_FILES];   /* the disc's files, by first sector */
static unsigned char told[MAX_FILES];
static unsigned char built[MAX_FILES];           /* 1: some of the file's bytes are the loader's */
static unsigned file_count;

/* Byte ranges of disc files that the loader built: a sector read of the file gets these bytes. */
struct range {
    unsigned sector;        /* the file's first sector */
    unsigned offset, size;  /* in the file */
    unsigned char *bytes;
};
static struct range ranges[MAX_RANGES];
static unsigned range_count;

/* Files whose built blocks differ from the disc's where the PS1 changed the arcade's data (port/arcade/README.md):
 * palettes (PL02, PL14, PL0B), body-part tile numbering (PL08, PL0E). They stay the disc's until the files that
 * refer to their lists and tiles are built too. */
static const char *const ps1_changed[] = {"PL02.PAC", "PL02X.PAC", "PL08.PAC", "PL08X.PAC", "PL0B.PAC", "PL0BX.PAC",
                                          "PL0E.PAC", "PL0EX.PAC", "PL14.PAC", "PL14X.PAC"};

static unsigned piece_bytes(void)
{
    unsigned i, n = 0;
    for (i = 0; i < arcade_program_piece_count; i++) n += arcade_program_pieces[i].size;
    return n;
}

int port_arcade_configure(int on, const char *path, char *err, size_t size)
{
    if (enabled) arcade_rom_close(&rom);
    enabled = 0;
    if (!on) return 0;
    if (arcade_pieces_valid(arcade_program_pieces, arcade_program_piece_count) != 0) {
        snprintf(err, size, "arcade: the build's table of program pieces is malformed");
        return -1;
    }
    if (arcade_rom_open(path, &rom, err, size) != 0) return -1;
    enabled = 1;
    printf("arcade: %s, the examined sfa2u set: program, graphics, samples and sound program verified\n", path);
    fflush(stdout);
    return 0;
}

int port_arcade_enabled(void)
{
    return enabled;
}

void port_arcade_missing(char *line, size_t size)
{
    snprintf(line, size, "arcade: no arcade source yet for the PS1 program's data (SLPS_004.15): %u of its %u bytes are built from the arcade set, the rest needs the disc",
             piece_bytes(), ARCADE_PS1_DATA_END - ARCADE_PS1_DATA_START);
}

unsigned port_arcade_program(unsigned char *ram, int check)
{
    unsigned i, differ = 0;
    unsigned char *out;
    if (!enabled) return 0;
    out = malloc(ARCADE_PS1_DATA_END - ARCADE_PS1_DATA_START);
    if (!out) port_halt(PORT_EXIT_PROGRAM, "arcade: out of memory");
    for (i = 0; i < arcade_program_piece_count; i++) {
        const struct arcade_piece *p = &arcade_program_pieces[i];
        unsigned char *at = ram + (p->address - PORT_RAM_BASE);
        if (arcade_piece_build(rom.program, ARCADE_PROGRAM_SIZE, p, out) != 0)
            port_halt(PORT_EXIT_PROGRAM, "arcade: the piece %s does not fit the arcade program", p->name);
        if (check) {
            if (memcmp(out, at, p->size) != 0) {
                unsigned k = 0;
                while (out[k] == at[k]) k++;
                printf("arcade check: %s (0x%08x, %u bytes, from 0x%06x): differs first at +0x%x\n", p->name, p->address, p->size, p->source, k);
                differ++;
            }
        } else {
            memcpy(at, out, p->size);
        }
    }
    free(out);
    if (check)
        printf("arcade check: %u pieces, %u bytes of the PS1 program's data: %u differ from the disc's\n", arcade_program_piece_count, piece_bytes(), differ);
    else
        printf("arcade: the PS1 program's data: %u of %u bytes built from the arcade set (%u pieces), the rest from the disc\n",
               piece_bytes(), ARCADE_PS1_DATA_END - ARCADE_PS1_DATA_START, arcade_program_piece_count);
    fflush(stdout);
    return differ;
}

static int by_sector(const void *a, const void *b)
{
    const struct port_disc_file *x = a, *y = b;
    return x->sector < y->sector ? -1 : x->sector > y->sector;
}

/* The file that holds the sector, or -1. */
static int file_of(unsigned sector)
{
    unsigned lo = 0, hi = file_count;
    while (lo < hi) {   /* the last file that starts at or before the sector */
        unsigned mid = (lo + hi) / 2;
        if (files[mid].sector <= sector) lo = mid + 1;
        else hi = mid;
    }
    if (lo == 0) return -1;
    lo--;
    if (sector >= files[lo].sector + (files[lo].size + PORT_DATA - 1u) / PORT_DATA) return -1;
    return (int)lo;
}

/* The built bytes over the n data bytes of `sector`. */
static void patch(unsigned sector, unsigned char *data, unsigned n)
{
    unsigned i;
    for (i = 0; i < range_count; i++) {
        const struct range *r = &ranges[i];
        unsigned long long at, lo, hi;
        if (sector < r->sector) continue;
        at = (unsigned long long)(sector - r->sector) * PORT_DATA;   /* the sector's place in the file */
        lo = at > r->offset ? at : r->offset;
        hi = at + n < (unsigned long long)r->offset + r->size ? at + n : (unsigned long long)r->offset + r->size;
        if (lo < hi) memcpy(data + (lo - at), r->bytes + (lo - r->offset), (size_t)(hi - lo));
    }
}

static void read_hook(unsigned sector, unsigned char *raw)
{
    int f = file_of(sector);
    if (f < 0) return;
    if (built[f] && !(raw[18] & 0x20)) patch(sector, raw + PORT_DATA_OFFSET, PORT_DATA);   /* a data sector (form 1) */
    if (told[f]) return;
    told[f] = 1;
    if (built[f]) printf("arcade: %s: built in part from the arcade set, the rest from the disc (first read at frame %u)\n", files[f].name, port_frames());
    else printf("arcade: %s from the disc: no arcade source yet (first read at frame %u)\n", files[f].name, port_frames());
    fflush(stdout);
}

/* Where the nth chunk (from 0) of `type` (table << 16 | slot) lies in the PAC file f: its offset in the file and its size.
 * -1 if there is none. */
static int pac_chunk_nth(struct port_disc *d, const struct port_disc_file *f, unsigned type, unsigned nth, unsigned *offset, unsigned *size)
{
    unsigned char head[PORT_DATA];
    char err[PORT_ERR];
    unsigned count, i, at = PORT_DATA;
    if (f->size < PORT_DATA || port_disc_read(d, f->sector, PORT_DATA, head, err, sizeof err) != 0) return -1;
    count = head[0] | head[1] << 8 | (unsigned)head[2] << 16 | (unsigned)head[3] << 24;
    if (count > (PORT_DATA - 0x20) / 32) return -1;
    for (i = 0; i < count; i++) {
        const unsigned char *e = head + 0x20 + 32 * i;
        unsigned e_type = e[0] | e[1] << 8 | (unsigned)e[2] << 16 | (unsigned)e[3] << 24;
        unsigned n = e[4] | e[5] << 8 | (unsigned)e[6] << 16 | (unsigned)e[7] << 24;
        if (n > f->size || at > f->size - n) return -1;
        if (e_type == type && nth-- == 0) {
            *offset = at;
            *size = n;
            return 0;
        }
        at += (n + PORT_DATA - 1u) / PORT_DATA * PORT_DATA;
    }
    return -1;
}

/* Table 0's chunk `slot`. */
static int pac_chunk(struct port_disc *d, const struct port_disc_file *f, unsigned slot, unsigned *offset, unsigned *size)
{
    return pac_chunk_nth(d, f, slot, 0, offset, size);
}

static int changed_by_ps1(const char *name)
{
    unsigned i;
    for (i = 0; i < sizeof ps1_changed / sizeof ps1_changed[0]; i++)
        if (!strcmp(ps1_changed[i], name)) return 1;
    return 0;
}

/* Compare `n` built bytes with the disc's at offset `at` of file f. 1 equal, 0 different, -1 unreadable. */
static int same_as_disc(struct port_disc *d, const struct port_disc_file *f, unsigned at, const unsigned char *bytes, unsigned n)
{
    unsigned first = at / PORT_DATA, skip = at % PORT_DATA, span = (skip + n + PORT_DATA - 1u) / PORT_DATA * PORT_DATA;
    unsigned char *disc = malloc(span ? span : 1);
    char err[PORT_ERR];
    int same;
    void (*hook)(unsigned, unsigned char *, unsigned) = port_disc_patch_hook;
    if (!disc) return -1;
    port_disc_patch_hook = NULL;   /* the disc's own bytes */
    same = port_disc_read(d, f->sector + first, span, disc, err, sizeof err) == 0 ? memcmp(disc + skip, bytes, n) == 0 : -1;
    port_disc_patch_hook = hook;
    free(disc);
    return same;
}

static void add_range(int f, unsigned offset, unsigned char *bytes, unsigned size)
{
    if (range_count == MAX_RANGES) port_halt(PORT_EXIT_PROGRAM, "arcade: more built ranges than the loader holds");
    ranges[range_count].sector = files[f].sector;
    ranges[range_count].offset = offset;
    ranges[range_count].size = size;
    ranges[range_count].bytes = bytes;
    range_count++;
    built[f] = 1;
}

/* The PS1 tile that arcade tile `code` stored with `flip` makes (4 bits a pixel, the left pixel of a pair in the low nibble,
 * the color index one higher). -1 if the code is outside the graphics. */
static int arcade_tile(unsigned code, unsigned flip, unsigned char out[128])
{
    const unsigned char *t;
    unsigned x, y;
    if (code >= ARCADE_TILES_SIZE / 128u) return -1;
    t = rom.tiles + (size_t)code * 128u;
    memset(out, 0, 128);
    for (y = 0; y < 16; y++)
        for (x = 0; x < 16; x++) {
            unsigned sx = flip & 1u ? 15u - x : x, sy = flip & 2u ? 15u - y : y, v = 0, plane;
            for (plane = 0; plane < 4; plane++) v |= ((t[sy * 8u + (sx / 8u) * 4u + plane] >> (7u - sx % 8u)) & 1u) << plane;
            out[y * 8u + x / 2u] |= (unsigned char)(((v + 1u) & 15u) << (x & 1u ? 4 : 0));
        }
    return 0;
}

/* Read `size` bytes at offset `at` of file f, the disc's own. NULL on failure. */
static unsigned char *disc_bytes(struct port_disc *d, const struct port_disc_file *f, unsigned at, unsigned size)
{
    unsigned first = at / PORT_DATA, skip = at % PORT_DATA, span = (skip + size + PORT_DATA - 1u) / PORT_DATA * PORT_DATA;
    unsigned char *buf = malloc(span ? span : 1), *out = malloc(size ? size : 1);
    char err[PORT_ERR];
    void (*hook)(unsigned, unsigned char *, unsigned) = port_disc_patch_hook;
    int ok;
    if (!buf || !out) { free(buf); free(out); return NULL; }
    port_disc_patch_hook = NULL;
    ok = port_disc_read(d, f->sector + first, span, buf, err, sizeof err) == 0;
    port_disc_patch_hook = hook;
    if (ok) memcpy(out, buf + skip, size);
    free(buf);
    if (!ok) { free(out); return NULL; }
    return out;
}

/* The fighter's sprite chunk built from the arcade tiles that `fo` numbers, packed no larger than the disc's chunk: the
 * chunk's bytes (the disc's size, the end zero-filled) in *out, or NULL when a numbered tile is not the disc's tile (the
 * PS1's own changes) or it does not pack. The disc gives tile 0 and the PS1's own tiles after the numbered ones. */
static unsigned char *sprite_chunk(struct port_disc *d, const struct port_disc_file *f, const struct arcade_fighter_out *fo,
                                   unsigned at, unsigned size, unsigned *same, unsigned *tiles_out)
{
    unsigned char *disc = disc_bytes(d, f, at, size), *out = NULL;
    unsigned char (*tiles)[128] = NULL;
    uint32_t count, i, packed;
    char err[PORT_ERR];
    *same = 0;
    *tiles_out = 0;
    if (!disc) return NULL;
    count = arcade_tilepack_count(disc, size);
    if (count < fo->tile_count || !(tiles = malloc((size_t)count * 128u))) goto done;
    for (i = 0; i < count; i++)
        if (arcade_tilepack_decode(disc, size, i, tiles[i], err, sizeof err) != 0) goto done;
    for (i = 1; i < fo->tile_count; i++) {
        unsigned char t[128];
        if (arcade_tile(fo->tile_code[i], fo->tile_flip[i], t) != 0) goto done;
        if (memcmp(t, tiles[i], 128) == 0) (*same)++;
        memcpy(tiles[i], t, 128);
    }
    *tiles_out = fo->tile_count - 1u;
    if (*same != fo->tile_count - 1u) goto done;
    out = calloc(size ? size : 1, 1);
    if (out && arcade_tilepack((const uint8_t (*)[128])tiles, count, out, size, &packed, err, sizeof err) != 0) {
        free(out);
        out = NULL;
    }
done:
    free(tiles);
    free(disc);
    return out;
}

/* Build each fighter file's position runs and cell lists. Built blocks equal to the disc's are served in place of it;
 * others stay the disc's. With check, only compare, and count the files that differ but for the PS1's known changes. */
static unsigned fighters(struct port_disc *d, int check)
{
    unsigned i, used = 0, kept = 0, bytes = 0, wrong = 0, tiles_used = 0, tile_bytes = 0, tile_wrong = 0;
    for (i = 0; i < arcade_fighter_count; i++) {
        const struct arcade_fighter *a = &arcade_fighters[i];
        struct arcade_fighter_out out;
        unsigned sector, size, moff, msize, poff, psize, slot = a->positions_slot == ARCADE_FIGHTER_SAME_CHUNK ? a->module_slot : a->positions_slot;
        int f = -1, same_lists, same_positions;
        char err[PORT_ERR];
        unsigned k;
        if (port_disc_find(d, a->file, &sector, &size, err, sizeof err) != 0) port_halt(PORT_EXIT_PROGRAM, "arcade: %s: %s", a->file, err);
        for (k = 0; k < file_count; k++)
            if (files[k].sector == sector) f = (int)k;
        if (f < 0 || pac_chunk(d, &files[f], a->module_slot, &moff, &msize) != 0 || pac_chunk(d, &files[f], slot, &poff, &psize) != 0 ||
            a->lists > msize || a->lists_room > msize - a->lists || a->positions > psize || a->positions_room > psize - a->positions)
            port_halt(PORT_EXIT_PROGRAM, "arcade: %s: the file is not laid out as the loader's table says", a->file);
        if (arcade_fighter_build(rom.program, ARCADE_PROGRAM_SIZE, a, &out, err, sizeof err) != 0)
            port_halt(PORT_EXIT_PROGRAM, "arcade: %s: %s", a->file, err);
        same_lists = out.lists_size == a->lists_room ? same_as_disc(d, &files[f], moff + a->lists, out.lists, out.lists_size) : 0;
        same_positions = out.positions_size == a->positions_room ? same_as_disc(d, &files[f], poff + a->positions, out.positions, out.positions_size) : 0;
        if (same_lists < 0 || same_positions < 0) port_halt(PORT_EXIT_PROGRAM, "arcade: %s: cannot read the disc's blocks", a->file);
        if (same_lists && same_positions) {
            unsigned soff, ssize, same_tiles = 0, numbered = 0;
            unsigned char *sprites = NULL;
            used++;
            bytes += out.lists_size + out.positions_size;
            if (pac_chunk(d, &files[f], 1, &soff, &ssize) == 0 || pac_chunk(d, &files[f], 2, &soff, &ssize) == 0)
                sprites = sprite_chunk(d, &files[f], &out, soff, ssize, &same_tiles, &numbered);
            if (sprites) {
                tiles_used++;
                tile_bytes += ssize;
            }
            if (check && !sprites) {
                printf("arcade check: %s: sprite tiles: %u of %u numbered tiles the disc's%s\n", a->file, same_tiles, numbered,
                       same_tiles == numbered ? ", but the chunk does not pack in the disc's size" : "");
                tile_wrong++;
            }
            if (!check) {
                add_range(f, moff + a->lists, out.lists, out.lists_size);
                add_range(f, poff + a->positions, out.positions, out.positions_size);
                if (sprites) add_range(f, soff, sprites, ssize);
                out.lists = out.positions = NULL;   /* the ranges own them now */
            } else {
                free(sprites);
            }
        } else {
            kept++;
            if (check) printf("arcade check: %s: lists %s, positions %s%s\n", a->file, same_lists ? "same" : "differ", same_positions ? "same" : "differ",
                              changed_by_ps1(a->file) ? " (the PS1's own changes, known)" : "");
            if (!changed_by_ps1(a->file)) wrong++;
        }
        arcade_fighter_free(&out);
    }
    printf("arcade%s: fighters: %u of %u files' cell lists and positions built from the arcade set (%u bytes)%s; %u differ from the disc's, %u of them not by the PS1's known changes\n",
           check ? " check" : "", used, arcade_fighter_count, bytes, check ? " are equal to the disc's" : " and used", kept, wrong);
    printf("arcade%s: fighters: %u files' sprite tiles built from the arcade graphics and packed (%u bytes), %s\n", check ? " check" : "", tiles_used, tile_bytes,
           check ? "each tile the disc's" : "used");
    fflush(stdout);
    return wrong + tile_wrong;
}

/* The index of the disc's file `name`; halts if there is none. */
static int file_index(struct port_disc *d, const char *name)
{
    unsigned sector, size, k;
    char err[PORT_ERR];
    if (port_disc_find(d, name, &sector, &size, err, sizeof err) != 0) port_halt(PORT_EXIT_PROGRAM, "arcade: %s: %s", name, err);
    for (k = 0; k < file_count; k++)
        if (files[k].sector == sector) return (int)k;
    port_halt(PORT_EXIT_PROGRAM, "arcade: %s is not in the disc's listing", name);
    return -1;
}

/* Built bytes for offset `at` of file f: served in place of the disc's when equal to them (bytes then belong to the
 * ranges), else freed. 1 equal, 0 not. With check only compared. */
static int serve_if_same(struct port_disc *d, int f, unsigned at, unsigned char *bytes, unsigned n, int check)
{
    int same = same_as_disc(d, &files[f], at, bytes, n);
    if (same < 0) port_halt(PORT_EXIT_PROGRAM, "arcade: %s: cannot read the disc's bytes", files[f].name);
    if (same && !check) add_range(f, at, bytes, n);
    else free(bytes);
    return same;
}

/* The fighter files' palettes (table 1) and pictures (table 2). The PS1's own palettes and page stay the disc's. */
static unsigned parts(struct port_disc *d, int check)
{
    unsigned i, pal_used = 0, pal_bytes = 0, page_used = 0, page_bytes = 0, differ = 0;
    for (i = 0; i < arcade_parts_count; i++) {
        const struct arcade_parts_file *a = &arcade_parts_files[i];
        int f = file_index(d, a->file);
        unsigned c, at, n, k;
        char err[PORT_ERR];
        for (c = 0; c < a->palette_chunks; c++) {
            unsigned char *b = malloc(ARCADE_PARTS_PALETTE_BYTES), *disc;
            uint64_t kept = 0;
            if (!b || pac_chunk_nth(d, &files[f], 0x10000u, c, &at, &n) != 0 || n != ARCADE_PARTS_PALETTE_BYTES)
                port_halt(PORT_EXIT_PROGRAM, "arcade: %s: no palette chunk %u as the table says", a->file, c);
            if (arcade_parts_palettes(rom.program, ARCADE_PROGRAM_SIZE, a, b, &kept, err, sizeof err) != 0) port_halt(PORT_EXIT_PROGRAM, "arcade: %s: %s", a->file, err);
            if (kept) {   /* the PS1's own colors: the disc's */
                if (!(disc = disc_bytes(d, &files[f], at, n))) port_halt(PORT_EXIT_PROGRAM, "arcade: %s: cannot read the disc's bytes", a->file);
                for (k = 0; k < 64; k++)
                    if (kept >> k & 1u) memcpy(b + 32u * k, disc + 32u * k, 32);
                free(disc);
            }
            if (serve_if_same(d, f, at, b, n, check)) { pal_used++; pal_bytes += n; }
            else { differ++; if (check) printf("arcade check: %s: palette chunk %u differs\n", a->file, c); }
        }
        for (c = 0; a->pages[c]; c++) {
            unsigned char *b = malloc(ARCADE_PARTS_PAGE_BYTES);
            int r;
            if (!b || pac_chunk_nth(d, &files[f], 0x20000u, c, &at, &n) != 0 || n != ARCADE_PARTS_PAGE_BYTES)
                port_halt(PORT_EXIT_PROGRAM, "arcade: %s: no picture chunk %u as the table says", a->file, c);
            r = a->pages[c] == 'V' ? arcade_parts_shared(rom.program, ARCADE_PROGRAM_SIZE, rom.tiles, ARCADE_TILES_SIZE, b, err, sizeof err)
                                   : arcade_parts_portrait(rom.program, ARCADE_PROGRAM_SIZE, rom.tiles, ARCADE_TILES_SIZE, a, b, err, sizeof err);
            if (r < 0) port_halt(PORT_EXIT_PROGRAM, "arcade: %s: %s", a->file, err);
            if (r == 1) { free(b); continue; }   /* the PS1's own page */
            if (serve_if_same(d, f, at, b, n, check)) { page_used++; page_bytes += n; }
            else { differ++; if (check) printf("arcade check: %s: picture chunk %u differs\n", a->file, c); }
        }
    }
    printf("arcade%s: fighters: %u palette chunks (%u bytes) and %u picture pages (%u bytes) built from the arcade set%s; %u differ\n",
           check ? " check" : "", pal_used, pal_bytes, page_used, page_bytes, check ? ", equal to the disc's" : " and used", differ);
    fflush(stdout);
    return differ;
}

int port_arcade_attach(struct port_disc *d, int check, char *err, size_t size)
{
    unsigned wrong;
    if (!enabled) return 0;
    if (port_disc_list(d, files, MAX_FILES, &file_count, err, size) != 0) return -1;
    qsort(files, file_count, sizeof files[0], by_sector);
    memset(told, 0, sizeof told);
    memset(built, 0, sizeof built);
    wrong = fighters(d, check);
    wrong += parts(d, check);
    if (check) return wrong ? 1 : 0;
    port_disc_patch_hook = patch;
    port_cd_read_hook = read_hook;
    return 0;
}
