/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../../../game.h"
#include "../../../../protos.h"
#include "../../../../externs.h"

extern ObjectFn data_8007981c_slot2b[];
extern ObjectFn data_80079828_slot2b[];

void call_handler_by_direction_index_2_slot2b(Object *obj) {
    data_8007981c_slot2b[obj->direction_index >> 1](obj);
}

void call_handler_by_state_3_2_slot2b(Object *obj) {
    data_80079828_slot2b[obj->state_3](obj);
}

void update_state_or_start_sequence_1c_on_input_slot2b(Object *obj) {
    obj->state_3++;
    obj->facing_side = obj->saved_facing;
    if (obj->direction_index != 0 && obj->hit_flag_a != 0) {
        if ((u8)try_force_other_state_if_box_in_range(obj, -0xd, 0x14) != 0) {
            obj->state_0 = 1;
            obj->state_1 = 2;
            obj->state_2 = 0;
            obj->state_3 = 0;
            return;
        }
        if (obj->direction_index == 2 && obj->attack_param_d != 0) {
            obj->state_3 = 2;
            obj->active_flag_3 = 1;
            obj->field_157 = 0;
            obj->active_state = 1;
            add_to_meter_clamped(obj, 1);
            set_sequence_by_side_and_mode(obj, 0x1c);
            return;
        }
    }
    obj->active_flag_3 = 1;
    set_sequence_and_tables_from_direction(obj);
}
