// SPDX-License-Identifier: MIT
/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../protos.h"
#include "../../externs.h"

/* A local view of a record of the table _svm_voice (0x30 bytes); only the
   fields this function uses are named (offsets in the comments). */
typedef struct {
    u8 pad00[0xA];
    s16 f0a;       /* 0x0a */
    u8 pad0c[2];
    s16 f0e;       /* 0x0e */
    s16 f10;       /* 0x10 */
    s16 f12;       /* 0x12 */
    u8 pad14[0x1C];
} VoiceView;

/* A local view of the start of _svm_cur (0x20 bytes); only the fields this
   function uses are named. */
typedef struct {
    u8 pad00[0xA];
    u8 f0a;        /* 0x0a */
    u8 f0b;        /* 0x0b */
    u8 f0c;        /* 0x0c */
    u8 f0d;        /* 0x0d */
    u8 f0e;        /* 0x0e */
    u8 pad0f[7];
    s16 f16;       /* 0x16 */
    u8 pad18[2];
    s16 f1a;       /* 0x1a */
    u8 pad1c[4];
} SvmCurView;

/* A local view of an element of a row of _ss_score (172 bytes). */
typedef struct {
    u8 pad00[0x74];
    u16 f74;       /* 0x74 */
    u16 f76;       /* 0x76 */
    u8 pad78[0x34];
} ScoreView;

/* A local view of the header that _svm_vh points at. */
typedef struct {
    u8 pad00[0x18];
    u8 f18;        /* 0x18 */
    u8 pad19[7];
} VabHdrView;

extern VoiceView _svm_voice[];
extern SvmCurView _svm_cur;
extern u8 spuVmMaxVoice;
extern VabHdrView *_svm_vh;
extern ScoreView *_ss_score[];
extern s16 _svm_stereo_mono;
extern u16 _svm_sreg_buf[];
extern u8 _svm_sreg_dirty[];

u32 SpuVmVSetUp(s16 vabId, s16 prog);

/*
 * Provenance: adapted from the function SpuVmSetVol of the file
 * src/main/psxsdk/libsnd/vmanager.c of sotn-decomp (MIT; commit and license
 * as ps1/src/sdk/README.md states them). Only the outline is the
 * reference's (a SpuVmVSetUp call, then a loop over the voices that match
 * three keys and set two volume cells); the volume computation, its
 * sources, its divisors and the stores to _svm_cur were rewritten from the
 * listing of this game's SDK version, which differs from the reference's.
 *
 * What it does (inferred, not an original name): sets the volume of every
 * sounding voice that belongs to one sequence/program, and returns how many
 * it changed. arg a0 = seq_sep_no (16 bits: the low byte selects a row
 * pointer of the table _ss_score, the high byte an element of 172 bytes in
 * that row, "score"), a1 = vabId, a2 = prog, a3 = arg3 (a volume, 16 bits),
 * the fifth argument arg4 (16 bits, on the stack; used as a pan). It calls
 * SpuVmVSetUp(vabId, prog) first (in the library that function sets the
 * pointers _svm_vh, _svm_pg and _svm_tn and the bytes at 1, 6 and 7 of
 * _svm_cur; its result is not used here), then stores seq_sep_no into the
 * halfword at 0x16 of _svm_cur. The bytes at 0xa, 0xb, 0xd and 0xe of
 * _svm_cur are read as they are; SpuVmVSetUp does not set them. For each
 * voice below spuVmMaxVoice whose records in _svm_voice have the signed
 * halfword at 0xa equal to seq_sep_no, the halfword at 0x12 equal to vabId
 * and the halfword at 0xe equal to prog (all compared as signed 16-bit
 * values), it:
 *   stores the low byte of the voice's halfword at 0x10 into the byte at
 *   0xc of _svm_cur and the voice number into the halfword at 0x1a;
 *   computes a = (((arg3 * (m * 0x3fff)) / 0x3f01) * c[0xa] * c[0xd]) / 0x3f01
 *   (m = the byte at 0x18 of the record that _svm_vh points at; c = the
 *   bytes of _svm_cur; the first division is signed, the second unsigned;
 *   0x3f01 is 0x7f * 0x7f);
 *   takes left = (a * h74) / 0x7f and right = (a * h76) / 0x7f (unsigned;
 *   h74 and h76 are the halfwords at 0x74 and 0x76 of the score element);
 *   applies three pans in turn, from c[0xe], from c[0xb] and from arg4 & 0xff:
 *   a pan p below 0x40 makes right = (right * p) >> 6, otherwise left =
 *   (left * (0x7f - p)) >> 6;
 *   when _svm_stereo_mono is 1, the smaller of left and right takes the
 *   larger one's value (equal: both stay);
 *   squares both and divides by 0x3fff (unsigned);
 *   stores left and right as halfwords into _svm_sreg_buf[voice * 8] and
 *   [voice * 8 + 1], ors 3 into _svm_sreg_dirty[voice], and counts it.
 * All products are 32-bit (the high half is dropped).
 */
s32 SpuVmSetVol(s16 seq_sep_no, s16 vabId, s16 prog, u16 arg3, u16 arg4) {
    ScoreView *score;
    int count;
    u8 i;
    int x;
    /* One local for the left volume and the product before it; a separate left builds 58 differing instruction slots. */
    u32 a;
    u32 right;
    u32 p;
    u32 lt;
    u32 rt;
    int y;

    score = &_ss_score[seq_sep_no & 0xFF][(seq_sep_no & 0xFF00) >> 8];
    count = 0;
    SpuVmVSetUp(vabId, prog);
    _svm_cur.f16 = seq_sep_no;
    for (i = 0; i < spuVmMaxVoice; i++) {
        if (_svm_voice[i].f0a == seq_sep_no && _svm_voice[i].f12 == vabId && _svm_voice[i].f0e == prog) {
            x = arg3 * ((_svm_vh->f18 << 14) - _svm_vh->f18);
            /* Stand-in, not recovered source: the local y, which is only a copy of x, reproduces the original's choice of registers. Without it this function differs from the original in 2 instruction slots. */
            y = x;
            x = y / 0x3F01;
            a = (u32)(x * _svm_cur.f0a * _svm_cur.f0d) / 0x3F01;
            lt = (a * score->f74) / 0x7F;
            rt = (a * score->f76) / 0x7F;
            _svm_cur.f0c = _svm_voice[i].f10;
            _svm_cur.f1a = i;
            if (_svm_cur.f0e < 0x40) {
                a = lt;
                right = (rt * _svm_cur.f0e) >> 6;
            } else {
                a = (lt * (0x7F - _svm_cur.f0e)) >> 6;
                right = rt;
            }
            if (_svm_cur.f0b < 0x40) {
                right = (right * _svm_cur.f0b) >> 6;
            } else {
                a = (a * (0x7F - _svm_cur.f0b)) >> 6;
            }
            p = arg4 & 0xFF;
            if (p < 0x40) {
                right = (right * p) >> 6;
            } else {
                a = (a * (0x7F - p)) >> 6;
            }
            if (_svm_stereo_mono == 1) {
                if (a < right) {
                    a = right;
                } else {
                    right = a;
                }
            }
            a = (a * a) / 0x3FFF;
            right = (right * right) / 0x3FFF;
            _svm_sreg_buf[i * 8] = a;
            _svm_sreg_buf[i * 8 + 1] = right;
            _svm_sreg_dirty[i] |= 3;
            count++;
        }
    }
    return count;
}
