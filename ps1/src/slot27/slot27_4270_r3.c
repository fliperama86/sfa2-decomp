/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern ObjectFn data_80027cbc_slot27[];

void func_80014500_slot27(Object *o) {
    Slot27Obj *obj = (Slot27Obj *)o;
    obj->field_12 += obj->field_4c;
    if ((s16)obj->field_12 == 0x1a0) {
        obj->field_46 = 0x18;
        obj->field_05 = obj->field_05 + 1;
    }
}

void func_80014540_slot27(Object *obj) {
    obj->field_46 = (s16)obj->field_46 - 1;
    if ((s16)obj->field_46 == 0) {
        obj->field_01 = 0;
        obj->field_05 = obj->field_05 + 1;
    }
}

void func_80014574_slot27(Object *obj) {
    func_8011f240((Slab172 *)obj);
}

void func_80014594_slot27(Object *obj) {
    data_80027cbc_slot27[obj->field_05](obj);
}

void func_800145d4_slot27(Object *obj) {
    obj->field_01 = 1;
    obj->field_50 = 8;
    obj->field_05 = obj->field_05 + 1;
}
