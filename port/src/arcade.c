#include "arcade.h"
#include "cd.h"
#include "port.h"
#include "../arcade/fighter.h"
#include "../arcade/program.h"
#include "../arcade/rom.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_FILES 512
#define MAX_RANGES 128

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
    if (built[f]) printf("arcade: %s: its fighter lists and positions from the arcade set, the rest from the disc (first read at frame %u)\n", files[f].name, port_frames());
    else printf("arcade: %s from the disc: no arcade source yet (first read at frame %u)\n", files[f].name, port_frames());
    fflush(stdout);
}

/* Where table 0's chunk `slot` lies in the PAC file at `sector`: its offset in the file and its size. -1 if there is none. */
static int pac_chunk(struct port_disc *d, const struct port_disc_file *f, unsigned slot, unsigned *offset, unsigned *size)
{
    unsigned char head[PORT_DATA];
    char err[PORT_ERR];
    unsigned count, i, at = PORT_DATA;
    if (f->size < PORT_DATA || port_disc_read(d, f->sector, PORT_DATA, head, err, sizeof err) != 0) return -1;
    count = head[0] | head[1] << 8 | (unsigned)head[2] << 16 | (unsigned)head[3] << 24;
    if (count > (PORT_DATA - 0x20) / 32) return -1;
    for (i = 0; i < count; i++) {
        const unsigned char *e = head + 0x20 + 32 * i;
        unsigned type = e[0] | e[1] << 8 | (unsigned)e[2] << 16 | (unsigned)e[3] << 24;
        unsigned n = e[4] | e[5] << 8 | (unsigned)e[6] << 16 | (unsigned)e[7] << 24;
        if (n > f->size || at > f->size - n) return -1;
        if (type == slot) {   /* table 0: the type is the slot alone */
            *offset = at;
            *size = n;
            return 0;
        }
        at += (n + PORT_DATA - 1u) / PORT_DATA * PORT_DATA;
    }
    return -1;
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

/* Build each fighter file's position runs and cell lists. Built blocks equal to the disc's are served in place of it;
 * others stay the disc's. With check, only compare, and count the files that differ but for the PS1's known changes. */
static unsigned fighters(struct port_disc *d, int check)
{
    unsigned i, used = 0, kept = 0, bytes = 0, wrong = 0;
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
            used++;
            bytes += out.lists_size + out.positions_size;
            if (!check) {
                add_range(f, moff + a->lists, out.lists, out.lists_size);
                add_range(f, poff + a->positions, out.positions, out.positions_size);
                out.lists = out.positions = NULL;   /* the ranges own them now */
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
    fflush(stdout);
    return wrong;
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
    if (check) return wrong ? 1 : 0;
    port_disc_patch_hook = patch;
    port_cd_read_hook = read_hook;
    return 0;
}
