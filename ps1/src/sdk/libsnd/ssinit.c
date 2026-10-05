// SPDX-License-Identifier: MIT
#include "common.h"
#include "libsnd_i.h"

typedef void (*SndSsMarkCallbackProc)(short seq_no, short sep_no, short data);
extern SndSsMarkCallbackProc _SsMarkCallback[32][16];

/* Adapted: separate tick variables (see ssstart.c); _SsInit takes a flag. */
extern s32 _snd_tick_vsync;
extern s32 _snd_tick_irq;
extern s32 _snd_tick_unk;
extern s32 _snd_tick_half;
extern void (*_snd_tick_prev_cb)(void);
extern s32 _snd_video_mode;
s32 GetVideoMode(void);
void SpuInitHot(void);

#ifdef SDK_PART
extern short D_80032EC0[];
extern short D_80032ED0[];
#else
static short D_80032EC0[] = {
    0x0000, 0x0000, 0x1000, 0x3000, 0x00BF, 0x0000, 0x0000, 0x0000};
static short D_80032ED0[] = {
    0x3FFF, 0x3FFF, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000};
#endif

void _SsInit(s32 flag) {
    u16* var_a2;
    int i, j;

    ResetCallback();
    if (flag == 0) {
        SpuInit();
    } else {
        SpuInitHot();
    }

    var_a2 = (u16*)0x1F801C00;
    for (i = 0; i < 24; i++) {
        for (j = 0; j < 8; j++) {
#ifdef VERSION_PC
            write_16(0x1F801C00 + (i * 8 + j) * 2, D_80032EC0[j], __FILE__,
                     __LINE__);
#else
            *var_a2++ = D_80032EC0[j];
#endif
        }
    }

    var_a2 = (u16*)0x1F801D80;
    for (i = 0; i < 16; i++) {
#ifdef VERSION_PC
        write_16(0x1F801D80 + i * 2, D_80032ED0[i], __FILE__, __LINE__);
#else
        *var_a2++ = D_80032ED0[i];
#endif
    }

    SpuVmInit(24);

    for (j = 0; j < 32; j++) {
        for (i = 0; i < 16; i++) {
            _SsMarkCallback[j][i] = 0;
        }
    }

    VBLANK_MINUS = 60;
    _snd_openflag = 0;
    _snd_tick_vsync = 0;
    _snd_tick_irq = -1;
    _snd_tick_unk = 0;
    _snd_tick_half = 0;
    _snd_tick_prev_cb = 0;
    _snd_video_mode = GetVideoMode();
    _snd_ev_flag = 0;
}

/* Adapted: in this SDK version the two public entry points are one-line
   wrappers in this file. The reference has SsInitHot in ssinit_h.c with a
   different body and no SsInit here. */
void SsInit(void) { _SsInit(0); }

void SsInitHot(void) { _SsInit(1); }
