/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void flip_facing_on_step_flag_2_slot04_0a(Object *obj) {
    update_sequence(obj);
    if ((u8)obj->step_flags == 2) {
        obj->state_3++;
        obj->facing_side ^= 1;
    }
}

void finish_sequence_clear_status_flag_and_reset_slot04_0a(Object *obj) {
    if ((s16)obj->step_flags & 0x8000) {
        obj->status_flag = 0;
        reset_object_to_state_1_0_sequence_0(obj);
    } else {
        update_sequence(obj);
    }
}
