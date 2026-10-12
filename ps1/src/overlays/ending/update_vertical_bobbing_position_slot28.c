/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../protos.h"
#include "../../externs.h"

extern void (*data_800306c4_slot28[])(Object *);

void update_vertical_bobbing_position_slot28(Object *o) {
    Slot28Obj *obj = (Slot28Obj *)o;
    obj->pos_y -= obj->delta_y;
    obj->delta_y += obj->field_58;
    if (obj->timer_reload != 0) {
        if (obj->delta_y >= 0) {
            obj->delta_y = 0x4000;
            obj->timer_reload = 0;
            obj->field_58 = -0x200;
        }
    } else if (obj->delta_y < 0) {
        obj->delta_y = -0x4000;
        obj->field_58 = 0x200;
        obj->timer_reload = 0xff;
    }
    obj->field_01 = 1;
}

void advance_sequence_and_show_object_slot28(Object *obj) {
    advance_sequence_checked(obj);
    obj->field_01 = 1;
}

void call_state_1_handler_c_advance_and_show_slot28(Object *obj) {
    data_800306c4_slot28[obj->state_1](obj);
    advance_sequence_checked(obj);
    obj->field_01 = 1;
}

void advance_state_1_if_variant_set_slot28(Object *obj) {
    if (obj->variant_index != 0) {
        obj->state_1 += 1;
    }
}
