/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern u8 data_801a3fe4[];

void func_800171f0_slot27(Object *o) {
    Slot27Obj *obj = (Slot27Obj *)o;
    Rect r;

    obj->field_14 = 0x0;
    obj->field_18 = 0x0;
    r.x = 0x240;
    r.y = 0x100;
    r.w = 0x10;
    r.h = 0x20;
    func_80158028(&r, data_801a3fe4);
}

void func_80017240_slot27(Object *o) {
    Slot27Obj *obj = (Slot27Obj *)o;
    Rect r;

    obj->field_14 = 0x400000;
    obj->field_18 = 0x800;
    r.x = 0x240;
    r.y = 0x120;
    r.w = 0x10;
    r.h = 0x20;
    func_80158028(&r, data_801a3fe4);
}

void func_80017298_slot27(Object *o) {
    Slot27Obj *obj = (Slot27Obj *)o;
    Rect r;

    obj->field_14 = 0x800000;
    obj->field_18 = 0x1000;
    r.x = 0x240;
    r.y = 0x140;
    r.w = 0x10;
    r.h = 0x20;
    func_80158028(&r, data_801a3fe4);
}

void func_800172f0_slot27(Object *o) {
    Slot27Obj *obj = (Slot27Obj *)o;
    Rect r;

    obj->field_14 = 0xc00000;
    obj->field_18 = 0x1800;
    r.x = 0x240;
    r.y = 0x160;
    r.w = 0x10;
    r.h = 0x20;
    func_80158028(&r, data_801a3fe4);
}
