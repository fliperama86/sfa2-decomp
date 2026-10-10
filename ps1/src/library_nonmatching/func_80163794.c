// SPDX-License-Identifier: MIT
/*
 * Nonmatching. This function is NOT byte-identical to the original: the
 * build of this C differs from the original's bytes (the printed line of the
 * test gives both sizes). The build does not use this file. The differential
 * test next to it (difftest.py, with func_80163794.py as the contract)
 * compares the behavior of this C with the original code on random inputs of
 * the contract below.
 *
 * Provenance: adapted from the function SpuVmSetVol of the file
 * src/main/psxsdk/libsnd/vmanager.c of sotn-decomp (MIT; commit and license
 * as ps1/src/sdk/README.md states them). Only the outline is the
 * reference's (a SpuVmVSetUp call, then a loop over the voices that match
 * three keys and set two volume cells); the volume computation, its
 * sources, its divisors and the stores to _svm_cur were rewritten from the
 * listing of this game's SDK version, which differs from the reference's.
 * The library's name for this address is SpuVmSetVol.
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
 * _svm_cur are read as they are; SpuVmVSetUp does not set them. For each voice below spuVmMaxVoice whose
 * records in _svm_voice have the signed halfword at 0xa equal to seq_sep_no,
 * the halfword at 0x12 equal to vabId and the halfword at 0xe equal to prog
 * (all compared as signed 16-bit values), it:
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
 *
 * Contract (the roles named for fields are inferred):
 *   Arguments: a0..a2 = seq_sep_no, vabId, prog (16-bit signed, passed
 *     sign-extended), a3 = arg3 (16-bit unsigned, zero-extended), arg4 at
 *     the stack position of a fifth argument (16-bit unsigned, zero-extended).
 *     Returns the count in v0 (the test compares v0).
 *   Reads: spuVmMaxVoice; _svm_voice (halfwords at 0xa, 0xe, 0x10, 0x12 of
 *     each voice below spuVmMaxVoice); _svm_vh and the byte at 0x18 of what
 *     it points at; _svm_cur bytes at 0xa, 0xb, 0xd, 0xe; _ss_score[low
 *     byte of seq_sep_no] and halfwords 0x74 and 0x76 of the element chosen
 *     by the high byte; _svm_stereo_mono.
 *   Writes: _svm_cur halfword at 0x16, and byte 0xc and halfword 0x1a for
 *     each matching voice; _svm_sreg_buf (two halfwords) and _svm_sreg_dirty
 *     (one byte) of each matching voice.
 *   Callees: SpuVmVSetUp is replaced by a recorder in both runs: it logs its
 *     address and its two arguments (under a mask of 0xffff, they are 16-bit
 *     values) and a copy of the first 0x20 bytes of _svm_cur at the call, so
 *     the store of seq_sep_no after the call, not before it, is checked. What
 *     the real SpuVmVSetUp would write (_svm_vh, _svm_pg, _svm_tn and the
 *     bytes at 1, 6 and 7 of _svm_cur) is outside the test: the setup gives
 *     _svm_vh and _svm_cur their content before the call.
 *   Aliasing: _svm_cur, the voice table, the sreg tables, the vab header and
 *     the score rows do not overlap.
 *   Excluded inputs: spuVmMaxVoice is 0 to 24; the low byte of seq_sep_no is
 *     0 to 31 (the table _ss_score has 32 entries) and the setup gives the
 *     row of that entry room for the high byte's element.
 *   Not reached by any input: none (the test's coverage line shows all 241
 *     instruction slots of the original executed).
 */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

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

int func_80163794(s16 seq_sep_no, s16 vabId, s16 prog, u16 arg3, u16 arg4) {
    ScoreView *score;
    int count;
    u8 i;
    int x;
    u32 a;
    u32 left;
    u32 right;
    u32 p;

    score = &_ss_score[seq_sep_no & 0xFF][(seq_sep_no & 0xFF00) >> 8];
    count = 0;
    SpuVmVSetUp(vabId, prog);
    _svm_cur.f16 = seq_sep_no;
    for (i = 0; i < spuVmMaxVoice; i++) {
        if (_svm_voice[i].f0a == seq_sep_no && _svm_voice[i].f12 == vabId && _svm_voice[i].f0e == prog) {
            x = arg3 * (_svm_vh->f18 * 0x3FFF);
            x = x / 0x3F01;
            a = (u32)(x * _svm_cur.f0a * _svm_cur.f0d) / 0x3F01;
            _svm_cur.f0c = _svm_voice[i].f10;
            _svm_cur.f1a = i;
            left = (a * score->f74) / 0x7F;
            right = (a * score->f76) / 0x7F;
            p = _svm_cur.f0e;
            if (p < 0x40) {
                right = (right * p) >> 6;
            } else {
                left = (left * (0x7F - p)) >> 6;
            }
            p = _svm_cur.f0b;
            if (p < 0x40) {
                right = (right * p) >> 6;
            } else {
                left = (left * (0x7F - p)) >> 6;
            }
            p = arg4 & 0xFF;
            if (p < 0x40) {
                right = (right * p) >> 6;
            } else {
                left = (left * (0x7F - p)) >> 6;
            }
            if (_svm_stereo_mono == 1) {
                if (left < right) {
                    left = right;
                } else {
                    right = left;
                }
            }
            left = (left * left) / 0x3FFF;
            right = (right * right) / 0x3FFF;
            _svm_sreg_buf[i * 8 + 0] = left;
            _svm_sreg_buf[i * 8 + 1] = right;
            _svm_sreg_dirty[i] |= 3;
            count++;
        }
    }
    return count;
}
