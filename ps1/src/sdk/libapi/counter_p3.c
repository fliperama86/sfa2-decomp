/* Part of libapi/counter.c of the SDK reference: functions SetRCnt. Made by hand in the form of the parts that
   sdkimport.py generates; the function's text is the reference's, unchanged. */
#define SDK_PART 3
// SPDX-License-Identifier: MIT
#include "common.h"

typedef struct {
    u16 rootCounter;
    s16 unk2;
    s16 mode;
    s16 : 16;
    s16 target;
    s32 : 32;
} Counter;

// https://psx-spx.consoledev.net/timers/

#ifdef SDK_PART
extern s32* D_8002D3B4;
extern volatile Counter* D_8002D3B8;
extern s32 D_8002D3BC[4];
#else
static s32* D_8002D3B4 = (s32*)0x1F801070; // _interrupt_status_register
static volatile Counter* D_8002D3B8 = (Counter*)0x1F801100; // _counters
static s32 D_8002D3BC[4] = {0x10, 0x20, 0x40, 0x01}; // _interrupt_status_masks
#endif

s32 SetRCnt(s32 spec, s16 target, s32 flags) {
    s32 i = spec & 0xFFFF;
    s32 final_mode = 0x48;
    if (i >= 3) {
        return 0;
    }
    D_8002D3B8[i].mode = 0;
    D_8002D3B8[i].target = target;

    if (i < 2u) {
        if (flags & 0x10) {
            final_mode = 0x49;
        }
        if (!(flags & 1)) {
            final_mode |= 0x100;
        }
    } else if (i == 2u) { // nb: `else` is redundant here
        if (!(flags & 1)) {
            final_mode = 0x248;
        }
    }
    if ((flags & 0x1000) != 0) {
        final_mode |= 0x10;
    }

    D_8002D3B8[i].mode = final_mode;
    return 1;
}

long GetRCnt(long spec);

s32 StartRCnt(s32 spec);

long StopRCnt(long spec);

long ResetRCnt(long spec);
