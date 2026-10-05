/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"

void func_8013a350(Object *a0, Object *a1, Box32 *a2) {
    if (a0->field_08 != 8 && a0->field_246 != 0 && a2->field_0d == 0xd) {
        a1->field_61 = 0x17;
    } else {
        a1->field_61 = a2->field_0d;
    }
    if (a1->field_61 == 0x1d) {
        a1->field_61 = 0x12;
    }
}
