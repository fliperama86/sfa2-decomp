/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_800174a8_slot27(Object *o) {
    Slot27Obj *obj = (Slot27Obj *)o;
    Rect r;

    obj->field_14 = 0x2000000;
    obj->field_18 = 0x1;
    r.x = 0x250;
    r.y = 0x100;
    r.w = 0x10;
    r.h = 0x20;
    func_80158028(&r, data_801a3fe4);
}

void func_80017500_slot27(Object *o) {
    Slot27Obj *obj = (Slot27Obj *)o;
    Rect r;

    obj->field_14 = 0x2400000;
    obj->field_18 = 0x801;
    r.x = 0x250;
    r.y = 0x120;
    r.w = 0x10;
    r.h = 0x20;
    func_80158028(&r, data_801a3fe4);
}

void func_80017558_slot27(Object *o) {
    Slot27Obj *obj = (Slot27Obj *)o;
    Rect r;

    obj->field_14 = 0x2800000;
    obj->field_18 = 0x1001;
    r.x = 0x250;
    r.y = 0x140;
    r.w = 0x10;
    r.h = 0x20;
    func_80158028(&r, data_801a3fe4);
}

void func_800175b0_slot27(Object *o) {
    Slot27Obj *obj = (Slot27Obj *)o;
    Rect r;

    obj->field_14 = 0x2c00000;
    obj->field_18 = 0x1801;
    r.x = 0x250;
    r.y = 0x160;
    r.w = 0x10;
    r.h = 0x20;
    func_80158028(&r, data_801a3fe4);
}
