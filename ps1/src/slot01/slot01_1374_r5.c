/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern int data_80055f38_slot01;
extern u8 data_80055ea4_slot01;

void func_80011864_slot01(void) {
    s16 i;
    s16 j;
    s16 k;
    for (i = 0; i < 5; i++) {
        for (j = 1; j < 0x10; j++) {
            for (k = 0; k < 0x20; k++) {
                data_801a27e4_rows[i][k * 16 + j] = 0x8421;
            }
        }
    }
    func_80137b10();
}

void func_80011918_slot01(void) {
    int moved = 0;
    if (data_801a696a & 0x80) {
        data_80055f38_slot01 = data_80055f38_slot01 - 1;
        if (data_80055f38_slot01 < 0) {
            data_80055f38_slot01 = 0;
        }
        moved = 1;
    }
    if (data_801a696a & 0x10) {
        data_80055f38_slot01 = data_80055f38_slot01 + 1;
        if (data_80055f38_slot01 >= 0x14) {
            data_80055f38_slot01 = 0x13;
        }
        moved++;
    }
    if (moved != 0) {
        data_80055ea4_slot01 = 0;
        data_8018f5a0->field_4c = 3;
        data_8018f5a0->field_4e = 0;
        data_8018f5a0->field_50 = 0;
        data_8018f5a0->field_52 = 1;
    }
}
