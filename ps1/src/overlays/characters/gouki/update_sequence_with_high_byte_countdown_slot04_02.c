/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../../game.h"
#include "../../../protos.h"
#include "../../../externs.h"

void update_sequence_with_high_byte_countdown_slot04_02(Object *obj) {
    s16 t;

    if (!((s16)obj->step_flags & 0x8000)) {
        t = obj->countdown;
        if (t & 0xff00) {
            t = t - 0x100;
            obj->countdown = t;
            if (!(t & 0xff00)) {
                obj->active_flag_4 = 0;
                start_action_if_buttons_pressed(obj);
            }
        } else {
            start_action_if_buttons_pressed(obj);
        }
        update_sequence(obj);
    } else {
        reset_object_to_state_1_0_sequence_0(obj);
    }
}
