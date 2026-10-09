/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_80017348_slot27(Object *o) {
    Slot27Obj *obj = (Slot27Obj *)o;
    Rect r;

    obj->field_14 = 0x1000000;
    obj->field_18 = 0x2000;
    r.x = 0x240;
    r.y = 0x180;
    r.w = 0x10;
    r.h = 0x20;
    func_80158028(&r, data_801a3fe4);
}

void func_800173a0_slot27(Object *o) {
    Slot27Obj *obj = (Slot27Obj *)o;
    Rect r;

    obj->field_14 = 0x1400000;
    obj->field_18 = 0x2800;
    r.x = 0x240;
    r.y = 0x1a0;
    r.w = 0x10;
    r.h = 0x20;
    func_80158028(&r, data_801a3fe4);
}

void func_800173f8_slot27(Object *o) {
    Slot27Obj *obj = (Slot27Obj *)o;
    Rect r;

    obj->field_14 = 0x1800000;
    obj->field_18 = 0x3000;
    r.x = 0x240;
    r.y = 0x1c0;
    r.w = 0x10;
    r.h = 0x20;
    func_80158028(&r, data_801a3fe4);
}

void func_80017450_slot27(Object *o) {
    Slot27Obj *obj = (Slot27Obj *)o;
    Rect r;

    obj->field_14 = 0x1c00000;
    obj->field_18 = 0x3800;
    r.x = 0x240;
    r.y = 0x1e0;
    r.w = 0x10;
    r.h = 0x20;
    func_80158028(&r, data_801a3fe4);
}
