/*
 * Not part of the matching build, which does not know the stand-alone
 * programs of the disc. No tool of the tree compares this C's bytes with
 * the original's, so nothing is claimed about them here. The differential
 * test next to it (difftest.py, with func_800110f4_logo.py) compares the
 * behavior of this C with the original code of the logo's program
 * (PAC/LOGO.EXE, the body loaded at 0x80010000) on random inputs of the
 * contract below.
 *
 * What it does (inferred, not an original name): expands run-length data
 * into a stream of three-byte colors. The format, read from the original's
 * listing (not tested beyond the cases below):
 *   - the input is a stream of 16-bit codes, read with unsigned halfword
 *     loads, ended by the code 0xffff (it is looked for before every
 *     code, the first one included);
 *   - three running bytes (r, g, b) start at 0; every addition wraps at 8
 *     bits (the original keeps them in registers and stores their low
 *     bytes);
 *   - a code with bit 15 clear is an index into the table at a0 of
 *     three-byte entries: the entry's three bytes are added to r, g, b,
 *     and r, g, b are written to the output;
 *   - a code with bit 15 set (not 0xffff) is a repeat: its low 15 bits are
 *     a count, and r, g, b are written to the output that many times,
 *     unchanged; a count of 0 writes nothing;
 *   - the output pointer is read from the word at 0x8002eb10 (gp + 0x14)
 *     when the function starts and is kept in a register; the word is not
 *     written back.
 * The output is not bounded by the function: it writes three bytes per
 * written color to wherever the pointer leads. The bound is the data's:
 * the sum of the counts and literals.
 *
 * Contract:
 *   Arguments: a0 = the table of three-byte entries (bytes), a1 = the
 *     stream (aligned to 2 bytes, ended by 0xffff). No result.
 *   Reads: the word at 0x8002eb10; the codes of the stream up to the
 *     terminator, each of them (a literal code is read twice by the
 *     original); the three bytes of the entry of every literal code.
 *   Writes: the output bytes, three per written color, from the pointer.
 *   No callee.
 *   Slots no input can reach: none known.
 */
#include "logo.h"

void func_800110f4_logo(u8 *table, u16 *codes) {
    u8 *out = data_8002eb10_logo;
    u8 r = 0;
    u8 g = 0;
    u8 b = 0;

    while (*codes != 0xffff) {
        u16 code = *codes++;
        if ((code & 0x8000) == 0) {
            u8 *entry = table + code * 3;
            r += entry[0];
            g += entry[1];
            b += entry[2];
            *out++ = r;
            *out++ = g;
            *out++ = b;
        } else {
            u16 count = code & 0x7fff;
            u16 i;
            for (i = 0; i < count; i++) {
                *out++ = r;
                *out++ = g;
                *out++ = b;
            }
        }
    }
}
