/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern ObjectFn data_80079164_slot00[];
extern ObjectFn data_8007916c_slot00[];

void call_handler_by_state_3_4_slot00(Object *obj) {
    data_80079164_slot00[obj->state_3](obj);
}

void advance_state_3_restore_facing_set_sequence_3_slot00(Object *obj) {
    obj->active_flag_3 = 1;
    obj->state_3++;
    obj->facing_side = obj->saved_facing;
    set_sequence_and_tables_from_direction(obj);
}

void update_sequence_or_finish_on_flag_4_slot00(Object *obj) {
    if ((s16)obj->step_flags & 0x8000) {
        reset_object_to_state_1_0_0_2_sequence_1(obj);
    } else {
        if (consume_active_frame_flag(obj) & 0xff) {
            obj->state_3 = 0;
        }
        update_sequence(obj);
    }
}

void call_handler_by_state_3_5_slot00(Object *obj) {
    data_8007916c_slot00[obj->state_3](obj);
}
