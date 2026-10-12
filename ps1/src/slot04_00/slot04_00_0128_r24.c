/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern void (*data_801c0574_slot04_00[])(Object *, Object *);
void func_801b35d4_slot04_00(Object *obj, Object *parent);

void func_801b3538_slot04_00(Object *obj) {
    data_801c0574_slot04_00[obj->state_0](obj, obj->parent);
}

void func_801b3578_slot04_00(Object *obj, Object *parent) {
    obj->state_0++;
    obj->field_1c = parent->field_1c;
    obj->table_index = parent->kind;
    obj->field_0c = parent->field_0c;
    obj->field_0e = parent->field_0e;
    obj->variant_index = 0;
    func_801b35d4_slot04_00(obj, parent);
}

void func_801b35d4_slot04_00(Object *obj, Object *parent) {
    int idx;
    int old;

    obj->field_01 = 0;
    if (obj->table_index != parent->kind) {
        obj->state_0++;
        return;
    }
    idx = parent->frame->frame_param;
    if (idx == 0) {
        obj->variant_index = 0;
        return;
    }
    old = obj->variant_index;
    obj->pos_x = parent->pos_x;
    obj->pos_y = parent->pos_y;
    obj->facing_side = parent->facing_side;
    obj->field_01 = 1;
    if (old == idx) {
        advance_sequence_checked(obj);
        return;
    }
    obj->variant_index = idx;
    if (parent->side == 0) {
        func_80130768(obj, idx, data_1f8000b4);
    } else {
        func_80130768(obj, idx, data_1f800164);
    }
}
