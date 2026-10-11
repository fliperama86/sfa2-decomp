// SPDX-License-Identifier: MIT
/* Reconstruction of the function at 0x80165d84 (the library's name is probably
   _SsInitSoundSep, inferred). Written by this project from the listing of the
   original, with the single-sequence twin _SsInitSoundSeq of seqinit.c as an
   aid. The form that builds the original's bytes was reached after the same
   function of the PSY-Q decompilation in the PsyZ project had been read
   (decomp/src/libsnd/sepinit.c there, MIT license), so this file carries the
   license line and that project's notice is in this folder's LICENSE and
   README. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../protos.h"
#include "../../externs.h"

/* A local view of a sequence record of _ss_score (172 bytes): the fields
   this function uses, named by their offsets where no role is known. */
typedef struct {
    u8 pad00[4];
    u8 *read_pos;
    u8 *next_pos;
    u8 *loop_pos;
    u8 f10;
    u8 f11;
    u8 f12;
    u8 f13;
    u8 f14;
    u8 f15;
    u8 f16;
    u8 pan[16];
    u8 f27;
    u8 f28;
    u8 f29;
    u8 f2a;
    u8 f2b;
    u8 programs[16];
    u8 pad3c[12];
    s16 f48;
    s16 f4a;
    s16 f4c;
    s16 vol[16];
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

/* What it does (inferred): prepares the sequence record
   _ss_score[slot][index] for the sequence number index of a group that lies
   at addr, and returns how many bytes of the group that sequence takes, or
   -1 with a message when the group's header is not the expected one. It
   clears the record's state, sets the 16 channels' pan, program and volume,
   reads the resolution and the tempo (which it turns into a count per
   minute, rounded), reads the length word, takes the first delta time
   through _SsReadDeltaValue, and sets the tick counters from the
   resolution, the tempo and VBLANK_MINUS. */
int func_80165d84(s16 slot, s16 index, s16 vab_id, u8 *addr) {
    int used = 0;
    ScoreView *score = &_ss_score[slot][index];
    int ch;
    int tempo;
    /* Stand-in, not recovered source: the byte locals b0 to b3, used again for the four bytes of the length word, reproduce the original's choice of registers. Without it this function differs from the original in 41 instruction slots. */
    /* The figure is for byte locals of its own for the length word. */
    u8 b0;
    u8 b1;
    u8 b2;
    u8 b3;
    u8 *c1;
    u8 *c2;
    u8 *c3;
    int len;
    u32 ticks;
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
    if (index == 0) {
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
    c1 = score->read_pos;
    score->read_pos = c1 + 1;
    b0 = c1[0];
    score->read_pos = c1 + 2;
    b1 = c1[1];
    score->f4a = b1 | (b0 << 8);
    c2 = score->read_pos;
    score->read_pos = c2 + 1;
    b2 = c2[0];
    score->read_pos = c2 + 2;
    b3 = c2[1];
    score->read_pos = c2 + 3;
    tempo = (b2 << 16) | (b3 << 8) | c2[2];
    score->f84 = tempo;
    used += 5;
    if ((tempo / 2) < 60000000 % tempo)
        score->f84 = 60000000 / tempo + 1;
    else
        score->f84 = 60000000 / tempo;
    score->f8c = score->f84;
    c3 = score->read_pos;
    score->read_pos = c3 + 3;
    b0 = c3[2];
    score->read_pos = c3 + 4;
    b1 = c3[3];
    score->read_pos = c3 + 5;
    b2 = c3[4];
    score->read_pos = c3 + 6;
    b3 = c3[5];
    len = (b0 << 24) + (b1 << 16) + (b2 << 8) + b3;
    score->f88 = score->f7c = _SsReadDeltaValue(slot, index);
    ticks = score->f4a * score->f84;
    score->next_pos = score->read_pos;
    score->loop_pos = score->read_pos;
    used += 6;
    if ((ticks * 10) < (VBLANK_MINUS * 60)) {
        score->f70 = score->f6e = (VBLANK_MINUS * 600) / ticks;
    } else {
        score->f6e = -1;
        /* The product is written out again in this arm: with the local ticks in its place this function differs from the original in 19 instruction slots. */
        score->f70 = (score->f4a * score->f84 * 10) / (VBLANK_MINUS * 60);
        if ((VBLANK_MINUS * 30) <
            (score->f4a * score->f84 * 10) % (VBLANK_MINUS * 60))
            score->f70++;
    }
    score->f72 = score->f70;
    return used + len;
}
