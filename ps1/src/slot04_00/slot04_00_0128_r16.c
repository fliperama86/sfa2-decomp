/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void update_sequence_with_countdown_or_finish_slot04_00(Object *obj) {
    s16 t;

    if ((s16)obj->step_flags & 0x8000) {
        reset_object_to_state_1_0_sequence_0(obj);
    } else {
        t = obj->countdown;
        if (t != 0) {
            t = t - 1;
            obj->countdown = t;
            if (t != 0) {
                goto tail;
            }
            obj->active_flag_4 = 0;
        }
        start_action_if_buttons_pressed(obj);
    tail:
        update_sequence(obj);
    }
}
