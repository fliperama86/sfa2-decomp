/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"


void func_8013a2a4(Object *a0, Object *a1, Box32 *a2) {
    if (a1->field_d8 != 0) {
        func_8013a050(a0, a1);
    } else if (a2->field_0d != 4) {
        func_8013a350(a0, a1, a2);
    } else {
        a1->field_15b = 1;
        a1->field_61 = a2->field_0d;
        if (a1->field_61 == 0x1d) {
            a1->field_61 = 0x12;
        }
    }
}
