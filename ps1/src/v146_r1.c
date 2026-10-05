/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"


void func_801336a0(u8 *base, Object *a, Object *b, s16 index) {
    if ((a->field_165 | b->field_165) != 0) {
        return;
    }
    if (a->field_d8 != 0) {
        data_80188d64 = 0;
        data_80188d68 = 0xf0;
    } else {
        data_80188d64 = 0xf0;
        data_80188d68 = 0xf0;
    }
    if (b->field_d8 != 0) {
        data_80188d65 = 0;
        data_80188d69 = 0xf0;
    } else {
        data_80188d65 = 0xf0;
        data_80188d69 = 0xf0;
    }
    base[(index << 4) + 0x8] = data_80188d64;
    base[(index << 4) + 0x9] = data_80188d68;
    base[(index << 4) + 0xa] = 0;
    base[(index << 4) + 0x48] = 0xf0;
    base[(index << 4) + 0x49] = 0x30;
    base[(index << 4) + 0x4a] = 0;
    base[(index << 4) + 0x28] = data_80188d65;
    base[(index << 4) + 0x29] = data_80188d69;
    base[(index << 4) + 0x2a] = 0;
    base[(index << 4) + 0x68] = 0xf0;
    base[(index << 4) + 0x69] = 0x30;
    base[(index << 4) + 0x6a] = 0;
    base[(index << 4) + 0xc8] = 0x49;
    base[(index << 4) + 0xc9] = 0xc9;
    base[(index << 4) + 0xca] = 0xf3;
    if (a->field_d8 == 0) {
        base[(index << 4) + 0xe8] = 0x20;
        base[(index << 4) + 0xe9] = 0xf0;
        base[(index << 4) + 0xea] = 0x20;
        base[(index << 4) + 0x108] = 0xf0;
        base[(index << 4) + 0x109] = 0xf0;
        base[(index << 4) + 0x10a] = 0x20;
    } else {
        base[(index << 4) + 0xe8] = 0x49;
        base[(index << 4) + 0xe9] = 0xc9;
        base[(index << 4) + 0xea] = 0xf3;
        base[(index << 4) + 0x108] = 0x49;
        base[(index << 4) + 0x109] = 0xc9;
        base[(index << 4) + 0x10a] = 0xf3;
    }
    base[(index << 4) + 0x128] = 0x49;
    base[(index << 4) + 0x129] = 0xc9;
    base[(index << 4) + 0x12a] = 0xf3;
    if (b->field_d8 == 0) {
        base[(index << 4) + 0x148] = 0x20;
        base[(index << 4) + 0x149] = 0xf0;
        base[(index << 4) + 0x14a] = 0x20;
        base[(index << 4) + 0x168] = 0xf0;
        base[(index << 4) + 0x169] = 0xf0;
        base[(index << 4) + 0x16a] = 0x20;
    } else {
        base[(index << 4) + 0x148] = 0x49;
        base[(index << 4) + 0x149] = 0xc9;
        base[(index << 4) + 0x14a] = 0xf3;
        base[(index << 4) + 0x168] = 0x49;
        base[(index << 4) + 0x169] = 0xc9;
        base[(index << 4) + 0x16a] = 0xf3;
    }
}
