/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_80077104_slot2b(Object *obj) {
    obj->state_3++;
    obj->facing_side = obj->saved_facing;
    if (obj->direction_index != 0 && obj->hit_flag_a != 0 && (u8)func_8013f8c4(obj, -0xd, 0x14) != 0) {
        obj->state_0 = 1;
        obj->state_1 = 2;
        obj->state_2 = 0;
        obj->state_3 = 0;
    } else {
        obj->active_flag_3 = 1;
        set_sequence_and_tables_from_direction(obj);
    }
}

void update_sequence_reset_state_3_on_check_slot2b(Object *obj) {
    if ((s16)obj->step_flags < 0) {
        reset_object_to_state_1_0_sequence_0(obj);
    } else {
        if (consume_active_frame_flag(obj) & 0xff) {
            obj->state_3 = 0;
        }
        update_sequence(obj);
    }
}

void update_sequence_or_finish_slot2b(Object *obj) {
    if ((s16)obj->step_flags < 0) {
        reset_object_to_state_1_0_sequence_0(obj);
    } else {
        update_sequence(obj);
    }
}
