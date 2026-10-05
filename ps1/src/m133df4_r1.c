/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"


void func_80133df4(u8 *base, s16 index) {
    if (player_left.field_d8 != 0) {
        data_80188d64 = 0;
        data_80188d68 = 0x30;
    } else {
        data_80188d64 = 0x30;
        data_80188d68 = 0x30;
    }
    base[(index << 4) + 8] = data_80188d64;
    base[(index << 4) + 9] = data_80188d68;
    base[(index << 4) + 0xa] = 0;
    base[(index << 4) + 0x48] = 0x30;
    base[(index << 4) + 0x49] = 0;
    base[(index << 4) + 0x4a] = 0;
    if (player_right.field_d8 != 0) {
        data_80188d65 = 0;
        data_80188d69 = 0x30;
    } else {
        data_80188d65 = 0x30;
        data_80188d69 = 0x30;
    }
    base[(index << 4) + 0x28] = data_80188d65;
    base[(index << 4) + 0x29] = data_80188d69;
    base[(index << 4) + 0x2a] = 0;
    base[(index << 4) + 0x68] = 0x30;
    base[(index << 4) + 0x69] = 0;
    base[(index << 4) + 0x6a] = 0;
}

void func_80133ed0(u8 *base, s16 index) {
    if (player_right.field_165 == 1 && game_state.field_31 == 0) {
        return;
    }
    if (data_80171c5c == 0 || data_80171c5c == 1) {
        base[(index << 4) + 0xc8] = 0;
        base[(index << 4) + 0xc9] = 0;
        base[(index << 4) + 0xca] = 0x30;
    } else if (data_80171c5c == 2) {
        base[(index << 4) + 0xc8] = 0;
        base[(index << 4) + 0xc9] = 0x30;
        base[(index << 4) + 0xca] = 0;
    } else {
        base[(index << 4) + 0xc8] = 0x30;
        base[(index << 4) + 0xc9] = 0x30;
        base[(index << 4) + 0xca] = 0;
    }
    if (data_80171c5c == 1 || data_80171c5c == 2) {
        base[(index << 4) + 0xe8] = 0;
        base[(index << 4) + 0xe9] = 0x30;
        base[(index << 4) + 0xea] = 0;
    } else {
        base[(index << 4) + 0xe8] = 0x30;
        base[(index << 4) + 0xe9] = 0x30;
        base[(index << 4) + 0xea] = 0;
    }
    base[(index << 4) + 0x108] = 0x30;
    base[(index << 4) + 0x109] = 0x30;
    base[(index << 4) + 0x10a] = 0;
}

void func_80133fd8(u8 *base, s16 index) {
    if (player_left.field_165 == 1 && game_state.field_31 == 0) {
        return;
    }
    if (data_80171c5d == 4 || data_80171c5d == 5) {
    base[(index << 4) + 0x128] = 0;
    base[(index << 4) + 0x129] = 0;
    base[(index << 4) + 0x12a] = 0x30;
    } else if (data_80171c5d == 6) {
    base[(index << 4) + 0x128] = 0;
    base[(index << 4) + 0x129] = 0x30;
    base[(index << 4) + 0x12a] = 0;
    } else {
    base[(index << 4) + 0x128] = 0x30;
    base[(index << 4) + 0x129] = 0x30;
    base[(index << 4) + 0x12a] = 0;
    }
    if (data_80171c5d == 5 || data_80171c5d == 6) {
    base[(index << 4) + 0x148] = 0;
    base[(index << 4) + 0x149] = 0x30;
    base[(index << 4) + 0x14a] = 0;
    } else {
    base[(index << 4) + 0x148] = 0x30;
    base[(index << 4) + 0x149] = 0x30;
    base[(index << 4) + 0x14a] = 0;
    }
    base[(index << 4) + 0x168] = 0x30;
    base[(index << 4) + 0x169] = 0x30;
    base[(index << 4) + 0x16a] = 0;
}
