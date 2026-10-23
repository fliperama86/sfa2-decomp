#include "arcade.h"
#include "cd.h"
#include "port.h"
#include "../arcade/program.h"
#include "../arcade/rom.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_FILES 512

static struct arcade_rom rom;
static int enabled;
static struct port_disc_file files[MAX_FILES];   /* the disc's files, by first sector */
static unsigned char told[MAX_FILES];
static unsigned file_count;

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
    unsigned char *built;
    if (!enabled) return 0;
    built = malloc(ARCADE_PS1_DATA_END - ARCADE_PS1_DATA_START);
    if (!built) port_halt(PORT_EXIT_PROGRAM, "arcade: out of memory");
    for (i = 0; i < arcade_program_piece_count; i++) {
        const struct arcade_piece *p = &arcade_program_pieces[i];
        unsigned char *at = ram + (p->address - PORT_RAM_BASE);
        if (arcade_piece_build(rom.program, ARCADE_PROGRAM_SIZE, p, built) != 0)
            port_halt(PORT_EXIT_PROGRAM, "arcade: the piece %s does not fit the arcade program", p->name);
        if (check) {
            if (memcmp(built, at, p->size) != 0) {
                unsigned k = 0;
                while (built[k] == at[k]) k++;
                printf("arcade check: %s (0x%08x, %u bytes, from 0x%06x): differs first at +0x%x\n", p->name, p->address, p->size, p->source, k);
                differ++;
            }
        } else {
            memcpy(at, built, p->size);
        }
    }
    free(built);
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

/* The first read of each file: none of the disc's files is built by the loader yet. */
static void read_hook(unsigned sector)
{
    unsigned lo = 0, hi = file_count;
    while (lo < hi) {   /* the last file that starts at or before the sector */
        unsigned mid = (lo + hi) / 2;
        if (files[mid].sector <= sector) lo = mid + 1;
        else hi = mid;
    }
    if (lo == 0) return;
    lo--;
    if (sector >= files[lo].sector + (files[lo].size + PORT_DATA - 1u) / PORT_DATA && files[lo].size) return;
    if (told[lo]) return;
    told[lo] = 1;
    printf("arcade: %s from the disc: no arcade source yet (first read at frame %u)\n", files[lo].name, port_frames());
    fflush(stdout);
}

int port_arcade_attach(struct port_disc *d, char *err, size_t size)
{
    if (!enabled) return 0;
    if (port_disc_list(d, files, MAX_FILES, &file_count, err, size) != 0) return -1;
    qsort(files, file_count, sizeof files[0], by_sector);
    memset(told, 0, sizeof told);
    port_cd_read_hook = read_hook;
    return 0;
}
