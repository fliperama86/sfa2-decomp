/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_80017608_slot27(Object *o) {
    Slot27Obj *obj = (Slot27Obj *)o;
    Rect r;

    obj->field_14 = 0x3000000;
    obj->field_18 = 0x2001;
    r.x = 0x250;
    r.y = 0x180;
    r.w = 0x10;
    r.h = 0x20;
    func_80158028(&r, data_801a3fe4);
}

void func_80017660_slot27(Object *o) {
    Slot27Obj *obj = (Slot27Obj *)o;
    Rect r;

    obj->field_14 = 0x3400000;
    obj->field_18 = 0x2801;
    r.x = 0x250;
    r.y = 0x1a0;
    r.w = 0x10;
    r.h = 0x20;
    func_80158028(&r, data_801a3fe4);
}

void func_800176b8_slot27(Object *o) {
    Slot27Obj *obj = (Slot27Obj *)o;
    Rect r;

    obj->field_14 = 0x3800000;
    obj->field_18 = 0x3001;
    r.x = 0x250;
    r.y = 0x1c0;
    r.w = 0x10;
    r.h = 0x20;
    func_80158028(&r, data_801a3fe4);
}

void func_80017710_slot27(Object *o) {
    Slot27Obj *obj = (Slot27Obj *)o;
    Rect r;

    obj->field_14 = 0x3c00000;
    obj->field_18 = 0x3801;
    r.x = 0x250;
    r.y = 0x1e0;
    r.w = 0x10;
    r.h = 0x20;
    func_80158028(&r, data_801a3fe4);
}
