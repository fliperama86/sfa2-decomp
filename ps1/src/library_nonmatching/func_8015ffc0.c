// SPDX-License-Identifier: MIT
/*
 * Nonmatching. This function is NOT byte-identical to the original: the
 * build of this C differs from the original's bytes (the printed line of the
 * test gives both sizes). The build does not use this file. The differential
 * test next to it (difftest.py, with func_8015ffc0.py as the contract)
 * compares the behavior of this C with the original code on random inputs of
 * the contract below.
 *
 * Provenance: adapted from the function SpuVmAlloc of the file
 * src/main/psxsdk/libsnd/vmanager.c of sotn-decomp (MIT; commit and license
 * as ps1/src/sdk/README.md states them). The search is the reference's; the
 * field offsets, the widths and the signedness of the compares were changed
 * to follow the listing of this game's SDK version, in which the voice
 * record is 0x30 bytes and has no fields where the reference has them.
 * The library's name for this address is SpuVmAlloc; the reference's
 * variable names are not used for the fields.
 *
 * What it does (inferred, not an original name): picks the voice (0 up to
 * spuVmMaxVoice - 1) that a new note takes, and returns its number (u8).
 * Voices are the 0x30-byte records of the table _svm_voice. The search goes
 * over the voices in order and keeps a candidate. A voice whose byte at 0x17
 * is 0 and whose halfword at 6 is 0 is free and is taken at once (the search
 * stops). Otherwise its signed halfword p at 0x14 is compared with the
 * running priority (starting at the byte _svm_cur.field_F_prior, a 16-bit
 * value): lower than it, the voice becomes the candidate (the priority takes
 * p, the count of candidates becomes 1, and the halfwords at 6 and 2 are
 * kept); equal to it, the count goes up by one and the voice replaces the
 * candidate when its halfword at 6 is smaller than the kept one (as unsigned
 * 16-bit values; the kept value starts at 0xffff), or, when those are equal,
 * when its signed halfword at 2 is larger than the kept one. With no free
 * voice and a count of 0 the result is spuVmMaxVoice; otherwise the
 * candidate. A result below spuVmMaxVoice then adds 1 to the halfword at 2
 * of every voice, sets the halfword at 2 of the result to 0 and its
 * halfword at 0x14 to _svm_cur.field_F_prior, and, when the byte at 0x17 of
 * the result is 2, calls SpuSetNoiseVoice(0, 0xffffff). The argument arg0
 * is not used.
 *
 * Contract (the roles named for fields are inferred):
 *   Arguments: a0 = arg0 (not used). Returns the voice number in v0 (the
 *     test compares v0).
 *   Reads: spuVmMaxVoice; the byte _svm_cur.field_F_prior (offset 0xf of
 *     _svm_cur); for each voice below spuVmMaxVoice, until a free one is
 *     found, the byte at 0x17 and the halfwords at 2, 6 and 0x14 of its
 *     record in _svm_voice.
 *   Writes: the halfword at 2 of every voice below spuVmMaxVoice and the
 *     halfword at 0x14 of the chosen voice, only when the result is below
 *     spuVmMaxVoice.
 *   Callees: SpuSetNoiseVoice is replaced by a recorder in both runs (it
 *     reaches the chip); it logs its address and its two arguments and a
 *     copy of the whole voice table at the call, so a store made after the
 *     call instead of before it is a difference. What it would do to the
 *     chip is outside the test.
 *   Aliasing: none (all state is the voice table and the globals above).
 *   Excluded inputs: spuVmMaxVoice is 0 to 24 (the table has 24 records;
 *     the original does not check the bound).
 *   Not reached by any input: none (the test's coverage line shows all 159
 *     instruction slots of the original executed).
 */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

/* A local view of a record of the table _svm_voice (0x30 bytes); only the
   fields this function uses are named (offsets in the comments). */
typedef struct {
    s16 f00;
    s16 f02;       /* 0x02 */
    s16 f04;
    u16 f06;       /* 0x06 */
    u8 pad08[0xC];
    s16 f14;       /* 0x14 */
    u8 pad16;
    u8 f17;        /* 0x17 */
    u8 pad18[0x18];
} VoiceView;

/* A local view of the start of _svm_cur: only the byte at 0xf is used. */
typedef struct {
    u8 pad00[0xF];
    u8 field_F_prior;
} SvmCurView;

extern VoiceView _svm_voice[];
extern SvmCurView _svm_cur;
extern u8 spuVmMaxVoice;

void SpuSetNoiseVoice(s32 on_off, u32 bits);

u8 func_8015ffc0(s32 arg0) {
    u8 channel;
    u8 count;
    u8 cand;
    u8 i;
    u16 prior;
    u16 kept6;
    int kept2;
    s16 p;

    channel = 99;
    kept6 = 0xFFFF;
    count = 0;
    kept2 = 0;
    prior = _svm_cur.field_F_prior;
    cand = 99;
    for (i = 0; i < spuVmMaxVoice; i++) {
        if (_svm_voice[i].f17 == 0 && _svm_voice[i].f06 == 0) {
            channel = i;
            break;
        }
        p = _svm_voice[i].f14;
        if (p < prior) {
            prior = p;
            cand = i;
            kept6 = _svm_voice[i].f06;
            kept2 = (u16)_svm_voice[i].f02;
            count = 1;
        } else if (p == prior) {
            count += 1;
            if (_svm_voice[i].f06 < kept6) {
                kept2 = (u16)_svm_voice[i].f02;
                kept6 = _svm_voice[i].f06;
                cand = i;
            } else if (_svm_voice[i].f06 == kept6) {
                if (kept2 < _svm_voice[i].f02) {
                    kept2 = _svm_voice[i].f02;
                    cand = i;
                }
            }
        }
    }
    if (channel == 99) {
        if (count == 0) {
            channel = spuVmMaxVoice;
        } else {
            channel = cand;
        }
    }
    if (channel < spuVmMaxVoice) {
        for (i = 0; i < spuVmMaxVoice; i++) {
            _svm_voice[i].f02++;
        }
        _svm_voice[channel].f02 = 0;
        _svm_voice[channel].f14 = _svm_cur.field_F_prior;
        if (_svm_voice[channel].f17 == 2) {
            SpuSetNoiseVoice(0, 0xFFFFFF);
        }
    }
    return channel;
}
