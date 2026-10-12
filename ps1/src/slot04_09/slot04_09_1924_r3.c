/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void update_sequence_or_finish_by_flag_bit_slot04_09(Object *obj) {
    s16 t = obj->step_flags;

    if (t & 0x8000) {
        reset_object_to_state_1_0_sequence_0(obj);
    } else {
        if ((t & 1) == 0) {
            obj->active_flag_4 = 0;
            start_action_if_buttons_pressed(obj);
        }
        update_sequence(obj);
    }
}
