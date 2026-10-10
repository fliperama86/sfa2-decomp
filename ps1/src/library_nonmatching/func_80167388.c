/*
 * Nonmatching. This function is NOT byte-identical to the original: the
 * build of this C differs from the original's bytes (the printed line of the
 * test gives both sizes). The build does not use this file. The differential
 * test next to it (difftest.py, with func_80167388.py as the contract)
 * compares the behavior of this C with the original code on random inputs of
 * the contract below.
 *
 * Provenance: written by this project from the listing of the original. The
 * reference that the SDK files come from only declares a function of this
 * name and has no definition of it, so nothing is adapted. The library's
 * name for this address is _SsContDataEntry.
 *
 * What it does (inferred, not an original name): handles the value of a
 * "data entry" control change of a sequence (arg2), according to state
 * bytes of the sequence record "score" (_ss_score[arg0][arg1], 172 bytes;
 * fields named here by their offsets). It first calls SsUtGetProgAtr(
 * score.s16 at 0x4c, score.programs[channel], &prog) with channel = the
 * byte at 0x12 and programs = the 16 bytes at 0x2c. Then, with b13, b14,
 * b16, b27, b29, b2a the bytes of the score at those offsets, in this
 * order of tests, the first that holds is taken:
 *   1. b27 == 1 and the byte at 0x10 is 0: byte 0x28 = arg2, byte 0x10 = 1.
 *   2. b16 is neither 0x1e nor 0x14: byte 0x15 = arg2, b2a is increased
 *      by one.
 *   3. b29 == 2: for each tone of the program (prog.tones, a byte), when
 *      b13 == 0 and b14 == 0: SsUtGetVagAtr(vab, program, tone, &vag), then
 *      bytes 0xc and 0xd of vag are set to arg2 & 0x7f, then SsUtSetVagAtr
 *      with the same arguments; when b13 == 1 and b14 == 0, and when b13
 *      == 2 and b14 == 0, the same two calls without any change of vag.
 *      (In the last two cases the original also computes a value from
 *      arg2 and does not use it; it loads byte 5 of vag (b13 == 1) or
 *      byte 4 (b13 == 2) after the first call and stores the same value
 *      back. This C does neither.) The three tests are made one after the
 *      other on score bytes that nothing in this test changes between
 *      them. Then byte 0x29 is set to 0.
 *   4. b29 != 2 and b2a == 2: _SsSndSetVabAttr(vab, program, b16, vag,
 *      adsr, idx, attr) is called once, with the tone number b16 (0x14 or
 *      0x1e here), idx = byte 0x15 and attr = arg2 & 0xff; vag and adsr are
 *      local records that this function does not fill (the callee fills its
 *      own copies). Then byte 0x2a is set to 0.
 *   5. Otherwise nothing more.
 * In every case it ends with _SsReadDeltaValue(arg0, arg1) and stores the
 * result into the word at 0x88 of the score (after the stores of 1 and 2,
 * before the bytes 0x29 and 0x2a are cleared in 3 and 4). vab = the s16 at 0x4c; program = the byte
 * programs[channel] at the time of each call.
 *
 * Contract (the roles named for fields are inferred):
 *   Arguments: a0 = arg0 (s16, 0 to 31: index into _ss_score), a1 = arg1
 *     (s16, 0 to 3: element in the row), a2 = arg2 (u8). No result.
 *   Reads: _ss_score[arg0]; in the score: bytes 0x10, 0x12, 0x13, 0x14,
 *     0x15, 0x16, 0x27, 0x29, 0x2a, the 16 program bytes at 0x2c, the s16
 *     at 0x4c; the byte tones of prog, which SsUtGetProgAtr fills.
 *   Writes: in the score, bytes 0x10, 0x15, 0x28, 0x29, 0x2a and the word
 *     at 0x88, as listed above.
 *   Callees: all five are replaced by recorders in both runs (they reach
 *     the sound library's tables): SsUtGetProgAtr (3 arguments; the
 *     recorder stands for it by writing a word with the tone count into
 *     prog), SsUtGetVagAtr (4; the recorder fills vag with 32 bytes that
 *     the setup chose), SsUtSetVagAtr (4; its vag is logged, 8 words),
 *     _SsReadDeltaValue (2; returns a value that the setup chose) and
 *     _SsSndSetVabAttr (7 arguments, 18 words: the log holds its three
 *     s16 arguments, idx and attr; the words of the two by-value records,
 *     which are uninitialized locals, are not logged). Each recorder also
 *     logs a copy of the whole score (43 words) at the call, so that a
 *     store made after a call instead of before it is a difference. What
 *     the real callees do to the sound library is outside the test: the two
 *     recorders that stand for a callee that fills memory are models
 *     written by the contract's author from how this function uses the
 *     result, not from the callees' code.
 *   Aliasing: the score row, the table _ss_score and the recorders' blocks
 *     do not overlap.
 *   Excluded inputs: the tone count is 0 to 6; the channel byte is 0 to 15;
 *     arg0 is 0 to 31 and arg1 is 0 to 3.
 *   Not reached by any input (read from the listing): in step 4 the original
 *     has a loop over the tones, with a call for each tone, for b16 == 0x10;
 *     step 2 has already taken every b16 other than 0x14 and 0x1e, so no
 *     input gets there, and this C does not have that loop (the test's
 *     --uncovered lists it as +0x360..+0x47c). In the computation that step
 *     3 does and does not use, one arm (+0x1f8..+0x200) handles a negative
 *     product of an unsigned byte, which does not occur.
 */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

