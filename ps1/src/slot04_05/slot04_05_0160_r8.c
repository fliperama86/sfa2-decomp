/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void load_velocities_from_table_4_slot04_05(Object *obj) {
    update_sequence_with_input_check(obj);
}

void update_sequence_or_call_finish_on_flag_slot04_05(Object *obj) {
    if ((s16)obj->step_flags & 0x8000) {
        reset_object_to_state_1_0_0_2_sequence_1(obj);
    } else {
        update_sequence(obj);
    }
}
