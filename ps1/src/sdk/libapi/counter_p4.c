/* Part of libapi/counter.c of the SDK reference: functions StartRCnt. Made by hand in the form of the parts that
   sdkimport.py generates; the function's text is the reference's, unchanged. */
#define SDK_PART 4
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

s32 StartRCnt(s32 spec) {
    s32 i = spec & 0xFFFF;
    D_8002D3B4[1] |= D_8002D3BC[i];
    return i < 3;
}

long StopRCnt(long spec);

long ResetRCnt(long spec);
