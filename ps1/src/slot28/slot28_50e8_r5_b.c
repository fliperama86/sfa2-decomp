/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern ObjectRef data_800518c8_slot28;

void func_80015724_slot28(Object *obj) {
    if ((s16)data_800518c8_slot28.p->field_3a < 0) {
        data_8018f5a0->field_60 = 0xb4;
        data_8018f5a0->field_52 = data_8018f5a0->field_52 + 1;
    }
}
