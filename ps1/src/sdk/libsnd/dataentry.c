/* Reconstruction, written by this project from the listing of the original: the reference that the other files of this folder come from declares a function of this name and has no definition of it, so nothing is adapted. Names/roles inferred, not original symbols. The library's name for this function is _SsContDataEntry. */
#include "../../game.h"
#include "../../protos.h"
#include "../../externs.h"

/* Local views of the records this function uses: the score (172 bytes),
   the program header and the tone record (VagAtr, 32 bytes) of the SDK, the
   18-byte envelope record passed by value. Score fields are named by offsets. */
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
    u8 prior, mode, vol, pan, center, shift, max, min, vibW, vibT, porW, porT, pbmin, pbmax;
    u8 reserved1, reserved2;
    u16 adsr1, adsr2;
    s16 prog, vag;
    s16 reserved[4];
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

/* What it does (inferred, not an original name): handles the value of a
   "data entry" control change of a sequence (arg2), according to state
   bytes of the sequence record "score" (_ss_score[arg0][arg1], 172 bytes;
   fields named here by their offsets). It first calls SsUtGetProgAtr(
   score.s16 at 0x4c, score.programs[channel], &prog) with channel = the
   byte at 0x12 and programs = the 16 bytes at 0x2c. Then, with b13, b14,
   b16, b27, b29, b2a the bytes of the score at those offsets, in this
   order of tests, the first that holds is taken:
     1. b27 == 1 and the byte at 0x10 is 0: byte 0x28 = arg2, byte 0x10 = 1.
     2. b16 is neither 0x1e nor 0x14: byte 0x15 = arg2, b2a is increased
        by one.
     3. b29 == 2: when b13 == 0 and b14 == 0, for each tone of the program
        (prog.tones, a byte): SsUtGetVagAtr(vab, program, tone, &vag), then
        bytes 0xc and 0xd of vag are set to arg2 & 0x7f, then
        SsUtSetVagAtr(vab, program, tone, &vag). When b13 == 1 and b14 == 0,
        and again when b13 == 2 and b14 == 0, the same two calls for each
        tone, with byte 5 (b13 == 1) or byte 4 (b13 == 2) of vag ORed with a
        value computed from arg2 once before the loop. The three tests are
        made one after the other on score bytes that nothing in this test
        changes between them. Then byte 0x29 is set to 0.
     4. b29 != 2 and b2a == 2: _SsSndSetVabAttr(vab, program, b16, vag,
        adsr, idx, attr) is called once, with the tone number b16 (0x14 or
        0x1e here), idx = byte 0x15 and attr = arg2 & 0xff; vag and adsr are
        local records that this function does not fill (the callee fills its
        own copies). Then byte 0x2a is set to 0.
     5. Otherwise nothing more.
   In every case it ends with _SsReadDeltaValue(arg0, arg1) and stores the
   result into the word at 0x88 of the score (after the stores of 1 and 2,
   before the bytes 0x29 and 0x2a are cleared in 3 and 4). vab = the s16 at
   0x4c; program = the byte programs[channel] at the time of each call. */
void _SsContDataEntry(s16 arg0, s16 arg1, u8 arg2) {
    ScoreView *score;
    ProgView prog;
    VagView vag;
    AdsrView adsr;
    u8 channel;
    int i;
    u16 u;
    u16 t;
    int v;

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
                /* The masked value is held in a local: written as one assignment to both bytes, this function differs from the original in 11 instruction slots. */
                v = arg2 & 0x7F;
                vag.pbmax = v;
                vag.pbmin = v;
                SsUtSetVagAtr(score->f4c, score->programs[channel], i, &vag);
            }
        }
        if (score->f13 == 1 && score->f14 == 0) {
            /* Stand-in, not recovered source: the test and the assignment below, whose value nothing reads, reproduces the original's stack frame. Without it this function differs from the original in 34 instruction slots. */
            /* Without it the build is 1500 bytes; the original is 1504 (one instruction missing). */
            if (arg2 > 64) {
                u = (arg2 * 100 / 8192) << 13;
            }
            if (arg2 > 64 && arg2 < 128) {
                t = (arg2 * 100 / 8192) << 13;
            } else {
                t = 0;
            }
            for (i = 0; i < prog.tones; i++) {
                SsUtGetVagAtr(score->f4c, score->programs[channel], i, &vag);
                vag.shift |= t;
                SsUtSetVagAtr(score->f4c, score->programs[channel], i, &vag);
            }
        }
        if (score->f13 == 2 && score->f14 == 0) {
            if (arg2 >= 64 && arg2 < 128) {
                t = (arg2 * 25) << 8;
            } else {
                t = 0;
            }
            for (i = 0; i < prog.tones; i++) {
                SsUtGetVagAtr(score->f4c, score->programs[channel], i, &vag);
                vag.center |= t;
                SsUtSetVagAtr(score->f4c, score->programs[channel], i, &vag);
            }
        }
        score->f88 = _SsReadDeltaValue(arg0, arg1);
        score->f29 = 0;
        return;
    }
    if (score->f2a == 2) {
        if (score->f16 == 0x10) {
            for (i = 0; i < prog.tones; i++) {
                _SsSndSetVabAttr(score->f4c, score->programs[channel], i, vag, adsr, score->f15, arg2);
            }
        } else {
            _SsSndSetVabAttr(score->f4c, score->programs[channel], score->f16, vag, adsr, score->f15, arg2);
        }
        score->f88 = _SsReadDeltaValue(arg0, arg1);
        score->f2a = 0;
        return;
    }
    score->f88 = _SsReadDeltaValue(arg0, arg1);
}
