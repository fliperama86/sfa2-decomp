/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"


void func_80134e94(u8 a, u8 b) {
    Object *left = &player_left;
    Object *right = left + 1;

    if (left->field_d8 == 0) {
        if (a == 3 && (u16)(left->field_c6 - 0x30) <= 0x30 && (b >= 7 || b == 0)) {
            data_80188d28 = table_80188d08[0];
        }
        if (a == 3 && (u16)(left->field_c6 - 0x60) <= 0x30 && (b >= 4 || b == 0)) {
            data_80188d28 = table_80188d08[1];
        }
        if (a == 3 && (s16)left->field_c6 >= 0x90) {
            data_80188d28 = table_80188d08[2];
        }
    } else if (a == 3 && data_80171c5e != 0) {
        data_80188d28 = table_80188d08[2];
    }
    if (right->field_d8 == 0) {
        if (a == 2 && (u16)(right->field_c6 - 0x30) <= 0x30 && (u8)(b - 4) >= 6) {
            data_80188d28 = table_80188d08[4];
        }
        if (a == 2 && (u16)(right->field_c6 - 0x60) <= 0x30 && (u8)(b - 7) >= 3) {
            data_80188d28 = table_80188d08[5];
        }
        if (a == 2 && (s16)right->field_c6 >= 0x90) {
            data_80188d28 = table_80188d08[6];
        }
    } else if (a == 2 && data_80171c5f != 0) {
        data_80188d28 = table_80188d08[6];
    }
}
