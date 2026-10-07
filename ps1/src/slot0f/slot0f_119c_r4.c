/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_800e146c_slot0f(Slot0fRece664 *src, Slot0fRece664 *dst) {
    int i;
    int j;
    for (i = 0; i < 2; i++) {
        for (j = 0; j < 8; j++) {
            dst->field_00[i].b[j] = src->field_00[i].b[j];
        }
    }
    for (i = 0; i < 8; i++) {
        dst->field_10[i] = src->field_10[i];
        dst->field_18[i] = src->field_18[i];
    }
    dst->field_20 = src->field_20;
    dst->field_21 = src->field_21;
    dst->field_22 = src->field_22;
    dst->field_23 = src->field_23;
    dst->field_24 = src->field_24;
    dst->field_25 = src->field_25;
    dst->field_26 = src->field_26;
    dst->field_27 = src->field_27;
    dst->field_28 = src->field_28;
    dst->field_29 = src->field_29;
    dst->field_2a = src->field_2a;
    dst->field_2b = src->field_2b;
    dst->field_2c = src->field_2c;
}
