/*
 * Nonmatching. This function is NOT byte-identical to the original: the
 * original keeps its channel number in two registers and a pointer in a
 * third, loads some words with lw and masks them where this C reads them as
 * halfwords of the same value, and has a frame of another size (all read
 * from the original's listing, not tested); the code built from this C has
 * other registers and a different size. The exact owner of the bytes in the
 * PS1 build stays the raw bytes of the resident image; the build does not
 * use this file. The differential test next to it (difftest.py) compares
 * the behavior of this C with the original code on random inputs of the
 * contract below.
 *
 * What it does (inferred, not an original name): plays a list of sound
 * commands. The list is a run of 8-byte entries, found through
 * table_8016e6e0[a] and the index c; each entry is two words, and the walk
 * goes on while bit 23 of the second word is set. The command is bits 12 to
 * 15 of the first word:
 *   0  start a note on a channel: the channel is bits 7 to 11 of the second
 *      word (plus 4 when b is not 0). If the entry's priority (bits 16 to 22
 *      of the second word) is lower than the one stored for the channel
 *      (data_80197ed0[channel]) and func_8016a7e4 for the channel's bit
 *      returns 2, the entry is skipped. Otherwise it stores the priority
 *      and starts the note with func_80164140, with the instrument of
 *      data_80190a44[a], two volumes scaled by a pan, and fields of the
 *      entry. The pan is byte 2 of the first word, or, when that is 0xff,
 *      a value stepped from d (d += 0xc0, divided by 6).
 *   1  set the volumes of the program data_80190a44[a + 8] with a pan from
 *      d as above (unscaled volumes 0x7f), remember c for a below 2 in
 *      table_80197ef8, and make three calls (func_80164ef0, func_8016936c,
 *      func_80168f50) with the program and the low 12 bits of the first word.
 *   9  with the low 12 bits 0xfff, call func_80165d34(byte 2, byte 2) of the
 *      first word, else func_80164a78(the low 12 bits, byte 2, byte 2).
 *  11  stop a channel: low 12 bits 0xfff stops all with func_80164bbc(0);
 *      otherwise the channel (plus 4 when b is not 0) has its priority cleared
 *      and func_8016452c is called for it.
 *  2 to 8, 10 and 12 to 15  do nothing.
 * a equal to 0x80 calls func_80164bbc(0) and returns.
 *
 * Contract:
 *   Arguments: a0 = a (0 to 0x7f, or 0x80), a1 = b, a2 = c (only its low 16
 *     bits are used), a3 = d (only its low 16 bits are used).
 *   No return value.
 *   Reads: table_8016e6e0[a]; the entries; data_80190a44[a] and [a + 8];
 *     data_80197ed0[channel].
 *   Writes: data_80197ed0[channel], table_80197ef8[a] (a below 2). The
 *     channel of command 11 is the low 12 bits of the first word (plus 4
 *     when b is not 0), so it can be as large as 0xffe + 4 (the setup draws
 *     such values); that of command 0 is 0 to 31, plus 4 when b is not 0.
 *     The setup fills the 0x1004 bytes from data_80197ed0 that such a
 *     channel can reach with random bytes (the image has zeros there, and
 *     a zero stored over a zero would show nothing), so the far store is
 *     compared like any other at the end of the case.
 *   Watched at every call (copied into the log): the 36 bytes of
 *     data_80197ed0 and the word at table_80197ef8.
 *   Callees replaced by recorders, the same in both runs (all in Sony's
 *     library, from 0x80164000; inferred from their addresses):
 *     func_80164bbc (1 argument), func_8016a7e4
 *     (1 argument; returns 2 in half of the cases and another value
 *     otherwise, the same for all its calls in a case), func_80164140 (8
 *     arguments), func_80164ef0 (4), func_8016936c (2), func_80168f50 (4),
 *     func_80165d34 (2), func_80164a78 (3), func_8016452c (1). All other
 *     results are 0. The volumes are passed as 16-bit values: the products
 *     are unsigned shifts of possibly negative numbers, as in the original.
 *   Excluded inputs: a above 0x80 (the original indexes past the table;
 *     inferred).
 *   Not changed from the original: the loop test reads the second word after
 *     the command has run.
 *   Not reached by any input: none expected; see the coverage line.
 */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

