/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void store_preset_image_0_slot27(Object *o) {
    Slot27Obj *obj = (Slot27Obj *)o;
    Rect r;

    obj->preset_value = 0x0;
    obj->preset_param = 0x0;
    r.x = 0x240;
    r.y = 0x100;
    r.w = 0x10;
    r.h = 0x20;
    func_80158028(&r, data_801a3fe4);
}

void store_preset_image_1_slot27(Object *o) {
    Slot27Obj *obj = (Slot27Obj *)o;
    Rect r;

    obj->preset_value = 0x400000;
    obj->preset_param = 0x800;
    r.x = 0x240;
    r.y = 0x120;
    r.w = 0x10;
    r.h = 0x20;
    func_80158028(&r, data_801a3fe4);
}

void store_preset_image_2_slot27(Object *o) {
    Slot27Obj *obj = (Slot27Obj *)o;
    Rect r;

    obj->preset_value = 0x800000;
    obj->preset_param = 0x1000;
    r.x = 0x240;
    r.y = 0x140;
    r.w = 0x10;
    r.h = 0x20;
    func_80158028(&r, data_801a3fe4);
}

void store_preset_image_3_slot27(Object *o) {
    Slot27Obj *obj = (Slot27Obj *)o;
    Rect r;

    obj->preset_value = 0xc00000;
    obj->preset_param = 0x1800;
    r.x = 0x240;
    r.y = 0x160;
    r.w = 0x10;
    r.h = 0x20;
    func_80158028(&r, data_801a3fe4);
}
