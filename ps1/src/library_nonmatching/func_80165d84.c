/*
 * Nonmatching. This function is NOT byte-identical to the original: the
 * build of this C differs from the original's bytes (the printed line of the
 * test gives both sizes). The build does not use this file. The differential
 * test next to it (difftest.py, with func_80165d84.py as the contract)
 * compares the behavior of this C with the original code on random inputs of
 * the contract below.
 *
 * Provenance: written by this project from the listing of the original. The
 * sound library of the reference that the SDK files come from has no
 * function of this kind for a group of sequences; the single-sequence twin
 * _SsInitSoundSeq of ../sdk/libsnd/seqinit.c was read as an aid and none of
 * its text is copied. The library's name for this address is probably
 * _SsInitSoundSep (inferred).
 *
 * What it does (inferred, not an original name): prepares the sequence
 * record "score" = _ss_score[slot][index] (172 bytes; fields named here by
 * their offsets) for the sequence number index of a group that lies at addr,
 * and returns how many bytes of the group that sequence takes (header
 * bytes, 5 for resolution and tempo, 6 for what follows and a 4-byte length
 * word that is added to the result), or -1.
 *   - Clears the bytes 0x10 to 0x16 and 0x27 to 0x2b, the halfwords at 0x48
 *     and 0x72, the words at 0x7c, 0x80, 0x84 and 0x88; sets the halfword at
 *     0x6e to 1 and the one at 0xa8 to 0x7f, the halfword at 0x4a to 0 (then
 *     to the resolution, below), the halfword at 0x4c to vab_id; for the 16
 *     channels sets the byte at 0x17 + c to 0x40, the byte at 0x2c + c to c
 *     and the halfword at 0x4e + 2c to 0x7f.
 *   - Sets the read position (word at 4) to addr. With index 0: when the
 *     first byte is 'S' or 'p' the position becomes addr + 6, and when the
 *     byte at addr + 5 is not 0 it prints "This is not SEP Data." through
 *     printf and returns -1; else the position becomes addr + 8 (8 bytes
 *     counted). When the first byte is neither 'S' nor 'p' the position
 *     stays addr, nothing is counted and no message is printed. With index
 *     other than 0 the position is addr + 2 and 2 bytes are counted.
 *   - Reads the big-endian halfword at the position into the halfword at
 *     0x4a (inferred: resolution) and the 3 bytes after it as a big-endian
 *     number t (inferred: tempo as microseconds per beat); 5 bytes are
 *     counted. It stores t in the word at 0x84, and then 60000000 / t, plus
 *     1 when t / 2 (a logical shift) is less than 60000000 % t; the word at
 *     0x8c gets the same value.
 *   - Skips 2 bytes, reads the next 4 bytes as a big-endian number len (the
 *     length of this sequence in the group, inferred), advances the position
 *     by 6 in all and counts 6.
 *   - Calls _SsReadDeltaValue(slot, index) (inferred name; the library's
 *     function at 0x80168038) and stores the result in the words at 0x7c and
 *     0x88; then copies the position (word at 4, read again after the call)
 *     into the words at 8 and 0xc.
 *   - With V = the library variable VBLANK_MINUS and p = the signed
 *     halfword at 0x4a times the word at 0x84 (a 32-bit product): when
 *     p * 10 < V * 60 (unsigned) the halfwords at 0x6e and 0x70 get
 *     V * 600 / p (unsigned); otherwise the halfword at 0x6e gets -1 and the
 *     one at 0x70 gets p * 10 / (V * 60), plus 1 when V * 30 is less than
 *     p * 10 % (V * 60). The halfword at 0x72 gets the one at 0x70. The
 *     result is the count plus len.
 *
 * Contract (the roles named for fields are inferred):
 *   Arguments: a0 = slot, a1 = index, a2 = vab_id (all s16; the original
 *     sign-extends the low half of each), a3 = addr, a pointer to bytes.
 *     slot is 0 to 31 and index 0 to 3 (the row _ss_score[slot] holds four
 *     records). Returns the count plus len as an int, or -1.
 *   Reads: _ss_score[slot]; the bytes at addr that are named above, 12 to 26
 *     of them at most (8 header bytes, then 11 or 13); VBLANK_MINUS; the
 *     words and halfwords of the score that it has just written.
 *   Writes: the score as listed above; nothing else.
 *   Callees: printf and _SsReadDeltaValue are replaced by recorders in both
 *     runs. printf (1 argument) logs its string (6 words behind the pointer;
 *     the pointer itself is masked to 0, the strings sit at other addresses
 *     in the two builds). _SsReadDeltaValue (2 arguments, masks 0xffff)
 *     returns a random word the setup chose, and logs a copy of the whole
 *     score (43 words) at its call, so that a store made after the call
 *     instead of before is a difference. What the real callee does (it reads
 *     the position and moves it) is outside the test: the recorder leaves
 *     the position as it is.
 *   Aliasing: the score row, the table _ss_score, addr's bytes and the
 *     recorders' blocks do not overlap.
 *   Excluded inputs (the original traps with a break instruction): t equal
 *     to 0; p equal to 0 when p * 10 < V * 60 holds (a division by zero);
 *     V * 60 equal to 0 modulo 2^32. The setup keeps the bytes of t from
 *     being all 0, makes the halfword of the resolution nonzero and V not a
 *     multiple of 2^30; p can still be 0 modulo 2^32 only if the product
 *     wraps to exactly 0, which the inputs of the setup do not make (the
 *     resolution is below 0x10000 and t is at least 1).
 *   Not reached by any input (the test's --uncovered lists seven slots, all
 *     of them guards of the division that the compiler of the original
 *     placed): the break for a zero divisor after the division by t
 *     (+0x1d0), the test for the quotient of -1 and the smallest number with
 *     its break (+0x1e0..+0x1e8), and the breaks for a zero divisor in the
 *     three divisions of the last step (+0x2f8, +0x33c, +0x370). The setup
 *     keeps all of them out (see Excluded inputs). This C has no such guards;
 *     a zero divisor in it is whatever the host's division does.
 */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

