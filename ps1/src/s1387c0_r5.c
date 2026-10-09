/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"


void func_80139dbc(Object *a0, Object *a1, Box32 *a2) {
    if (a2->field_0d == 2 && table_80188ed0[4] < table_80188ed0[0] &&
        table_80188ed0[4] < table_80188ed0[2]) {
        func_8013a2a4(a0, a1, &a0->wide_boxes[a0->frame->active]);
    } else if ((a0->field_75 & 1) == 0) {
        func_8013a2a4(a0, a1, &a0->wide_boxes[a0->frame->active]);
    } else {
        func_8013a050(a0, a1);
    }
}

void func_80139e5c(Object *a0, Object *a1, Box32 *unused) {
    if ((a0->field_75 & 2) == 0) {
        func_8013a2a4(a0, a1, &a0->wide_boxes[a0->frame->active]);
    } else {
        func_8013a050(a0, a1);
    }
}

void func_80139eb4(Object *a0, Object *a1, Box32 *a2) {
    if (a0->field_0b == a1->field_0b) {
        func_8013a154(a0, a1, a2);
    } else if ((u8)func_80139f54(a0, a1, a2) != 0 || (u8)func_80139f84(a0, a1, a2) != 0) {
        func_8013ca3c(a0, a1);
    } else {
        func_8013a154(a0, a1, a2);
    }
}
