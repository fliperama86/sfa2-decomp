#include "arcade.h"
#include "cd.h"
#include "port.h"
#include "../arcade/fighter.h"
#include "../arcade/fighter_parts.h"
#include "../arcade/sequence.h"
#include "../arcade/stage.h"
#include "../arcade/screens.h"
#include "../arcade/music.h"
#include "../arcade/module_data.h"
#include "../arcade/screen_lists.h"
#include "../arcade/sounds.h"
#include "settings.h"
#include "port_tables.h"
#include <stdint.h>
#include "../arcade/program.h"
#include "../arcade/program_more.h"
#include "../arcade/rom.h"
#include "../arcade/tilepack.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_FILES 512

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
    int equal;              /* 1: equal to the disc's bytes (a pack leaves them out); 0: decoding to the same (a pack keeps the disc's) */
};
static struct port_disc *pack_disc;   /* the disc when it is a pack: what to serve is what was served when the pack was made */
static struct range *ranges;   /* grown as needed */
static unsigned range_count, range_room;
static unsigned file_first[MAX_FILES], file_ranges[MAX_FILES];   /* each file's ranges, after they are sorted */
static unsigned long long built_bytes[MAX_FILES];                /* bytes of each file that the loader builds equal to the disc's */

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
    struct arcade_effects_out e;
    unsigned i, n = piece_bytes() + 4u * arcade_reloc_count;
    char err[PORT_ERR];
    if (arcade_effects_build(rom.program, ARCADE_PROGRAM_SIZE, &arcade_effects, &e, err, sizeof err) == 0) {
        for (i = 0; i < e.size; i++) n += e.known[i];
        arcade_effects_free(&e);
    }
    snprintf(line, size, "arcade: no arcade source yet for all of the PS1 program's data (SLPS_004.15): %u of its %u bytes are built from the arcade set, the rest needs the disc",
             n, ARCADE_PS1_DATA_END - ARCADE_PS1_DATA_START);
}

/* The program's pointer words (the build's layout) and the effects' position runs and cell lists. Returns the number of
 * words and bytes that differ from the disc's (with check). */
