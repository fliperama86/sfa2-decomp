/*
 * Nonmatching. This function is NOT byte-identical to the original: the
 * built code differs from the original's bytes in instruction scheduling and
 * register choice, and the original holds two copies of its code (one for
 * each value of bit 15 of the flags) where this C holds one. The exact owner
 * of the bytes in the PS1 build stays the raw bytes of the resident image;
 * the build does not use this file. The differential test next to it
 * (difftest.py with func_8011df3c.py) compares the behavior of this C with
 * the original code on random inputs of the contract below.
 *
 * What it does (inferred, not an original name): unpacks one compressed
 * texture block into 16-bit cells. The flags pick the block: bits 0 to 13
 * index a table of 32-bit offsets at the start of the data, bit 15 selects
 * whether each cell has its four nibbles reversed, and bits 14 and 15 pick
 * one of four address masks. Every cell is stored at the destination
 * address (a running pointer) exclusive-or the mask. The block starts with
 * a halfword n:
 *   n == 0              the next 64 halfwords are the cells;
 *   high byte of n 0    low byte n8 is a count of cells in groups of 16:
 *                       ((n8 - 1) / 16 + 1) flag halfwords follow, then the
 *                       data; for each of n8 / 16 full groups and then
 *                       n8 % 16 more cells, one flag bit (lowest first)
 *                       selects either one literal halfword, or a reference:
 *                       a halfword whose low 2 bits plus 2 give a length
 *                       and whose upper 14 bits give a distance in
 *                       halfwords back from the start of the block; that
 *                       many cells are copied from there;
 *   n & 0xff00 == 0x8000  four flag halfwords follow, then the data; each of
 *                       the 64 flag bits selects a literal halfword (bit
 *                       set) or a zero cell (bit clear);
 *   any other n         the work is given to func_8011e5c8 (bit 15 set) or
 *                       func_8011e790 (clear), a byte-oriented unpacker
 *                       that is passed n, the stream after n, the
 *                       destination and the second mask.
 *
 * Contract (what the code reads and writes; roles are inferred):
 *   Arguments: a0 = base of the data, a1 = flags (the upper 16 bits are
 *     ignored), a2 = destination (an even address). No return value.
 *   Callees: func_8011e5c8 and func_8011e790 are game code that stays in
 *     game code (they call nothing); they run as the original, the same in
 *     both runs. The test thereby also checks the four arguments they get.
 *     Both read their fourth argument as an address mask.
 *   Reads: the offset table at the base, the block it points to, the data
 *     the references point back to, and (read-only, from the image) the two
 *     tables of four masks {0, 0x78, 6, 0x7e} and {0, 0x78, 7, 0x7f} that
 *     the unit holds as static const data.
 *   Writes: halfwords (bytes, for the callees) at destination exclusive-or
 *     mask for up to 2560 bytes (a few ten thousand for the callees).
 *   Aliasing: the data block and the destination block are distinct; a
 *     reference may point anywhere in the data block below the start of the
 *     stream, which the setup fills with random bytes.
 *   Excluded inputs: a table index above 7 and offsets that put the stream
 *     outside the data block; a destination whose block is smaller than the
 *     largest output; both are bounds of the setup, not of the function.
 *   Not reached by any input: none; every instruction slot of the original
 *     is executed (the original's tests that a reference length is not 0
 *     are always true, as the length is 2 to 5, and the slots they skip are
 *     reached from the literal path).
 */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_8011e5c8(unsigned char n, unsigned char *src, unsigned int dst, unsigned int key);
void func_8011e790(unsigned int n, unsigned char *src, unsigned int dst, unsigned int key);

/* Address masks, one pair per value of bits 14 and 15 of the flags. */
static const u16 mask_table[4] = {0x0000, 0x0078, 0x0006, 0x007e};
static const u16 key_table[4] = {0x0000, 0x0078, 0x0007, 0x007f};

/* Stores one cell at the running destination exclusive-or the mask. */
static u16 *put_cell(u16 *dst, u16 mask, u16 c, int reverse) {
    if (reverse)
        c = (c >> 12) | ((c & 0xf00) >> 4) | ((c & 0xf0) << 4) | ((c & 0xf) << 12);
    *(u16 *)((u32)dst ^ mask) = c;
    return dst + 1;
}

/* Unpacks "count" cells, each governed by the next bit of "bits": a literal
   halfword, or a reference (length, distance) into the block. */
static u16 *unpack_cells(u16 **stream, u16 *start, u16 *dst, u16 bits, int count, u16 mask, int reverse) {
    u16 *p = *stream;
    u16 *src;
    u16 c, len;
    int i, j;

    for (i = 0; i < count; i++) {
        if (bits & 1) {
            c = *p++;
            len = (c & 3) + 2;
            src = start - (c >> 2);
            for (j = 0; j < len; j++)
                dst = put_cell(dst, mask, *src++, reverse);
        } else {
            dst = put_cell(dst, mask, *p++, reverse);
        }
        bits >>= 1;
    }
    *stream = p;
    return dst;
}

void func_8011df3c(void *a, int b, u8 *c) {
    unsigned flags = b;
    u16 *dst = (u16 *)c;
    int reverse = (flags & 0x8000) != 0;
    u16 mask = mask_table[(flags >> 14) & 3];
    u16 key = key_table[(flags >> 14) & 3];
    u16 *start = (u16 *)((u8 *)a + ((int *)a)[flags & 0x3fff]);
    u16 *p = start;
    u16 pal[16];
    u16 *tp;
    u16 n, n8, bits;
    int i, j, groups;

    n = *p++;
    if (n == 0) {
        for (i = 0; i < 64; i++)
            dst = put_cell(dst, mask, *p++, reverse);
    } else if ((n & 0xff00) == 0) {
        n8 = n & 0xff;
        for (i = 0; i < ((n8 - 1) >> 4) + 1; i++)
            pal[i] = *p++;
        tp = pal;
        groups = n8 >> 4;
        for (i = 0; i < groups; i++)
            dst = unpack_cells(&p, start, dst, *tp++, 16, mask, reverse);
        if ((n8 & 0xf) != 0)
            dst = unpack_cells(&p, start, dst, *tp, n8 & 0xf, mask, reverse);
    } else if ((n & 0xff00) == 0x8000) {
        for (i = 0; i < 4; i++)
            pal[i] = *p++;
        for (i = 0; i < 4; i++) {
            bits = pal[i];
            for (j = 0; j < 16; j++) {
                if (bits & 1)
                    dst = put_cell(dst, mask, *p++, reverse);
                else
                    dst = put_cell(dst, mask, 0, reverse);
                bits >>= 1;
            }
        }
    } else if (reverse) {
        func_8011e5c8(n, (unsigned char *)p, (unsigned int)dst, key);
    } else {
        func_8011e790(n, (unsigned char *)p, (unsigned int)dst, key);
    }
}
