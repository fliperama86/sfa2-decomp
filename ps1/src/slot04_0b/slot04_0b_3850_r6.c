/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b3f98_slot04_0b(Object *obj, Object *parent) {
    int idx;
    int old;

    obj->field_01 = 0;
    if (obj->field_03 != parent->kind) {
        obj->field_04++;
        return;
    }
    idx = parent->frame->field_09;
    if (idx == 0) {
        obj->field_48 = 0;
        return;
    }
    obj->pos_x = parent->pos_x;
    obj->pos_y = parent->pos_y;
    if (idx == 0x10) {
        if (obj->field_0b != 0) {
            obj->pos_x = obj->pos_x + 0x30;
        } else {
            obj->pos_x = obj->pos_x - 0x30;
        }
    }
    obj->field_0b = parent->field_0b;
    old = obj->field_48;
    obj->field_01 = 1;
    if (old == idx) {
        func_80131094(obj);
        return;
    }
    obj->field_48 = idx;
    if (parent->side == 0) {
        func_80130768(obj, idx, data_1f8000b4);
    } else {
        func_80130768(obj, idx, data_1f800164);
    }
}
