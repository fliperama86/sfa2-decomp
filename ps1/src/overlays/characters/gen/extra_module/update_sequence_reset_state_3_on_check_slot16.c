/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../../../game.h"
#include "../../../../protos.h"
#include "../../../../externs.h"

extern void (*data_8007e870_slot16[])(Object *);
extern void set_active_flag_3_and_call_direction_sequence_slot16(Object *obj);
extern void reset_object_to_state_1_sequence_0_slot16(Object *obj);
extern void load_velocities_from_table_slot16(Object *obj);

void update_sequence_reset_state_3_on_check_slot16(Object *obj) {
    if ((s16)obj->step_flags < 0) {
        reset_object_to_state_1_sequence_0_slot16(obj);
    } else {
        if (consume_active_frame_flag(obj) & 0xff) {
            obj->state_3 = 0;
        }
        update_sequence(obj);
    }
}

void call_handler_by_state_3_set_flag_slot16(Object *obj) {
    obj->field_157 = 1;
    data_8007e870_slot16[obj->state_3](obj);
}

void advance_state_3_restore_facing_slot16(Object *obj) {
    obj->state_3 = obj->state_3 + 1;
    obj->facing_side = obj->saved_facing;
    set_active_flag_3_and_call_direction_sequence_slot16(obj);
}

void update_sequence_reset_state_3_on_check_2_slot16(Object *obj) {
    if ((s16)obj->step_flags < 0) {
        load_velocities_from_table_slot16(obj);
    } else {
        if (consume_active_frame_flag(obj) & 0xff) {
            obj->state_3 = 0;
        }
        update_sequence(obj);
    }
}
