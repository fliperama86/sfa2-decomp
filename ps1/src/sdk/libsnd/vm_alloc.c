// SPDX-License-Identifier: MIT
/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../protos.h"
#include "../../externs.h"

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

/*
 * Adapted from SpuVmAlloc of sotn-decomp (src/main/psxsdk/libsnd/vmanager.c,
 * MIT), with this game's voice record (0x30 bytes). Inferred: picks the voice
 * (0 up to spuVmMaxVoice - 1) that a new note takes and returns its number.
 * A free voice (f17 and f06 both 0) is taken at once; else the lowest f14
 * wins, ties going to the smaller f06, then the larger f02. With no voice
 * found the result is spuVmMaxVoice. A result below it ages every voice
 * (f02 += 1), zeroes f02 of the result, sets its f14 to _svm_cur.field_F_prior
 * and, for f17 == 2, calls SpuSetNoiseVoice(0, 0xFFFFFF). arg0 is not used.
 */
u8 SpuVmAlloc(s32 arg0) {
    u8 channel;
    u8 count;
    u8 cand;
    u8 i;
    u16 prior;
    u16 kept6;
    u16 kept2; /* an int builds 158 instructions, 21 lines differ */
    u16 v;
    channel = 99;
    kept6 = 0xFFFF;
    count = 0;
    kept2 = 0;
    prior = _svm_cur.field_F_prior;
    cand = 99;
    for (i = 0; i < spuVmMaxVoice; i++) {
        /* v is only here for the bytes: the plain test f17 == 0 && f06 == 0
           builds 158 instructions, 22 lines differ. */
        if (_svm_voice[i].f17 == 0 && (v = _svm_voice[i].f06) == 0) {
            channel = i;
            break;
        }
        if (_svm_voice[i].f14 < prior) {
            prior = _svm_voice[i].f14;
            cand = i;
            kept6 = _svm_voice[i].f06;
            kept2 = _svm_voice[i].f02;
            count = 1;
        } else if (_svm_voice[i].f14 == prior) {
            count += 1;
            if (_svm_voice[i].f06 < kept6) {
                kept2 = _svm_voice[i].f02;
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
