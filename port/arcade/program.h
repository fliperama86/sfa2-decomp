/* The parts of the PS1 program's data that the arcade program holds too: each
 * a range of the PS1 program at its address, built from a range of the arcade
 * program. Only addresses and layouts are here, no byte of either game. */
#ifndef ARCADE_PROGRAM_H
#define ARCADE_PROGRAM_H

#include <stddef.h>

/* The PS1 program's data: from the end of its last function to the end of
 * its last byte that is not zero (the rest of the image is zeros). */
#define ARCADE_PS1_DATA_START 0x8016d91cu
#define ARCADE_PS1_DATA_END   0x80183904u   /* 90,088 bytes of data */

struct arcade_piece {
    unsigned address;    /* in the PS1 program */
    unsigned size;       /* bytes, even */
    unsigned source;     /* address in the arcade program, even */
    const char *units;   /* one letter a 16-bit unit, repeated over the piece: 'b' two bytes as they are,
                          * 'w' a 16-bit value (its two bytes turned, big-endian to little-endian) */
    const char *name;    /* the PS1 symbol it lies in, and the offset */
};

extern const struct arcade_piece arcade_program_pieces[];
extern const unsigned arcade_program_piece_count;

/* The piece's bytes, as the PS1 program has them, into out[0..p->size). -1
 * when the piece does not fit the arcade program or its layout is malformed. */
int arcade_piece_build(const unsigned char *program, size_t program_size, const struct arcade_piece *p, unsigned char *out);
/* The table's own order and bounds: sorted by address, not overlapping, each
 * inside the PS1 program's data, even, with a well-formed layout. 0 or -1. */
int arcade_pieces_valid(const struct arcade_piece *p, unsigned count);

#endif