/* A local view of the score record (172 bytes); fields by their offsets. */
typedef struct {
    u8 pad00[4];
    u8 *read_pos;     /* 0x04 */
    u8 *next_pos;     /* 0x08 */
    u8 *loop_pos;     /* 0x0c */
    u8 f10;
    u8 f11;
    u8 f12;
    u8 f13;
    u8 f14;
    u8 f15;
    u8 f16;
    u8 pan[16];       /* 0x17 */
    u8 f27;
    u8 f28;
    u8 f29;
    u8 f2a;
    u8 f2b;
    u8 programs[16];  /* 0x2c */
    u8 pad3c[12];
    s16 f48;
    s16 f4a;
    s16 f4c;
    s16 vol[16];      /* 0x4e */
    s16 f6e;
    s16 f70;
    u16 f72;
    u8 pad74[8];
    s32 f7c;
    s32 f80;
    s32 f84;
    s32 f88;
    s32 f8c;
    u8 pad90[0x18];
    s16 fa8;
    u8 padaa[2];
} ScoreView;

extern ScoreView *_ss_score[];
extern u32 VBLANK_MINUS;

int printf(const char *, ...);
s32 _SsReadDeltaValue(s16 slot, s16 index);

int func_80165d84(s16 slot, s16 index, s16 vab_id, u8 *addr) {
    ScoreView *score;
    int ch;
    int used;
    int len;
    s32 tempo;
    s32 quot;
    s32 rem;
    u32 prod;
    u32 v;
    u8 *p;

    used = 0;
    score = &_ss_score[slot][index];
    score->f6e = 1;
    score->f10 = 0;
    score->f11 = 0;
    score->f12 = 0;
    score->f13 = 0;
    score->f14 = 0;
    score->f15 = 0;
    score->f16 = 0;
    score->f27 = 0;
    score->f28 = 0;
    score->f29 = 0;
    score->f2a = 0;
    score->f2b = 0;
    score->f48 = 0;
    score->f4a = 0;
    score->f4c = vab_id;
    score->f72 = 0;
    score->f7c = 0;
    score->f80 = 0;
    score->f84 = 0;
    score->f88 = 0;
    score->fa8 = 0x7F;
    for (ch = 0; ch < 16; ch++) {
        score->pan[ch] = 0x40;
        score->programs[ch] = ch;
        score->vol[ch] = 0x7F;
    }
    score->read_pos = addr;
    if ((s16)index == 0) {
        if (*addr == 'S' || *addr == 'p') {
            score->read_pos = addr + 6;
            if (addr[5] != 0) {
                printf("This is not SEP Data.\n");
                return -1;
            }
            score->read_pos = addr + 8;
            used += 8;
        }
    } else {
        score->read_pos = addr + 2;
        used += 2;
    }
    p = score->read_pos;
    score->read_pos = p + 1;
    score->f4a = p[1] | (p[0] << 8);
    score->read_pos = p + 2;
    p = score->read_pos;
    tempo = (p[0] << 16) | (p[1] << 8) | p[2];
    score->read_pos = p + 3;
    quot = 60000000 / tempo;
    rem = 60000000 % tempo;
    score->f84 = tempo;
    used += 5;
    if ((tempo >> 1) < rem) {
        score->f84 = quot + 1;
    } else {
        score->f84 = quot;
    }
    score->f8c = score->f84;
    p = score->read_pos;
    score->read_pos = p + 6;
    len = (p[2] << 24) + (p[3] << 16) + (p[4] << 8) + p[5];
    score->f7c = _SsReadDeltaValue(slot, index);
    score->f88 = score->f7c;
    score->loop_pos = score->read_pos;
    score->next_pos = score->read_pos;
    v = VBLANK_MINUS;
    prod = score->f4a * score->f84;
    used += 6;
    if (prod * 10 < v * 60) {
        score->f6e = score->f70 = (v * 600) / prod;
    } else {
        score->f6e = -1;
        score->f70 = (prod * 10) / (v * 60);
        if (v * 30 < (prod * 10) % (v * 60)) {
            score->f70++;
        }
    }
    score->f72 = score->f70;
    return used + len;
}
