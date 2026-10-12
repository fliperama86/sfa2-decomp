/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void store_preset_image_8_slot27(Object *o) {
    Slot27Obj *obj = (Slot27Obj *)o;
    Rect r;

    obj->preset_value = 0x2000000;
    obj->preset_param = 0x1;
    r.x = 0x250;
    r.y = 0x100;
    r.w = 0x10;
    r.h = 0x20;
    func_80158028(&r, data_801a3fe4);
}

void store_preset_image_9_slot27(Object *o) {
    Slot27Obj *obj = (Slot27Obj *)o;
    Rect r;

    obj->preset_value = 0x2400000;
    obj->preset_param = 0x801;
    r.x = 0x250;
    r.y = 0x120;
    r.w = 0x10;
    r.h = 0x20;
    func_80158028(&r, data_801a3fe4);
}

void store_image_rect_y_140_set_preset_slot27(Object *o) {
    Slot27Obj *obj = (Slot27Obj *)o;
    Rect r;

    obj->preset_value = 0x2800000;
    obj->preset_param = 0x1001;
    r.x = 0x250;
    r.y = 0x140;
    r.w = 0x10;
    r.h = 0x20;
    func_80158028(&r, data_801a3fe4);
}

void store_image_rect_y_160_set_preset_slot27(Object *o) {
    Slot27Obj *obj = (Slot27Obj *)o;
    Rect r;

    obj->preset_value = 0x2c00000;
    obj->preset_param = 0x1801;
    r.x = 0x250;
    r.y = 0x160;
    r.w = 0x10;
    r.h = 0x20;
    func_80158028(&r, data_801a3fe4);
}