/* Local views of the records this function uses: the score (172 bytes),
   the program header and the tone record (VagAtr, 32 bytes) of the SDK, the
   18-byte envelope record passed by value. Fields are named by offsets. */
typedef struct {
    u8 pad00[0x10];
    u8 f10;
    u8 pad11;
    u8 f12;          /* the channel */
    u8 f13;
    u8 f14;
    u8 f15;
    u8 f16;
    u8 pad17[0x10];
    u8 f27;
    u8 f28;
    u8 f29;
    u8 f2a;
    u8 pad2b;
    u8 programs[16]; /* 0x2c */
    u8 pad3c[0x10];
    s16 f4c;
    u8 pad4e[0x3A];
    s32 f88;
    u8 pad8c[0x20];
} ScoreView;

typedef struct {
    u8 tones;
    u8 pad01[15];
} ProgView;

typedef struct {
    u8 pad00[0xC];
    u8 f0c;
    u8 f0d;
    u8 pad0e[0x12];
} VagView;

typedef struct {
    u16 w[9];
} AdsrView;

extern ScoreView *_ss_score[];

short SsUtGetProgAtr(short vabId, short prog, ProgView *pProg);
short SsUtGetVagAtr(short vabId, short prog, short toneNum, VagView *pVag);
short SsUtSetVagAtr(short vabId, short prog, short toneNum, VagView *pVag);
s32 _SsReadDeltaValue(s16 arg0, s16 arg1);
void _SsSndSetVabAttr(s16 vabId, s16 progNum, s16 toneNum, VagView vag, AdsrView adsr, short idx, unsigned char attr);

void func_80167388(s16 arg0, s16 arg1, u8 arg2) {
    ScoreView *score;
    ProgView prog;
    VagView vag;
    AdsrView adsr;
    u8 channel;
    int i;

    score = &_ss_score[arg0][arg1];
    channel = score->f12;
    SsUtGetProgAtr(score->f4c, score->programs[channel], &prog);
    if (score->f27 == 1 && score->f10 == 0) {
        score->f28 = arg2;
        score->f10 = 1;
        score->f88 = _SsReadDeltaValue(arg0, arg1);
        return;
    }
    if (score->f16 != 0x1E && score->f16 != 0x14) {
        score->f15 = arg2;
        score->f2a++;
        score->f88 = _SsReadDeltaValue(arg0, arg1);
        return;
    }
    if (score->f29 == 2) {
        if (score->f13 == 0 && score->f14 == 0) {
            for (i = 0; i < prog.tones; i++) {
                SsUtGetVagAtr(score->f4c, score->programs[channel], i, &vag);
                vag.f0d = vag.f0c = arg2 & 0x7F;
                SsUtSetVagAtr(score->f4c, score->programs[channel], i, &vag);
            }
        }
        if (score->f13 == 1 && score->f14 == 0) {
            for (i = 0; i < prog.tones; i++) {
                SsUtGetVagAtr(score->f4c, score->programs[channel], i, &vag);
                SsUtSetVagAtr(score->f4c, score->programs[channel], i, &vag);
            }
        }
        if (score->f13 == 2 && score->f14 == 0) {
            for (i = 0; i < prog.tones; i++) {
                SsUtGetVagAtr(score->f4c, score->programs[channel], i, &vag);
                SsUtSetVagAtr(score->f4c, score->programs[channel], i, &vag);
            }
        }
        score->f88 = _SsReadDeltaValue(arg0, arg1);
        score->f29 = 0;
        return;
    }
    if (score->f2a == 2) {
        _SsSndSetVabAttr(score->f4c, score->programs[channel], score->f16, vag, adsr, score->f15, arg2);
        score->f88 = _SsReadDeltaValue(arg0, arg1);
        score->f2a = 0;
        return;
    }
    score->f88 = _SsReadDeltaValue(arg0, arg1);
}
