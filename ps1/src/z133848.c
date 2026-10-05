/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"



void func_80133848(u8 *base, Object *a, Object *b) {
    u16 raw = data_801a27d0;
    s16 index;
    u8 *p;

    if ((a->field_165 | b->field_165) == 0) {
        return;
    }
    if (a->field_165 != 1 && b->field_165 != 1) {
        func_80133df4(base, (s16)raw);
    }
    index = (s16)raw;
    func_80133ed0(base, index);
    func_80133fd8(base, index);

    if (a->field_165 != 0 && a->field_299 == 0) {
        p = base + (index << 4);
        *(u16 *)(p + 0xcc) = 0x76;
        *(u16 *)(p + 0xec) = 0x46;
        *(u16 *)(p + 0x10c) = 0x16;
        data_80188d44 = *(u8 *)&a->field_c6 + (a->field_255 >> 1) * 0x30 + 0x30;
        if (data_80188d44 >= 0x90) {
            u8 t = data_80188d54 + 1;
            *(u16 *)(p + 0x110) = 0x30;
            data_80188d54 = t;
            if (a->field_255 >= 2) {
                u8 t = data_80188d50 + 1;
                *(u16 *)(p + 0xf0) = 0x30;
                data_80188d50 = t;
            }
            if (a->field_255 >= 4) {
                u8 t = data_80188d4c;
                *(u16 *)(p + 0xd0) = 0x30;
                data_80188d4c = t + 1;
            }
        } else if (data_80188d44 >= 0x60) {
            u8 t = data_80188d50 + 1;
            *(u16 *)(p + 0xf0) = 0x30;
            data_80188d50 = t;
            if (a->field_255 >= 2) {
                u8 t = data_80188d4c;
                *(u16 *)(p + 0xd0) = 0x30;
                data_80188d4c = t + 1;
            }
        } else {
            u8 t = data_80188d4c;
            *(u16 *)(p + 0xd0) = 0x30;
            data_80188d4c = t + 1;
        }
        index = (s16)raw;
        func_80134154(base, index);
        if (game_state.field_32 & 4) {
            if (data_80188d4c != 0) {
                u8 *q = (u8 *)((index << 4) + (int)base);
                q[0xc8] = 0x29;
                q[0xc9] = 0xa9;
                q[0xca] = 0xd3;
            }
            if (data_80188d50 != 0) {
                u8 *q = (u8 *)((index << 4) + (int)base);
                q[0xe8] = 0x29;
                q[0xe9] = 0xa9;
                q[0xea] = 0xd3;
            }
            if (data_80188d54 != 0) {
                u8 *q = (u8 *)((index << 4) + (int)base);
                q[0x108] = 0x29;
                q[0x109] = 0xa9;
                q[0x10a] = 0xd3;
            }
        } else {
            if (data_80188d4c != 0) {
                u8 *q = (u8 *)((index << 4) + (int)base);
                q[0xc8] = 0xf0;
                q[0xc9] = 0xf0;
                q[0xca] = 0;
            }
            if (data_80188d50 != 0) {
                u8 *q = (u8 *)((index << 4) + (int)base);
                q[0xe8] = 0xf0;
                q[0xe9] = 0xf0;
                q[0xea] = 0;
            }
            if (data_80188d54 != 0) {
                u8 *q = (u8 *)((index << 4) + (int)base);
                q[0x108] = 0xf0;
                q[0x109] = 0xf0;
                q[0x10a] = 0;
            }
        }
    }

    if (b->field_165 != 0 && b->field_299 == 0) {
        data_80188d48 = *(u8 *)&b->field_c6 + (b->field_255 >> 1) * 0x30 + 0x30;
        if (data_80188d48 >= 0x90) {
            u8 *q = base + ((s16)raw << 4);
            u8 t = data_80188d60 + 1;
            *(u16 *)(q + 0x170) = 0x30;
            data_80188d60 = t;
            if (b->field_255 >= 2) {
                u8 t = data_80188d5c + 1;
                *(u16 *)(q + 0x150) = 0x30;
                data_80188d5c = t;
            }
            if (b->field_255 >= 4) {
                u8 t = data_80188d58;
                *(u16 *)(q + 0x130) = 0x30;
                data_80188d58 = t + 1;
            }
        } else if (data_80188d48 >= 0x60) {
            u8 *q = base + ((s16)raw << 4);
            u8 t = data_80188d5c + 1;
            *(u16 *)(q + 0x150) = 0x30;
            data_80188d5c = t;
            if (b->field_255 >= 2) {
                u8 t = data_80188d58;
                *(u16 *)(q + 0x130) = 0x30;
                data_80188d58 = t + 1;
            }
        } else {
            u8 *q = base + ((s16)raw << 4);
            u8 t = data_80188d58;
            *(u16 *)(q + 0x130) = 0x30;
            data_80188d58 = t + 1;
        }
        index = (s16)raw;
        func_801341c4(base, index);
        if (game_state.field_32 & 8) {
            if (data_80188d58 != 0) {
                u8 *q = (u8 *)((index << 4) + (int)base);
                q[0x128] = 0x29;
                q[0x129] = 0xa9;
                q[0x12a] = 0xd3;
            }
            if (data_80188d5c != 0) {
                u8 *q = (u8 *)((index << 4) + (int)base);
                q[0x148] = 0x29;
                q[0x149] = 0xa9;
                q[0x14a] = 0xd3;
            }
            if (data_80188d60 != 0) {
                u8 *q = (u8 *)((index << 4) + (int)base);
                q[0x168] = 0x29;
                q[0x169] = 0xa9;
                q[0x16a] = 0xd3;
            }
        } else {
            if (data_80188d58 != 0) {
                u8 *q = (u8 *)((index << 4) + (int)base);
                q[0x128] = 0xf0;
                q[0x129] = 0xf0;
                q[0x12a] = 0;
            }
            if (data_80188d5c != 0) {
                u8 *q = (u8 *)((index << 4) + (int)base);
                q[0x148] = 0xf0;
                q[0x149] = 0xf0;
                q[0x14a] = 0;
            }
            if (data_80188d60 != 0) {
                u8 *q = (u8 *)((index << 4) + (int)base);
                q[0x168] = 0xf0;
                q[0x169] = 0xf0;
                q[0x16a] = 0;
            }
        }
    }
}