/* Inferred declarations; not original ones. */
extern u8 data_80197ed0[];
extern s16 table_80197ef8[];
int func_8016a7e4(unsigned mask);
void func_80164140(int a, int b, int c, int d, int e, int f, short g, short h);
void func_80164ef0(int a, int b, unsigned short c, unsigned short d);
void func_80168f50(short a, short b, unsigned char c, short d);
void func_80164a78(int a, int b, int c);
void func_8016452c(int a);

/* Steps the pan source by 0xc0 and returns the pan (the stepped value
   divided by 6). */
static int next_pan(short *d) {
    *d += 0xc0;
    return *d / 6;
}

void func_80120604(int a, int b, int c, short d) {
    u32 *p;
    int more;
    int ch;
    int pan;
    unsigned left;
    unsigned right;

    if (a == 0x80) {
        func_80164bbc(0);
        return;
    }
    p = table_8016e6e0[a] + (u16)c * 2;
    do {
        switch ((p[0] >> 12) & 0xf) {
        case 0:
            ch = (p[1] >> 7) & 0x1f;
            if (b != 0) {
                ch += 4;
            }
            if ((int)((p[1] >> 16) & 0x7f) < data_80197ed0[ch]) {
                if (func_8016a7e4(1 << ch) == 2) {
                    break;
                }
            }
            data_80197ed0[ch] = (p[1] >> 16) & 0x7f;
            left = p[1] & 0x7f;
            right = left;
            if (((u8 *)p)[2] == 0xff) {
                pan = next_pan(&d);
            } else {
                pan = ((u8 *)p)[2];
            }
            if (pan < 0x40) {
                left = (unsigned)(pan * (short)left) >> 6;
            } else {
                right = (unsigned)((0x7f - pan) * (short)right) >> 6;
            }
            func_80164140(ch, data_80190a44[a], ((u8 *)p)[3], (p[1] >> 12) & 0xf,
                          (p[1] >> 24) & 0x7f, 0, right, left);
            break;
        case 1:
            pan = next_pan(&d);
            left = 0x7f;
            right = 0x7f;
            if (pan < 0x40) {
                left = (unsigned)(pan * 0x7f) >> 6;
            } else {
                right = (unsigned)((0x7f - pan) * 0x7f) >> 6;
            }
            if (a < 2) {
                table_80197ef8[a] = c;
            }
            /* The definition (s160fbc_r10_b.c) takes its last two parameters as unsigned short. The
               original's code sign-extends them here before the call, so the call is made through a
               type that takes them as short: called with the definition's own type, this C zero-extends
               them and its test differs from the original. */
            ((void (*)(int, int, short, short))func_80164ef0)(data_80190a44[a + 8], p[0] & 0xfff, right, left);
            func_8016936c(data_80190a44[a + 8], p[0] & 0xfff);
            func_80168f50(data_80190a44[a + 8], p[0] & 0xfff, 1, 1);
            break;
        case 9:
            if ((p[0] & 0xfff) == 0xfff) {
                func_80165d34(((u8 *)p)[2], ((u8 *)p)[2]);
            } else {
                func_80164a78(p[0] & 0xfff, ((u8 *)p)[2], ((u8 *)p)[2]);
            }
            break;
        case 11:
            ch = p[0] & 0xfff;
            if (ch == 0xfff) {
                func_80164bbc(0);
            } else {
                if (b != 0) {
                    ch += 4;
                }
                data_80197ed0[ch] = 0;
                func_8016452c(ch);
            }
            break;
        default:
            break;
        }
        more = (p[1] & 0x800000) != 0;
        p += 2;
    } while (more);
}
