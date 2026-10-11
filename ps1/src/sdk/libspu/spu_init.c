// SPDX-License-Identifier: MIT
/* Reconstruction, written by this project from the listing of the original.
   The reference that the other files of this folder come from has an
   _spu_init of another version of the library, with another order of stores;
   it was read for the names of the chip's registers, and its idle loop
   (waste_time below, as in spu_p1.c) is used here, so this file carries the
   reference's license line. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../protos.h"
#include "../../externs.h"

/* A local view of the sound chip's registers behind _spu_RXX: only what this
   function uses is named (offsets in the comments). */
typedef struct {
    u8 voice[0x180];
    u16 main_vol[2];   /* 0x180 */
    u16 rev_vol[2];    /* 0x184 */
    u16 key_on[2];     /* 0x188 */
    u16 key_off[2];    /* 0x18c */
    u16 chan_fm[2];    /* 0x190 */
    u16 noise_mode[2]; /* 0x194 */
    u16 rev_mode[2];   /* 0x198 */
    u16 gap_19c[7];
    u16 spucnt;        /* 0x1aa */
    u16 data_trans;    /* 0x1ac */
    u16 spustat;       /* 0x1ae */
    u16 cd_vol[2];     /* 0x1b0 */
    u16 ex_vol[2];     /* 0x1b4 */
} SpuRegsView;

/* A local view of the record that _spu_setVoiceRegs takes. */
typedef struct {
    u32 voice;
    u32 mask;
    s16 vol_l;
    s16 vol_r;
    u32 volmode;
    u16 pitch;
    u16 pad12;
    u32 addr;
    u32 loop_addr;
    u32 adsr;
} VoiceAttrView;

extern volatile SpuRegsView *_spu_RXX;
extern volatile u32 *D_80033514;
extern s32 _spu_transMode;
extern s32 _spu_addrMode;
extern u16 _spu_tsa;
extern s32 D_800334FC;
extern s32 _spu_mem_mode;
extern s32 _spu_mem_mode_plus;
extern s32 _spu_mem_mode_unit;
extern s32 _spu_mem_mode_unitM;
extern s32 _spu_inTransfer;
extern void (*volatile _spu_transferCallback)(void);
extern void (*volatile _spu_IRQCallback)(void);
extern u16 D_80033540[];
/* The message that _spu_writeByIO of spu_p2.c prints too; the bytes are not
   this file's. */
extern char _spu_timeout_msg[];

int printf(const char *, ...);
s32 _spu_writeByIO(u8 *addr, u32 size);
void _spu_setVoiceRegs(VoiceAttrView *attr);

static inline void waste_time(volatile s32 *p) {
    p[1] = 0xD;
    for (p[0] = 0; p[0] < 0xF0; p[0]++) {
        p[1] *= 3;
    }
}

/* What it does (inferred): the reset of the sound chip that the library's
   _SpuInit calls. arg0 is 0 for a cold start and not 0 for a hot one. It
   clears the transfer state, sets three bits of the DMA control word, clears
   the chip's control register, idles, and waits for the low 11 bits of the
   status register to clear (after 5000 polls it prints a time-out message
   and goes on). It sets the four memory-mode words, the transfer mode, and
   clears the main and reverb volumes, the key-off pair and the reverb mode.
   A cold start also clears the pairs for frequency modulation, noise, CD
   and external volume, writes the 16 bytes of D_80033540 through
   _spu_writeByIO, sets all voices from a local record through
   _spu_setVoiceRegs, and keys them on and off with four idle loops after
   each. It ends with _spu_inTransfer 1, the control register 0xc000, both
   callbacks cleared, and returns 0. */
s32 _spu_init(s32 arg0) {
    VoiceAttrView attr;
    volatile s32 w[2];

    _spu_transMode = 0;
    _spu_addrMode = 0;
    _spu_tsa = 0;
    *D_80033514 |= 0xB0000;
    _spu_RXX->spucnt = 0;
    waste_time(w);
    D_800334FC = 0;
    while (_spu_RXX->spustat & 0x7FF) {
        if (++D_800334FC > 5000) {
            printf(_spu_timeout_msg, "wait (reset)");
            break;
        }
    }
    _spu_mem_mode = 2;
    _spu_mem_mode_plus = 3;
    _spu_mem_mode_unit = 8;
    _spu_mem_mode_unitM = 7;
    _spu_RXX->data_trans = 4;
    _spu_RXX->main_vol[0] = 0;
    _spu_RXX->main_vol[1] = 0;
    _spu_RXX->rev_vol[0] = 0;
    _spu_RXX->rev_vol[1] = 0;
    _spu_RXX->key_off[0] = 0xFFFF;
    _spu_RXX->key_off[1] = 0xFFFF;
    _spu_RXX->rev_mode[0] = 0;
    _spu_RXX->rev_mode[1] = 0;
    if (arg0 == 0) {
        _spu_RXX->chan_fm[0] = 0;
        _spu_RXX->chan_fm[1] = 0;
        _spu_RXX->noise_mode[0] = 0;
        _spu_RXX->noise_mode[1] = 0;
        _spu_RXX->cd_vol[0] = 0;
        _spu_RXX->cd_vol[1] = 0;
        _spu_RXX->ex_vol[0] = 0;
        _spu_RXX->ex_vol[1] = 0;
        _spu_tsa = 0x200;
        _spu_writeByIO((u8 *)D_80033540, 0x10);
        attr.voice = 0xFFFFFFFF;
        attr.mask = 0x1F;
        attr.vol_l = 0;
        attr.vol_r = 0;
        attr.pitch = 0x3FFF;
        attr.addr = 0x200;
        attr.adsr = 0;
        _spu_addrMode = 0;
        _spu_setVoiceRegs(&attr);
        _spu_RXX->key_on[0] |= 0xFFFF;
        _spu_RXX->key_on[1] |= 0xFF;
        waste_time(w);
        waste_time(w);
        waste_time(w);
        waste_time(w);
        _spu_RXX->key_off[0] |= 0xFFFF;
        _spu_RXX->key_off[1] |= 0xFF;
        waste_time(w);
        waste_time(w);
        waste_time(w);
        waste_time(w);
    }
    _spu_inTransfer = 1;
    _spu_RXX->spucnt = 0xC000;
    _spu_transferCallback = 0;
    _spu_IRQCallback = 0;
    return 0;
}
