/* Part of libapi/counter.c of the SDK reference: functions ResetRCnt. Made by hand in the form of the parts that
   sdkimport.py generates, with one change to the reference's text: the cast in ResetRCnt, see the note there. */
#define SDK_PART 2
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

s32 SetRCnt(s32 spec, s16 target, s32 flags);

long GetRCnt(long spec);

s32 StartRCnt(s32 spec);

long StopRCnt(long spec);

long ResetRCnt(long spec) {
    s32 i = spec & 0xFFFF;
    if (i >= 3) {
        return 0;
    }
    /* Stand-in, not recovered source: the cast that takes `volatile` off the pointer for this store reproduces the original's order of instructions. Without it this function differs from the original in 4 instruction slots. */
    /* The reference has `D_8002D3B8[i].rootCounter = 0;`. With it the function has 14 instructions where the original has 13. */
    ((Counter*)D_8002D3B8)[i].rootCounter = 0;
    return 1;
}
