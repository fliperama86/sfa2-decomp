/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern void (*data_80022e98_slot12[])(Object *);

void func_80012944_slot12(Object *obj) {
    data_80022e98_slot12[obj->field_05](obj);
}

void func_80012984_slot12(Object *o) {
    Slot12Obj *obj = (Slot12Obj *)o;
    int v;

    *(s32 *)&o->field_14 += obj->field_50;
    obj->field_50 += obj->field_58;
    v = obj->field_46 - 1;
    obj->field_46 = v;
    if ((s16)v < 0) {
        o->pos_y = 0;
        obj->field_05++;
        if (obj->field_03 != 0) {
            o->pos_y = 0x70;
        }
    }
}

void func_800129e4_slot12(Object *obj) {
}

void func_800129ec_slot12(Object *obj) {
    func_8011f240();
}
