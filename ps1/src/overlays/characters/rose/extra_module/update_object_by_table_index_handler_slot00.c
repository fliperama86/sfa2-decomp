/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../../../game.h"
#include "../../../../protos.h"
#include "../../../../externs.h"

extern ObjectFn data_8007a0d8_slot00[];
extern ObjectFn data_8007a104_slot00[];

void update_object_by_table_index_handler_slot00(Object *obj) {
    if (game_state.gate_flag != 0) {
        obj->state_0 = 2;
        obj->field_01 = 0;
    } else {
        if (game_state.field_65 == 0) {
            data_8007a0d8_slot00[obj->table_index >> 1](obj);
        }
        if (obj->active_flag == 0) {
            set_flag_and_push_if_mode_8(obj);
        }
    }
}

void blink_copy_parent_and_call_state_2_handler_slot00(Object *obj) {
    Object *p = obj->parent;
    if (p->field_01 != 0) {
        obj->field_a4 = obj->field_a4 - 1;
        if (obj->field_a4 & 0x80) {
            obj->field_a4 = 2;
            obj->field_0c = p->field_0c;
            obj->image_row = p->image_row;
            obj->field_a5 = obj->field_a5 ^ 1;
            if (obj->field_a5 != 0) {
                obj->field_0c = 0xff;
                obj->image_row = obj->image_row + 1;
            }
        }
        data_8007a104_slot00[obj->state_2](obj);
    }
}
