/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"

void func_8013a204(Object *a0, Object *a1, Object *a2) {
    a1->field_61 = 3;
    if (a2->field_0d != 0x10) {
        if ((a2->field_08 & 0x80) || a2->field_0d == 7 || a2->field_0d == 4 || a2->field_0d == 0x1c) {
            a1->field_15b = 1;
        }
    }
}

void func_8013a25c(Object *a0, Object *a1, Box32 *a2) {
    if (data_80188ed0.in_04 > data_80188ed0.in_00) {
        func_8013a350(a0, a1, a2);
    } else {
        a1->field_61 = 1;
    }
}
