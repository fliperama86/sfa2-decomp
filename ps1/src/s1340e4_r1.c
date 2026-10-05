/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"


void func_801340e4(u8 *base, s16 index) {
    u8 *p = (u8 *)((index << 4) + (int)base);
    p[0xc8] = 0xf0;
    p[0xc9] = 0xf0;
    p[0xca] = 0;
    p[0xe8] = 0xf0;
    p[0xe9] = 0xf0;
    p[0xea] = 0;
    p[0x108] = 0xf0;
    p[0x109] = 0xf0;
    p[0x10a] = 0;
}

void func_8013411c(u8 *base, s16 index) {
    u8 *p = (u8 *)((index << 4) + (int)base);
    p[0x128] = 0xf0;
    p[0x129] = 0xf0;
    p[0x12a] = 0;
    p[0x148] = 0xf0;
    p[0x149] = 0xf0;
    p[0x14a] = 0;
    p[0x168] = 0xf0;
    p[0x169] = 0xf0;
    p[0x16a] = 0;
}

void func_80134154(u8 *base, s16 index) {
    if (player_left.field_d8 != 0) {
        u8 *p = base + (index << 4);
        u8 c = data_80188d4c;
        u8 d, e;
        *(u16 *)(p + 0xd0) = 0x30;
        *(u16 *)(p + 0xf0) = 0x30;
        *(u16 *)(p + 0x110) = 0x30;
        d = data_80188d50;
        e = data_80188d54;
        data_80188d4c = c + 1;
        data_80188d50 = d + 1;
        data_80188d54 = e + 1;
    }
}

void func_801341c4(u8 *base, s16 index) {
    if (player_right.field_d8 != 0) {
        u8 *p = base + (index << 4);
        u8 c = data_80188d58;
        u8 d, e;
        *(u16 *)(p + 0x130) = 0x30;
        *(u16 *)(p + 0x150) = 0x30;
        *(u16 *)(p + 0x170) = 0x30;
        d = data_80188d5c;
        e = data_80188d60;
        data_80188d58 = c + 1;
        data_80188d5c = d + 1;
        data_80188d60 = e + 1;
    }
}
