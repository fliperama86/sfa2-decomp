/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"


void func_801350c0(u8 a, u8 b) {
    Object *left = &player_left;
    Object *right = left + 1;

    if (left->field_d8 == 0) {
        if (a == 3 && (u16)(left->field_c6 - 0x30) <= 0x30 && (b >= 7 || b == 0)) {
        data_80188d10 -= 1;
        if (data_80188d10 & 0x80) {
            table_80188d08[0] += data_80188d2c;
            data_80188d10 = table_80171c60[table_80188d08[0]];
        }
        if (table_80188d08[0] == 5 && data_80188d2c == 1) {
            table_80188d08[0] = 3;
            data_80188d2c = -1;
            data_80188d10 = 4;
        }
        if (table_80188d08[0] == 0 && data_80188d2c == -1) {
            table_80188d08[0] = 2;
            data_80188d2c = 1;
            data_80188d10 = 4;
        }
        }
        if (a == 3 && (u16)(left->field_c6 - 0x60) <= 0x30 && (b >= 4 || b == 0)) {
        data_80188d14 -= 1;
        if (data_80188d14 & 0x80) {
            table_80188d08[1] += data_80188d30;
            data_80188d14 = table_80171c5b[table_80188d08[1]];
        }
        if (table_80188d08[1] == 9 && data_80188d30 == 1) {
            table_80188d08[1] = 8;
            data_80188d30 = -1;
            data_80188d14 = 4;
        }
        if (table_80188d08[1] == 4 && data_80188d30 == -1) {
            table_80188d08[1] = 6;
            data_80188d30 = 1;
            data_80188d14 = 4;
        }
        }
        if (a == 3 && (s16)left->field_c6 >= 0x90) {
        data_80188d18 -= 1;
        if (data_80188d18 & 0x80) {
            table_80188d08[2] += data_80188d34;
            data_80188d18 = table_80171c57[table_80188d08[2]];
        }
        if (table_80188d08[2] == 13 && data_80188d34 == 1) {
            table_80188d08[2] = 11;
            data_80188d34 = -1;
            data_80188d18 = 4;
        }
        if (table_80188d08[2] == 8 && data_80188d34 == -1) {
            table_80188d08[2] = 10;
            data_80188d34 = 1;
            data_80188d18 = 4;
        }
        }
    } else if (a == 3 && data_80171c5e != 0) {
        data_80188d18 -= 1;
        if (data_80188d18 & 0x80) {
            table_80188d08[2] += data_80188d34;
            data_80188d18 = table_80171c57[table_80188d08[2]];
        }
        if (table_80188d08[2] == 13 && data_80188d34 == 1) {
            table_80188d08[2] = 11;
            data_80188d34 = -1;
            data_80188d18 = 4;
        }
        if (table_80188d08[2] == 8 && data_80188d34 == -1) {
            table_80188d08[2] = 10;
            data_80188d34 = 1;
            data_80188d18 = 4;
        }
    }
    if (right->field_d8 == 0) {
        if (a == 2 && (u16)(right->field_c6 - 0x30) <= 0x30 && (u8)(b - 4) >= 6) {
        data_80188d1c -= 1;
        if (data_80188d1c & 0x80) {
            table_80188d08[4] += data_80188d38;
            data_80188d1c = table_80171c60[table_80188d08[4]];
        }
        if (table_80188d08[4] == 5 && data_80188d38 == 1) {
            table_80188d08[4] = 3;
            data_80188d38 = -1;
            data_80188d1c = 4;
        }
        if (table_80188d08[4] == 0 && data_80188d38 == -1) {
            table_80188d08[4] = 2;
            data_80188d38 = 1;
            data_80188d1c = 4;
        }
        }
        if (a == 2 && (u16)(right->field_c6 - 0x60) <= 0x30 && (u8)(b - 7) >= 3) {
        data_80188d20 -= 1;
        if (data_80188d20 & 0x80) {
            table_80188d08[5] += data_80188d3c;
            data_80188d20 = table_80171c5b[table_80188d08[5]];
        }
        if (table_80188d08[5] == 9 && data_80188d3c == 1) {
            table_80188d08[5] = 8;
            data_80188d3c = -1;
            data_80188d20 = 4;
        }
        if (table_80188d08[5] == 4 && data_80188d3c == -1) {
            table_80188d08[5] = 6;
            data_80188d3c = 1;
            data_80188d20 = 4;
        }
        }
        if (a == 3 && (s16)right->field_c6 >= 0x90) {
        data_80188d24 -= 1;
        if (data_80188d24 & 0x80) {
            table_80188d08[6] += data_80188d40;
            data_80188d24 = table_80171c57[table_80188d08[6]];
        }
        if (table_80188d08[6] == 13 && data_80188d40 == 1) {
            table_80188d08[6] = 11;
            data_80188d40 = -1;
            data_80188d24 = 4;
        }
        if (table_80188d08[6] == 8 && data_80188d40 == -1) {
            table_80188d08[6] = 10;
            data_80188d40 = 1;
            data_80188d24 = 4;
        }
        }
    } else if (a == 3 && data_80171c5f != 0) {
        data_80188d24 -= 1;
        if (data_80188d24 & 0x80) {
            table_80188d08[6] += data_80188d40;
            data_80188d24 = table_80171c57[table_80188d08[6]];
        }
        if (table_80188d08[6] == 13 && data_80188d40 == 1) {
            table_80188d08[6] = 11;
            data_80188d40 = -1;
            data_80188d24 = 4;
        }
        if (table_80188d08[6] == 8 && data_80188d40 == -1) {
            table_80188d08[6] = 10;
            data_80188d40 = 1;
            data_80188d24 = 4;
        }
    }
}
