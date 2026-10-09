/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern HudState *data_8018f5a0;
extern ObjectFn data_80027cfc_slot27[];

void func_800148a4_slot27(Object *o) {
    Slot27Obj *obj = (Slot27Obj *)o;
    int v;
    if (data_8018f5a0->field_4e >= 3 && game_state.mode != 3) {
        obj->field_12 += obj->field_4c;
        v = (s16)obj->field_12;
        if ((u32)v >= 0x381) {
            o->field_05++;
        }
    }
    func_8011ffdc(o);
}

void func_8001492c_slot27(Object *obj) {
    obj->field_01 = 0;
    obj->field_05++;
}

void func_80014940_slot27(Object *obj) {
    func_8011f240((Slab172 *)obj);
}

void func_80014960_slot27(Object *obj) {
    data_80027cfc_slot27[obj->field_05](obj);
}

void func_800149a0_slot27(Object *obj) {
    obj->field_7a = 0x10;
    obj->field_7c = 0x1e0;
    obj->field_01 = 1;
    obj->field_4c = 8;
    obj->field_05++;
}
