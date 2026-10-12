/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../protos.h"
#include "../../externs.h"

void store_preset_image_4_slot27(Object *o) {
    Slot27Obj *obj = (Slot27Obj *)o;
    Rect r;

    obj->preset_value = 0x1000000;
    obj->preset_param = 0x2000;
    r.x = 0x240;
    r.y = 0x180;
    r.w = 0x10;
    r.h = 0x20;
    func_80158028(&r, data_801a3fe4);
}

void store_preset_image_5_slot27(Object *o) {
    Slot27Obj *obj = (Slot27Obj *)o;
    Rect r;

    obj->preset_value = 0x1400000;
    obj->preset_param = 0x2800;
    r.x = 0x240;
    r.y = 0x1a0;
    r.w = 0x10;
    r.h = 0x20;
    func_80158028(&r, data_801a3fe4);
}

void store_preset_image_6_slot27(Object *o) {
    Slot27Obj *obj = (Slot27Obj *)o;
    Rect r;

    obj->preset_value = 0x1800000;
    obj->preset_param = 0x3000;
    r.x = 0x240;
    r.y = 0x1c0;
    r.w = 0x10;
    r.h = 0x20;
    func_80158028(&r, data_801a3fe4);
}

void store_preset_image_7_slot27(Object *o) {
    Slot27Obj *obj = (Slot27Obj *)o;
    Rect r;

    obj->preset_value = 0x1c00000;
    obj->preset_param = 0x3800;
    r.x = 0x240;
    r.y = 0x1e0;
    r.w = 0x10;
    r.h = 0x20;
    func_80158028(&r, data_801a3fe4);
}
