/* The user's arcade set (MAME's sfa2u), assembled into the views that the
 * loader reads: the 68000 program, the graphics as the video chip reads them,
 * the QSound samples and the sound CPU's program. Only the examined revision
 * is accepted: each view's SHA-256 is pinned. Nothing here is decrypted; the
 * loader reads data, never the program's encrypted opcodes. */
#ifndef ARCADE_ROM_H
#define ARCADE_ROM_H

#include <stddef.h>
#include "zip.h"

#define ARCADE_PROGRAM_SIZE 0x300000u   /* sz2u.03 to sz2u.08 */
#define ARCADE_TILES_SIZE 0x1400000u    /* sz2.13m to sz2.20m */
#define ARCADE_SAMPLES_SIZE 0x400000u   /* sz2.11m, sz2.12m */
#define ARCADE_SOUND_SIZE 0x40000u      /* sz2.01a, sz2.02a */

struct arcade_rom {
    unsigned char *program;   /* the 68000's view: words big-endian, as the CPU reads data */
    unsigned char *tiles;     /* 4 bits a pixel in planes, 128 bytes a 16x16 tile, as port/intro reads them */
    unsigned char *samples;   /* signed 8-bit samples in the order the QSound chip reads them */
    unsigned char *sound;     /* the Z80's program */
};

/* Assemble the views from the archive's members (sizes checked, digests not).
 * On failure nothing is left allocated. */
int arcade_rom_assemble(const struct arcade_zip *z, struct arcade_rom *r, char *err, size_t errsize);
/* Each view's SHA-256 against the examined set's. */
int arcade_rom_verify(const struct arcade_rom *r, char *err, size_t errsize);
/* Both, from the archive at path. */
int arcade_rom_open(const char *path, struct arcade_rom *r, char *err, size_t errsize);
void arcade_rom_close(struct arcade_rom *r);

#endif
