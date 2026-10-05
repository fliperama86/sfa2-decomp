/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"



void func_8013a050(Object *a0, Object *a1) {
    if (a0->wide_boxes[a0->frame->active].field_0d == 0x1d) {
        a1->field_29f = 0x30;
    }
    a1->field_61 = 0xff;
    data_80188f34++;
    if (a0->field_66 != 0) {
        a1->field_289 = 0;
    } else {
        a1->field_288 = 0;
    }
}

void func_8013a0c4(Object *a0, Object *a1, Box32 *a2) {
    if (a0->field_4b != 0 ||
        (a0->field_08 != 8 && a0->field_45 != 0 && (a2->field_12 & 0x80) == 0) ||
        (a0->field_08 == 8 && (a2->field_12 & 0x80) == 0)) {
        func_8013a050(a0, a1);
    } else {
        func_8013a154(a0, a1, a2);
    }
}

void func_8013a154(Object *a0, Object *a1, Box32 *a2) {
    if (a0->field_7e != 0 && a0->field_7e < 10) {
        a1->field_61 = 7;
        a1->field_15b = 1;
    } else {
        if (a1->field_61 == 0x1d) {
            a1->field_61 = 0x12;
        }
        if (a1->field_45 != 0) {
            handlers_70a4[a2->field_0d]();
        } else {
            handlers_702c[a2->field_0d]();
        }
    }
}
