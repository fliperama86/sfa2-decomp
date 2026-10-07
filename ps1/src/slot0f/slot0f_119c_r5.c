/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_800e1580_slot0f(Slot0fRece5c4 *src, Slot0fRece5c4 *dst) {
    int i;
    int j;
    for (i = 0; i < 6; i++) {
        dst->field_00 = src->field_00;
        dst->field_08 = src->field_08;
        dst->field_09 = src->field_09;
        dst->field_0a = src->field_0a;
        dst->field_0b = src->field_0b;
        dst->field_0c = src->field_0c;
        for (j = 0; j < 4; j++) {
            dst->field_04[j] = src->field_04[j];
        }
        src++;
        dst++;
    }
}

void func_800e1618_slot0f(u8 *src, u8 *dst) {
    int i;
    for (i = 0; i < 64; i++) {
        *dst++ = *src++;
    }
}