static unsigned program_more(unsigned char *ram, int check)
{
    const unsigned span = ARCADE_PS1_DATA_END - ARCADE_PS1_DATA_START;
    unsigned char *data = ram + (ARCADE_PS1_DATA_START - PORT_RAM_BASE), *words;
    struct arcade_effects_out e;
    unsigned i, differ = 0, known = 0;
    char err[PORT_ERR];
    if (arcade_relocs_valid(arcade_relocs, arcade_reloc_count) != 0) port_halt(PORT_EXIT_PROGRAM, "arcade: the build's table of pointer words is malformed");
    if (!(words = calloc(span, 1))) port_halt(PORT_EXIT_PROGRAM, "arcade: out of memory");
    if (arcade_relocs_apply(arcade_relocs, arcade_reloc_count, words, span) != 0) port_halt(PORT_EXIT_PROGRAM, "arcade: a pointer word lies outside the program's data");
    for (i = 0; i < arcade_reloc_count; i++) {
        unsigned at = arcade_relocs[i].location - ARCADE_PS1_DATA_START;
        if (check) differ += memcmp(words + at, data + at, 4) != 0;
        else memcpy(data + at, words + at, 4);
    }
    free(words);
    if (arcade_effects_build(rom.program, ARCADE_PROGRAM_SIZE, &arcade_effects, &e, err, sizeof err) != 0) port_halt(PORT_EXIT_PROGRAM, "arcade: effects: %s", err);
    for (i = 0; i < e.size; i++) {
        unsigned char *at = ram + (arcade_effects.address - PORT_RAM_BASE) + i;
        if (!e.known[i]) continue;
        known++;
        if (check) differ += *at != e.data[i];
        else *at = e.data[i];
    }
    arcade_effects_free(&e);
    printf("arcade%s: the PS1 program's data: %u pointer words (the port's layout) and %u bytes of effect cell lists and positions built from the arcade set%s\n",
           check ? " check" : "", arcade_reloc_count, known, check ? "" : ", used");
    return differ;
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
    differ += program_more(ram, check);
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
static int file_of(unsigned sector);
static void patch(unsigned sector, unsigned char *data, unsigned n)
{
    unsigned i, end;
    int f = file_of(sector);
    if (f < 0 || !built[f]) return;
    end = file_first[f] + file_ranges[f];
    for (i = file_first[f]; i < end; i++) {
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
    if (built[f]) printf("arcade: %s: built in part from the arcade set, the rest from the %s (first read at frame %u)\n", files[f].name, pack_disc ? "pack" : "disc", port_frames());
    else printf("arcade: %s from the %s: no arcade source yet (first read at frame %u)\n", files[f].name, pack_disc ? "pack" : "disc", port_frames());
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
/* With a pack: whether [at, at + n) of the file lies in what was built equal to the disc's when the pack was made. */
static int recorded(const struct port_disc_file *f, unsigned at, unsigned n)
{
    const unsigned (*r)[2];
    const unsigned char *digest;
    unsigned count, lo = 0, hi;
    if (port_disc_pack_built(pack_disc, f->sector, &r, &count, &digest) != 0) return 0;
    hi = count;
    while (lo < hi) {   /* the last range that starts at or before `at` */
        unsigned mid = (lo + hi) / 2;
        if (r[mid][0] <= at) lo = mid + 1;
        else hi = mid;
    }
    return lo > 0 && (unsigned long long)at + n <= (unsigned long long)r[lo - 1][0] + r[lo - 1][1];
}

static int same_as_disc(struct port_disc *d, const struct port_disc_file *f, unsigned at, const unsigned char *bytes, unsigned n)
{
    if (pack_disc) return recorded(f, at, n);
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

static void add_range_kind(int f, unsigned offset, unsigned char *bytes, unsigned size, int equal);
static void add_range(int f, unsigned offset, unsigned char *bytes, unsigned size)
{
    add_range_kind(f, offset, bytes, size, 1);
}

static void add_range_kind(int f, unsigned offset, unsigned char *bytes, unsigned size, int equal)
{
    if (range_count == range_room) {
        unsigned room = range_room ? range_room * 2u : 4096u;
        struct range *more = realloc(ranges, (size_t)room * sizeof *more);
        if (!more || room < range_room) port_halt(PORT_EXIT_PROGRAM, "arcade: out of memory for the built ranges");
        ranges = more;
        range_room = room;
    }
    ranges[range_count].sector = files[f].sector;
    ranges[range_count].offset = offset;
    ranges[range_count].size = size;
    ranges[range_count].bytes = bytes;
    ranges[range_count].equal = equal;
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

struct step_stats { unsigned steps, frames, entries, other; };

/* Serve the runs of `count` units of `unit` bytes at file offset `at` that are built and equal to the disc's. The ranges
 * point into `bytes`, which is not freed. Returns the units served. */
static unsigned serve_units(struct port_disc *d, int f, unsigned at, unsigned char *bytes, const unsigned char *built_unit,
                            unsigned count, unsigned unit, int check)
{
    unsigned char *disc = disc_bytes(d, &files[f], at, count * unit);
    unsigned i = 0, served = 0;
    if (!disc) port_halt(PORT_EXIT_PROGRAM, "arcade: %s: cannot read the disc's bytes", files[f].name);
    while (i < count) {
        unsigned j = i;
        while (j < count && built_unit[j] && (pack_disc ? recorded(&files[f], at + j * unit, unit) : !memcmp(bytes + j * unit, disc + j * unit, unit))) j++;
        if (j > i) {
            if (!check) add_range(f, at + i * unit, bytes + i * unit, (j - i) * unit);
            built_bytes[f] += (unsigned long long)(j - i) * unit;
            served += j - i;
            i = j;
        } else {
            i++;
        }
    }
    free(disc);
    return served;
}

/* The fighter's animation steps, frame records and step tables (sequence.c), served where built and equal to the disc's. */
static void steps_of(struct port_disc *d, int f, const struct arcade_fighter *a, const struct arcade_fighter_out *fo, unsigned moff,
                     int check, struct step_stats *st)
{
    const struct arcade_sequence *s = arcade_sequence_find(a->file);
    struct arcade_sequence_lists lists;
    struct arcade_sequence_out so;
    char err[PORT_ERR];
    unsigned n;
    if (!s) return;
    if (arcade_sequence_lists(rom.program, ARCADE_PROGRAM_SIZE, a, fo, &lists, err, sizeof err) != 0 ||
        arcade_sequence_build(rom.program, ARCADE_PROGRAM_SIZE, s, &lists, &so, err, sizeof err) != 0)
        port_halt(PORT_EXIT_PROGRAM, "arcade: %s: %s", a->file, err);
    arcade_sequence_lists_free(&lists);
    n = serve_units(d, f, moff + s->steps, so.steps, so.steps_built, so.step_count, 12, check);
    st->steps += n;
    st->other += so.step_count - n;
    st->frames += serve_units(d, f, moff + s->frames, so.frames, so.frames_built, so.frame_count, 16, check);
    st->entries += serve_units(d, f, moff + s->steps + 12u * so.step_count, so.table, so.table_built, so.table_entries, 4, check);
    if (check) arcade_sequence_free(&so);   /* else the ranges point into it */
}

/* Build each fighter file's position runs and cell lists. Built blocks equal to the disc's are served in place of it;
 * others stay the disc's. With check, only compare, and count the files that differ but for the PS1's known changes. */
static unsigned fighters(struct port_disc *d, int check)
{
    unsigned i, used = 0, kept = 0, bytes = 0, wrong = 0, tiles_used = 0, tile_bytes = 0, tile_wrong = 0;
    struct step_stats st = {0, 0, 0, 0};
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
        steps_of(d, f, a, &out, moff, check, &st);   /* before the lists may pass to the ranges */
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
            built_bytes[f] += out.lists_size + out.positions_size + (sprites ? ssize : 0u);
            if (!check) {
                add_range(f, moff + a->lists, out.lists, out.lists_size);
                add_range(f, poff + a->positions, out.positions, out.positions_size);
                if (sprites) add_range_kind(f, soff, sprites, ssize, 0);
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
    printf("arcade%s: fighters: animation: %u steps, %u frame records and %u table entries built from the arcade set and equal to the disc's%s; %u steps the PS1's own or changed stay the disc's\n",
           check ? " check" : "", st.steps, st.frames, st.entries, check ? "" : ", used", st.other);
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
    if (same) built_bytes[f] += n;
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
            {   /* palette by palette: the PS1's own colors (kept) stay the disc's */
                unsigned char mask[64];
                unsigned got;
                (void)disc;
                for (k = 0; k < 64; k++) mask[k] = !(kept >> k & 1u);
                got = serve_units(d, f, at, b, mask, 64, 32, check);
                for (k = 0; k < 64; k++) got += !mask[k];
                if (got == 64) { pal_used++; pal_bytes += n; }
                else { differ++; if (check) printf("arcade check: %s: palette chunk %u differs\n", a->file, c); }
                if (check) free(b);   /* else the ranges point into it */
            }
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

/* A stage piece served where known and equal to the disc's: chunk `type`, nth 0, of file f. Returns the bytes served. */
static unsigned stage_piece(struct port_disc *d, int f, unsigned type, unsigned nth, struct arcade_stage_piece *pc, int check)
{
    unsigned at, n;
    if (!pc->data || !pc->size) return 0;
    if (pac_chunk_nth(d, &files[f], type, nth, &at, &n) != 0 || pc->size > n)
        port_halt(PORT_EXIT_PROGRAM, "arcade: %s: no chunk 0x%x as the stage table says", files[f].name, type);
    return serve_units(d, f, at, pc->data, pc->known, pc->size, 1, check);
}

/* The stage files: layer maps (in the stage module), texture pages, layer palettes, object tiles and palettes, and the
 * shared effect tiles, each served where the build knows it and it equals the disc's. */
static void stages(struct port_disc *d, int check)
{
    unsigned i, served = 0, total = 0;
    struct arcade_stage_piece shared;
    char err[PORT_ERR];
    if (arcade_stage_build_shared(rom.tiles, ARCADE_TILES_SIZE, &shared, err, sizeof err) != 0) port_halt(PORT_EXIT_PROGRAM, "arcade: stages: %s", err);
    for (i = 0; i < arcade_stage_count; i++) {
        const struct arcade_stage *a = &arcade_stages[i];
        struct arcade_stage_out o;
        int f = file_index(d, a->file);
        if (arcade_stage_build(rom.program, ARCADE_PROGRAM_SIZE, rom.tiles, ARCADE_TILES_SIZE, a, &o, err, sizeof err) != 0)
            port_halt(PORT_EXIT_PROGRAM, "arcade: %s: %s", a->file, err);
        total += o.module.size + o.pages.size + o.palettes.size + o.object_tiles.size + o.object_palettes.size + shared.size;
        served += stage_piece(d, f, 6u, 0, &o.module, check);            /* the module: its layer maps */
        served += stage_piece(d, f, 0x20000u, 0, &o.pages, check);       /* table 2: the texture pages */
        served += stage_piece(d, f, 0x10000u, 0, &o.palettes, check);    /* table 1, first: the layer palettes */
        served += stage_piece(d, f, 9u, 0, &o.object_tiles, check);
        served += stage_piece(d, f, 0xau, 0, &o.object_palettes, check);
        served += stage_piece(d, f, 7u, 0, &shared, check);              /* the shared effect tiles */
        if (check) arcade_stage_free(&o);   /* else the ranges point into it */
    }
    printf("arcade%s: stages: %u of %u bytes of what the loader builds of the %u stage files from the arcade set are equal to the disc's%s\n",
           check ? " check" : "", served, total, arcade_stage_count, check ? "" : " and used");
    fflush(stdout);
}

/* How much of the files the game reads the loader builds, by kind of file (the name up to its digits), and the streams
 * (music, movies) apart. */
static void coverage(int check)
{
    struct kind { char name[16]; unsigned long long built, size; unsigned files; } kinds[48];
    unsigned i, nk = 0, k;
    unsigned long long built = 0, size = 0, streams = 0;
    for (i = 0; i < file_count; i++) {
        const char *n = files[i].name, *dot = strrchr(n, '.');
        char name[16];
        size_t len = 0;
        if (dot && (!strcmp(dot, ".xa") || !strcmp(dot, ".str"))) { streams += files[i].size; continue; }
        if (dot && (!strcmp(dot, ".cnf") || !strcmp(dot, ".inf") || !strcmp(dot, ".dat"))) continue;   /* not game data */
        if (!strncmp(n, "slps_", 5)) continue;   /* the program: its data is counted by the pieces, its code is the port's C */
        while (n[len] && !(n[len] >= '0' && n[len] <= '9') && n[len] != '.' && len + 1 < sizeof name) { name[len] = n[len]; len++; }
        name[len] = 0;
        for (k = 0; k < nk && strcmp(kinds[k].name, name); k++) {}
        if (k == nk) {
            if (nk == sizeof kinds / sizeof kinds[0]) continue;
            memcpy(kinds[nk].name, name, len + 1);
            kinds[nk].built = kinds[nk].size = 0;
            kinds[nk].files = 0;
            nk++;
        }
        kinds[k].built += built_bytes[i];
        kinds[k].size += files[i].size;
        kinds[k].files++;
        built += built_bytes[i];
        size += files[i].size;
    }
    printf("arcade%s: coverage: %llu of %llu bytes (%llu%%) of the disc's files other than the program and the streams are built from the arcade set (the modules' code, which the port's C replaces, counts as not built); the streams (%llu MB of recorded music and movies) are not\n",
           check ? " check" : "", built, size, size ? built * 100 / size : 0, streams >> 20);
    for (k = 0; k < nk; k++)
        if (kinds[k].size >= 65536)
            printf("arcade%s: coverage: %-8s %3u files, %3llu%% built\n", check ? " check" : "", kinds[k].name, kinds[k].files, kinds[k].built * 100 / kinds[k].size);
    fflush(stdout);
}

/* The other screens' files (select, continue, endings, demos, boss, HUD): their tiles, palettes and pictures by the recipes of
 * screens_table.c. Bytes equal to the disc's are served, and packed tile segments whose tiles decode to the disc's (checked).
 * The coverage counts what is built from the arcade alone (a second build without the disc's chunk). */
static unsigned screens(struct port_disc *d, int check)
{
    unsigned i, u, chunks = 0, wrong = 0;
    unsigned long long served = 0, arcade_only = 0;
    for (i = 0; i < arcade_screens_file_count; i++) {
        const struct arcade_screens_file *sf = &arcade_screens_files[i];
        int f = file_index(d, sf->file);
        for (u = sf->first; u < (unsigned)sf->first + sf->count; u++) {
            const struct arcade_screens_use *us = &arcade_screens_uses[u];
            const struct arcade_screens_recipe *r = &arcade_screens_recipes[us->recipe];
            struct arcade_screens_out o, alone;
            unsigned at, n, k;
            unsigned char *disc;
            char err[PORT_ERR];
            if (pac_chunk_nth(d, &files[f], (unsigned)us->table << 16 | us->slot, us->ordinal, &at, &n) != 0 || n != r->size)
                port_halt(PORT_EXIT_PROGRAM, "arcade: %s: no chunk %u/0x%x as the screens table says", sf->file, us->table, us->slot);
            if (!(disc = disc_bytes(d, &files[f], at, n))) port_halt(PORT_EXIT_PROGRAM, "arcade: %s: cannot read the disc's bytes", sf->file);
            if (arcade_screens_build(rom.program, ARCADE_PROGRAM_SIZE, rom.tiles, ARCADE_TILES_SIZE, r, disc, n, &o, err, sizeof err) != 0)
                port_halt(PORT_EXIT_PROGRAM, "arcade: %s: %s", sf->file, err);
            if (check) {   /* the coverage: what the arcade builds alone */
                unsigned long long a = 0;
                if (arcade_screens_build(rom.program, ARCADE_PROGRAM_SIZE, rom.tiles, ARCADE_TILES_SIZE, r, NULL, 0, &alone, err, sizeof err) != 0)
                    port_halt(PORT_EXIT_PROGRAM, "arcade: %s: %s", sf->file, err);
                for (k = 0; k < alone.size; k++) a += alone.known[k] != 0;
                free(alone.data); free(alone.known);
                arcade_only += a;
                built_bytes[f] += a;
            }
            chunks++;
            /* With a pack its chunk lacks the bytes built equal to the disc's: the pack's digests check them instead. */
            if (!pack_disc && arcade_screens_verify(r, &o, disc, n, err, sizeof err) != 0) {
                wrong++;
                if (check) printf("arcade check: %s: chunk %u/0x%x: %s\n", sf->file, us->table, us->slot, err);
            } else {
                for (k = 0; k < o.size;) {   /* runs of one kind of known byte */
                    unsigned e = k;
                    while (e < o.size && o.known[e] == o.known[k]) e++;
                    if (o.known[k] == 2) {
                        if (!check) add_range_kind(f, at + k, o.data + k, e - k, 0);
                        served += e - k;
                    } else if (o.known[k] == 1) {
                        unsigned long long before = built_bytes[f];
                        served += serve_units(d, f, at + k, o.data + k, o.known + k, e - k, 1, check);
                        built_bytes[f] = before;   /* counted by the build of the arcade alone */
                    }
                    k = e;
                }
            }
            free(disc);
            if (check) { free(o.data); free(o.known); }   /* else the ranges point into it */
        }
    }
    if (check)
        printf("arcade check: screens: %u chunks of %u files: %llu bytes built, equal to the disc's or decoding to its tiles (%llu of them from the arcade alone, the rest with PS1 tiles in packed segments); %u chunks differ\n",
               chunks, arcade_screens_file_count, served, arcade_only, wrong);
    else
        printf("arcade: screens: %u chunks of %u files: %llu bytes built and used\n", chunks, arcade_screens_file_count, served);
    fflush(stdout);
    return wrong;
}

/* The rest of the fighter files' code chunks' data: tables equal to the arcade's, converted records and pointer words
 * (module_data.c), served where equal to the disc's. */
static void module_data(struct port_disc *d, int check)
{
    unsigned i, at, n;
    unsigned long long served = 0;
    for (i = 0; i < arcade_module_data_count; i++) {
        const struct arcade_module_data *m = &arcade_module_data[i];
        int f = file_index(d, m->file);
        unsigned char *bytes, *known;
        if (pac_chunk(d, &files[f], m->slot, &at, &n) != 0 || n != m->size)
            port_halt(PORT_EXIT_PROGRAM, "arcade: %s: no chunk of slot 0x%x as the module table says", m->file, m->slot);
        if (!(bytes = calloc(n, 1)) || !(known = calloc(n, 1))) port_halt(PORT_EXIT_PROGRAM, "arcade: out of memory");
        if (arcade_module_data_build(rom.program, ARCADE_PROGRAM_SIZE, m, bytes, known, n) != 0)
            port_halt(PORT_EXIT_PROGRAM, "arcade: %s: a piece of the module's data does not build", m->file);
        served += serve_units(d, f, at, bytes, known, n, 1, check);
        free(known);
        if (check) free(bytes);   /* else the ranges point into it */
    }
    printf("arcade%s: fighters: %llu bytes of the modules' other data (tables, records, pointer words) built from the arcade set and %s\n",
           check ? " check" : "", served, check ? "equal to the disc's" : "used");
    fflush(stdout);
}

/* The screen modules' cell lists and position runs (screen_lists.c), served where built and equal to the disc's. */
static void screen_lists(struct port_disc *d, int check)
{
    unsigned i, at, n;
    unsigned long long served = 0;
    char err[PORT_ERR];
    for (i = 0; i < arcade_screen_lists_use_count; i++) {
        const struct arcade_screen_lists_use *u = &arcade_screen_lists_uses[i];
        struct arcade_screen_lists_out o;
        int f = file_index(d, u->file);
        if (pac_chunk(d, &files[f], u->module_slot, &at, &n) != 0) port_halt(PORT_EXIT_PROGRAM, "arcade: %s: no module chunk of slot 0x%x", u->file, u->module_slot);
        if (arcade_screen_lists_build(rom.program, ARCADE_PROGRAM_SIZE, rom.tiles, ARCADE_TILES_SIZE, u, &o, err, sizeof err) != 0)
            port_halt(PORT_EXIT_PROGRAM, "arcade: %s: %s", u->file, err);
        if (u->positions > n || o.positions_size > n - u->positions || u->lists > n || o.lists_size > n - u->lists)
            port_halt(PORT_EXIT_PROGRAM, "arcade: %s: a chain does not fit its module chunk", u->file);
        served += serve_units(d, f, at + u->positions, o.positions, o.positions_known, o.positions_size, 1, check);
        served += serve_units(d, f, at + u->lists, o.lists, o.lists_known, o.lists_size, 1, check);
        if (check) arcade_screen_lists_free(&o);   /* else the ranges point into it */
    }
    printf("arcade%s: screens: %llu bytes of the modules' cell lists and position runs built from the arcade set and %s\n",
           check ? " check" : "", served, check ? "equal to the disc's" : "used");
    fflush(stdout);
}

/* ---- music: the arcade's songs in place of the disc's recorded streams (port/arcade/music.md, section 11) */

/* The PS1's music code (file * 256 + channel of ZERO2_XA/ZERO20N.XA) and the arcade song that is the same music: the fighters'
 * themes by the two games' tables of the same index, the others by measurement where it was decisive. Codes not here keep the
 * recorded stream. */
static const struct { unsigned short code, song; } songs[] = {
    {0x000, 1}, {0x001, 2}, {0x002, 11}, {0x003, 9}, {0x004, 3}, {0x005, 6}, {0x006, 5}, {0x007, 8},
    {0x100, 7}, {0x101, 10}, {0x102, 12}, {0x103, 4}, {0x104, 13}, {0x105, 18}, {0x106, 17}, {0x107, 14},
    {0x200, 15}, {0x201, 16}, {0x202, 53},
    {0x600, 28}, {0x601, 29}, {0x300, 30}, {0x304, 31}, {0x404, 32}, {0x405, 37}, {0x403, 39}, {0x206, 40},
    {0x301, 41}, {0x302, 42}, {0x401, 43}, {0x506, 45}, {0x507, 46}, {0x303, 48}, {0x502, 54}, {0x402, 55},
};
static struct arcade_music *player;
static int playing = -1;   /* the song in place of the stream, or -1: the stream's own sound (under the audio lock) */

static void stream_hook(unsigned sector, unsigned channel)
{
    int f = file_of(sector), song = -1;
    unsigned i;
    const char *n;
    if (f >= 0 && !strncmp(n = files[f].name, "zero20", 6) && n[6] >= '0' && n[6] <= '7' && channel < 8) {
        unsigned code = (unsigned)(n[6] - '0') * 256u + channel;
        for (i = 0; i < sizeof songs / sizeof songs[0]; i++)
            if (songs[i].code == code) song = songs[i].song;
        port_trace_line("arcade: stream %s channel %u (music code 0x%03x): %s %d", n, channel, code, song < 0 ? "the disc's recording" : "arcade song", song);
    }
    port_audio_lock();
    if (song != playing) {
        if (song >= 0) arcade_music_command(player, (unsigned)song);
        else arcade_music_stop(player);
        playing = song;
    }
    port_audio_unlock();
}

/* While an arcade song stands for the stream, the stream is consumed in silence: the player sounds through the mix (below). */
static size_t music_replace(short *out, size_t frames)
{
    if (playing < 0) return 0;
    memset(out, 0, frames * 2 * sizeof *out);
    return frames;
}

/* The arcade player's sound (songs and effects) added to each block of the output, under the audio lock. */
static void arcade_mix(short *out, int frames)
{
    static int16_t buf[2 * 2048];
    int done = 0, i;
    while (done < frames) {
        int n = frames - done > 2048 ? 2048 : frames - done;
        if (arcade_music_render(player, buf, (size_t)n) != n) return;
        for (i = 0; i < 2 * n; i++) {
            int v = out[2 * done + i] + buf[i];
            out[2 * done + i] = (short)(v < -32768 ? -32768 : v > 32767 ? 32767 : v);
        }
        done += n;
    }
}

/* ---- sound effects: the game's sound requests as the arcade's effects (music.md section 12b), by the owner's word: "Just use
 * the arcade. It should sound as good as the arcade version". A request whose sound the arcade has plays as its effect; the
 * others play as on the PS1. */
#define CHUNK_TABLES 0x8017eb1cu    /* data_8017eb1c: the chunk loader's six tables of destinations; table 3 slot s: bank slot s's VAB header */
#define COMMAND_LISTS 0x8016e6e0u   /* table_8016e6e0[slot]: the bank slot's command list (8-byte entries) */
#define CAMERA_X 0x801aa5e6u        /* box_margin[0] */
#define OBJECT_X 0x12u              /* Object.pos_x */
static unsigned sounds_arcade, sounds_ps1;

/* The disc's VAB headers (the chunks of table 3, slots 0 to 3, of every archive), each with its key (the table's key_vab). The
 * game's sound library writes into a header once it has opened it, so a slot's header in memory is known by the disc header it is
 * most like (sound_bank), not by its own key. */
static uint32_t fnv(const unsigned char *p, size_t n, uint32_t h);
static const unsigned char *game(unsigned address, size_t n);
struct bank { uint32_t key; unsigned size; unsigned char *bytes; };
static struct bank *banks;
static unsigned bank_count;
static struct { unsigned address, programs; unsigned char head[32]; uint32_t key; int known; } slot_bank[4];

static void read_banks(struct port_disc *d)
{
    unsigned i, slot, nth, at, n, room = 0;
    char err[PORT_ERR];
    for (i = 0; i < file_count; i++) {
        if (!strstr(files[i].name, ".pac")) continue;
        for (slot = 0; slot < 4; slot++)
            for (nth = 0; pac_chunk_nth(d, &files[i], 0x30000u | slot, nth, &at, &n) == 0; nth++) {
                unsigned char *b;
                if (n < 32 || n > 0x10000u || !(b = malloc(n))) continue;
                if (port_disc_read(d, files[i].sector + at / PORT_DATA, n, b, err, sizeof err) != 0 || memcmp(b, "pBAV", 4) != 0) { free(b); continue; }
                if (bank_count == room) {
                    struct bank *more = realloc(banks, (room = room ? room * 2 : 64) * sizeof *more);
                    if (!more) port_halt(PORT_EXIT_PROGRAM, "arcade: out of memory");
                    banks = more;
                }
                banks[bank_count].key = fnv(b, n, 0x811c9dc5u);
                banks[bank_count].size = n;
                banks[bank_count].bytes = b;
                bank_count++;
            }
    }
}

/* The key of the disc header that the header of `slot` in memory is. Once it has opened a header the library rewrites its last 512
 * bytes (the samples' sizes become their places in the sound chip's memory) and bytes 8 to 15 of each of the 128 program records
 * (0xff on the disc); the rest is compared: equal to one disc header, or 0 when none is (or several that differ are). */
static int same_header(const unsigned char *disc, const unsigned char *memory, unsigned len)
{
    unsigned k;
    for (k = 0; k < len - 512u; k++)
        if (disc[k] != memory[k] && !(k >= 0x20u && k < 0x820u && (k - 0x20u) % 16u >= 8u)) return 0;
    return 1;
}

static int sound_bank(unsigned slot, unsigned vh, unsigned programs, uint32_t *key)
{
    const unsigned char *h = game(vh, 32);
    unsigned len = 3104u + 512u * (programs > 1 ? programs - 1 : 0), i, k, best = 0, best_same = 0;
    if (!h) return 0;
    if (slot_bank[slot].address == vh && slot_bank[slot].programs == programs && !memcmp(slot_bank[slot].head, h, 32)) {
        *key = slot_bank[slot].key;
        return slot_bank[slot].known;
    }
    if (!(h = game(vh, len))) return 0;
    for (i = 0, k = 0; i < bank_count; i++) {
        if (banks[i].size != len || !same_header(banks[i].bytes, h, len)) continue;
        if (best_same && banks[i].key != banks[best].key) k = 1;   /* two disc headers of other keys fit */
        best_same = len - 512u;
        best = i;
    }
    slot_bank[slot].address = vh;
    slot_bank[slot].programs = programs;
    memcpy(slot_bank[slot].head, h, 32);
    slot_bank[slot].known = best_same && !k;
    slot_bank[slot].key = slot_bank[slot].known ? banks[best].key : 0;
    port_trace_line("arcade: sound slot %u: the header at 0x%08x (%u bytes) %s 0x%08x", slot, vh, len,
                    slot_bank[slot].known ? "is the disc's header of key" : k ? "fits disc headers of several keys:" : "is no disc header:", slot_bank[slot].key);
    *key = slot_bank[slot].key;
    return slot_bank[slot].known;
}

static const unsigned char *game(unsigned address, size_t n)
{
    return (const unsigned char *)port_gs((const void *)(uintptr_t)address, n);
}

static unsigned word_at(unsigned address)
{
    const unsigned char *p = game(address, 4);
    return p ? (unsigned)p[0] | (unsigned)p[1] << 8 | (unsigned)p[2] << 16 | (unsigned)p[3] << 24 : 0;
}

static uint32_t fnv(const unsigned char *p, size_t n, uint32_t h)
{
    size_t i;
    for (i = 0; i < n; i++) h = (h ^ p[i]) * 0x01000193u;
    return h;
}

/* Play the request as the arcade's effect: 1, or 0 when the arcade has no effect for it. */
static int arcade_effect(unsigned slot, int alt, unsigned start, int pos)
{
    const unsigned char *h, *e;
    unsigned vh, list, programs, k, lo = 0, hi = arcade_sound_count, code, dx, pan;
    uint32_t key_vab, key_run = 0x811c9dc5u;
    unsigned char packet[16];
    if (!player || slot > 3) return 0;
    vh = word_at(word_at(CHUNK_TABLES + 12u) + 4u * slot);
    if (!(h = game(vh, 0x14))) return 0;
    programs = h[0x12] | h[0x13] << 8;
    if (!sound_bank(slot, vh, programs, &key_vab)) return 0;
    list = word_at(COMMAND_LISTS + 4u * slot);
    for (k = start; k < start + 80u; k++) {   /* the sound's entries: up to the first without bit 23 of word 1 */
        if (!(e = game(list + 8u * k, 8))) return 0;
        key_run = fnv(e, 8, key_run);
        if (!(e[6] & 0x80)) break;
    }
    while (lo < hi) {
        unsigned mid = (lo + hi) / 2;
        const struct arcade_sound *a = &arcade_sounds[mid];
        if (a->key_vab < key_vab || (a->key_vab == key_vab && a->key_run < key_run)) lo = mid + 1;
        else hi = mid;
    }
    port_trace_line("arcade: sound slot %u start %u: header 0x%08x programs %u key 0x%08x, run key 0x%08x: %s", slot, start, vh, programs, key_vab, key_run,
                    lo < arcade_sound_count && arcade_sounds[lo].key_vab == key_vab && arcade_sounds[lo].key_run == key_run ? "known" : "not known");
    if (lo == arcade_sound_count || arcade_sounds[lo].key_vab != key_vab || arcade_sounds[lo].key_run != key_run) return 0;
    code = alt ? arcade_sounds[lo].code1 : arcade_sounds[lo].code0;
    if (code == 0xffffu) return 0;
    dx = pos < 0 ? 0u : pos > 0x17f ? 0x17fu : (unsigned)pos;
    for (k = 0, pan = 0; k < 15; k++) pan += arcade_sound_pan_edges[k] <= dx;
    pan = arcade_sound_pan_first + pan - 1u;
    memset(packet, 0, sizeof packet);
    packet[0] = (unsigned char)(code >> 8);
    packet[1] = (unsigned char)code;
    packet[7] = (unsigned char)pan;   /* byte 6: pan motion 0, fixed */
    port_audio_lock();
    arcade_music_packet(player, packet, sizeof packet);
    port_audio_unlock();
    sounds_arcade++;
    return 1;
}

/* The PS1's own sound for a request the arcade does not have: the game's func_80120604, as the wrappers call it. */
static void ps1_sound(unsigned slot, int alt, unsigned start, int pos)
{
    static port_fn impl;
    unsigned i;
    if (!impl)
        for (i = 0; i < port_function_count; i++)
            if (port_functions[i].image == -1 && !strcmp(port_functions[i].name, "func_80120604")) impl = (port_fn)port_functions[i].impl;
    if (!impl) port_halt(PORT_EXIT_OTHER, "arcade: func_80120604 has no implementation in this build");
    sounds_ps1++;
    port_call4(impl, slot, (unsigned)alt, start, (unsigned)(short)pos);
}

static void request(unsigned slot, int alt, unsigned start, int pos)
{
    if (!arcade_effect(slot, alt, start, pos)) ps1_sound(slot, alt, start, pos);
}

/* The camera-relative x of an object, clamped as the game's wrappers do. */
static int object_pos(unsigned object)
{
    const unsigned char *o = game(object + OBJECT_X, 2), *c = game(CAMERA_X, 2);
    int d = (o ? (short)(o[0] | o[1] << 8) : 0) - (c ? (short)(c[0] | c[1] << 8) : 0);
    return d < -192 ? -192 : d >= 576 ? 575 : d;
}

static void port_o_play_side_sound_at(unsigned object, int side, int sound);
static void port_o_play_side_sound_at(unsigned object, int side, int sound) { request((unsigned)side, side, (unsigned)sound & 0xffu, object_pos(object)); }
static void port_o_play_sound_at(unsigned object, int side, unsigned sound);
static void port_o_play_sound_at(unsigned object, int side, unsigned sound) { request((sound & 0xff00u) >> 8, side, sound & 0xffu, object ? object_pos(object) : 192); }
static void port_o_side_sound(int side, unsigned sound);
static void port_o_side_sound(int side, unsigned sound) { request((sound & 0xff00u) >> 8, side, sound & 0xffu, side == 0 ? 64 : 320); }

const struct port_override port_arcade_overrides[] = {
    { "play_side_sound_at", (void *)port_o_play_side_sound_at, "plays the arcade's effect for the sound when the arcade has one (setting arcade_assets)", &port_settings.arcade_assets },
    { "play_sound_at", (void *)port_o_play_sound_at, "plays the arcade's effect for the sound when the arcade has one (setting arcade_assets)", &port_settings.arcade_assets },
    { "call_func_with_side_x_origin", (void *)port_o_side_sound, "plays the arcade's effect for the sound when the arcade has one (setting arcade_assets)", &port_settings.arcade_assets },
    { NULL, NULL, NULL, NULL }
};

static void sound_counts(void)
{
    printf("arcade: sound: %u requests played as the arcade's effects, %u as the PS1's own sounds\n", sounds_arcade, sounds_ps1);
    fflush(stdout);
}

static void music_attach(void)
{
    static int told;
    if (!told) { told = 1; atexit(sound_counts); }
    if (!player && !(player = arcade_music_open(&rom, 1))) port_halt(PORT_EXIT_PROGRAM, "arcade: the music player cannot start");
    playing = -1;
    port_cd_stream_hook = stream_hook;
    port_cd_audio_replace = music_replace;
    port_audio_set_mix(arcade_mix);
    printf("arcade: music: %u of the disc's recorded tracks played as the arcade's songs\n", (unsigned)(sizeof songs / sizeof songs[0]));
    fflush(stdout);
}

static int by_place(const void *a, const void *b)
{
    const struct range *x = a, *y = b;
    if (x->sector != y->sector) return x->sector < y->sector ? -1 : 1;
    return x->offset < y->offset ? -1 : x->offset > y->offset;
}

/* Sort the ranges by file and place, and note each file's span of them. */
static void index_ranges(void)
{
    unsigned i, k = 0;
    qsort(ranges, range_count, sizeof ranges[0], by_place);
    memset(file_ranges, 0, sizeof file_ranges);
    for (i = 0; i < file_count; i++) {
        while (k < range_count && ranges[k].sector < files[i].sector) k++;
        file_first[i] = k;
        while (k < range_count && ranges[k].sector == files[i].sector) { file_ranges[i]++; k++; }
    }
}

/* The SHA-256 of the bytes built equal to the disc's in file f, its ranges one after another in order (after index_ranges). */
static void built_digest(int f, unsigned char out[32])
{
    unsigned i, end = file_first[f] + file_ranges[f];
    size_t total = 0, at = 0;
    unsigned char *all;
    for (i = file_first[f]; i < end; i++)
        if (ranges[i].equal) total += ranges[i].size;
    if (!(all = malloc(total ? total : 1))) port_halt(PORT_EXIT_PROGRAM, "arcade: out of memory");
    for (i = file_first[f]; i < end; i++)
        if (ranges[i].equal) { memcpy(all + at, ranges[i].bytes, ranges[i].size); at += ranges[i].size; }
    port_sha256(all, total, out);
    free(all);
}

static int is_named(const char *name, const char *const *list)
{
    for (; *list; list++)
        if (!strcmp(name, *list)) return 1;
    return 0;
}

static int le_put(FILE *o, unsigned long long v, unsigned bytes)
{
    unsigned char b[8];
    unsigned i;
    for (i = 0; i < bytes; i++) b[i] = (unsigned char)(v >> (8 * i));
    return fwrite(b, 1, bytes, o) == bytes ? 0 : -1;
}

struct extent { unsigned first, count, kind, file_size, file; unsigned long long at; };

/* Write the pack (port/arcade/README.md, "Playing without the disc"; the format is in disc.c): the disc's layout and every byte the
 * loader does not build equal to the disc's. The music streams and the two movies the port plays are whole raw sectors; the movies of
 * the programs it skips and the padding file are left out. Called after port_arcade_attach without check, with the disc. */
int port_arcade_import(struct port_disc *d, const char *path, char *err, size_t size)
{
    static const char *const raw_files[] = {"capcom.str", "z2o.str", NULL};
    static const char *const left_out[] = {"a2o.str", "bf3.str", "znull.dat", NULL};
    unsigned total = (unsigned)(d->length / PORT_SECTOR), i, n = 0, next = 0, k;
    struct extent *ex = calloc(2u * file_count + 2u, sizeof *ex);
    unsigned long long at, bytes_kept = 0, sectors_raw = 0;
    FILE *o;
    unsigned char raw[PORT_SECTOR];
    if (!enabled || !ex) { snprintf(err, size, "arcade: import: %s", ex ? "the arcade set is not open" : "out of memory"); free(ex); return -1; }
    for (i = 0; i <= file_count; i++) {   /* files in sector order, the sectors between them as raw extents */
        unsigned first = i < file_count ? files[i].sector : total, count = i < file_count ? (files[i].size + PORT_DATA - 1u) / PORT_DATA : 0;
        if (first > next) { ex[n].first = next; ex[n].count = first - next; ex[n].kind = 1; ex[n].file = ~0u; n++; }
        if (i == file_count) break;
        if (count && first >= next) {
            ex[n].first = first; ex[n].count = count; ex[n].file = i; ex[n].file_size = files[i].size;
            ex[n].kind = is_named(files[i].name, left_out) ? 2u : (strstr(files[i].name, ".xa") || is_named(files[i].name, raw_files)) ? 1u : 0u;
            n++;
            next = first + count;
        }
    }
    if (!(o = fopen(path, "wb"))) { free(ex); snprintf(err, size, "arcade: import: cannot create %s", path); return -1; }
    k = 0;
    for (i = 0; i < n; i++) k += ex[i].kind != 2;
    at = 16ull + 64ull * k;
    for (i = 0; i < n; i++) {   /* where each extent's data goes */
        struct extent *e = &ex[i];
        if (e->kind == 2) continue;
        e->at = at;
        if (e->kind == 1) at += (unsigned long long)e->count * PORT_SECTOR;
        else {
            unsigned r, end = file_first[e->file] + file_ranges[e->file], built_n = 0, data_n = 0, cur = 0;
            unsigned long long data_bytes = 0;
            for (r = file_first[e->file]; r < end; r++)
                if (ranges[r].equal) {
                    built_n++;
                    if (ranges[r].offset > cur) { data_n++; data_bytes += ranges[r].offset - cur; }
                    cur = ranges[r].offset + ranges[r].size;
                }
            if (e->file_size > cur) { data_n++; data_bytes += e->file_size - cur; }
            at += 8ull * (built_n + data_n) + data_bytes;
        }
    }
    if (fwrite("SFA2PK01", 1, 8, o) != 8 || le_put(o, total, 4) || le_put(o, k, 4)) goto bad;
    for (i = 0; i < n; i++) {   /* the table */
        struct extent *e = &ex[i];
        unsigned char digest[32];
        unsigned built_n = 0, data_n = 0, r, end, cur = 0;
        if (e->kind == 2) continue;
        memset(digest, 0, sizeof digest);
        if (e->kind == 0) {
            end = file_first[e->file] + file_ranges[e->file];
            for (r = file_first[e->file]; r < end; r++)
                if (ranges[r].equal) { built_n++; if (ranges[r].offset > cur) data_n++; cur = ranges[r].offset + ranges[r].size; }
            if (e->file_size > cur) data_n++;
            built_digest((int)e->file, digest);
        }
        if (le_put(o, e->first, 4) || le_put(o, e->count, 4) || le_put(o, e->kind, 4) || le_put(o, e->kind == 0 ? e->file_size : 0, 4) ||
            le_put(o, data_n, 4) || le_put(o, built_n, 4) || fwrite(digest, 1, 32, o) != 32 || le_put(o, e->at, 8)) goto bad;
    }
    for (i = 0; i < n; i++) {   /* the data */
        struct extent *e = &ex[i];
        if (e->kind == 2) continue;
        if (e->kind == 1) {
            for (k = 0; k < e->count; k++) {
                if (port_disc_raw(d, e->first + k, raw) != 0 || fwrite(raw, 1, PORT_SECTOR, o) != PORT_SECTOR) goto bad;
            }
            sectors_raw += e->count;
        } else {
            unsigned r, end = file_first[e->file] + file_ranges[e->file], cur = 0, pass;
            unsigned char *disc = disc_bytes(d, &files[e->file], 0, e->file_size);
            if (!disc) goto bad;
            for (pass = 0; pass < 3; pass++) {   /* the data ranges, the built ranges, then the data ranges' bytes */
                cur = 0;
                for (r = file_first[e->file]; r <= end; r++) {
                    unsigned start = r < end ? ranges[r].offset : e->file_size;
                    if (r < end && !ranges[r].equal) continue;
                    if (pass == 1) {
                        if (r < end && (le_put(o, ranges[r].offset, 4) || le_put(o, ranges[r].size, 4))) { free(disc); goto bad; }
                    } else if (start > cur) {
                        if (pass == 0 ? (le_put(o, cur, 4) || le_put(o, start - cur, 4)) : fwrite(disc + cur, 1, start - cur, o) != start - cur) { free(disc); goto bad; }
                        if (pass == 2) bytes_kept += start - cur;
                    }
                    if (r < end) cur = ranges[r].offset + ranges[r].size;
                }
            }
            free(disc);
        }
    }
    if (fclose(o) != 0) { o = NULL; goto bad; }
    printf("arcade: import: %s: %llu bytes of the disc's files kept (what the loader does not build) and %llu raw sectors of streams; %llu MB in all\n",
           path, bytes_kept, sectors_raw, at >> 20);
    fflush(stdout);
    free(ex);
    return 0;
bad:
    if (o) fclose(o);
    free(ex);
    snprintf(err, size, "arcade: import: cannot write %s", path);
    return -1;
}

int port_arcade_attach(struct port_disc *d, int check, char *err, size_t size)
{
    unsigned wrong;
    if (!enabled) return 0;
    pack_disc = d->pack ? d : NULL;
    if (port_disc_list(d, files, MAX_FILES, &file_count, err, size) != 0) return -1;
    qsort(files, file_count, sizeof files[0], by_sector);
    memset(told, 0, sizeof told);
    memset(built, 0, sizeof built);
    memset(built_bytes, 0, sizeof built_bytes);
    wrong = fighters(d, check);
    wrong += parts(d, check);
    module_data(d, check);
    stages(d, check);
    wrong += screens(d, check);
    screen_lists(d, check);
    if (check) coverage(check);
    if (check) return wrong ? 1 : 0;
    index_ranges();
    read_banks(d);
    if (pack_disc) {   /* what is served now must be what was served when the pack was made */
        unsigned i;
        for (i = 0; i < file_count; i++) {
            const unsigned (*r)[2];
            const unsigned char *digest;
            unsigned char got[32];
            unsigned count;
            if (port_disc_pack_built(d, files[i].sector, &r, &count, &digest) != 0) continue;
            built_digest((int)i, got);
            if (memcmp(got, digest, 32) != 0) {
                snprintf(err, size, "arcade: the pack was made by another build of the loader (%s differs): make it again with --arcade-import", files[i].name);
                return -1;
            }
        }
        printf("arcade: the pack's files and the loader's build agree\n");
    }
    music_attach();
    port_disc_patch_hook = patch;
    port_cd_read_hook = read_hook;
    return 0;
}
