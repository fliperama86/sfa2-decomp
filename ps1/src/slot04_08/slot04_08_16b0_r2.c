/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void update_countdown_clear_flag_or_finish_slot04_08(Object *obj) {
    ((Slot04aObj *)obj)->field_1c8--;
    if (((Slot04aObj *)obj)->field_1c8 == 0) {
        obj->active_flag_4 = 0;
    }
    if ((s16)obj->step_flags & 0x8000) {
        reset_object_to_state_1_0_sequence_0(obj);
    } else {
        start_action_if_buttons_pressed(obj);
        update_sequence(obj);
    }
}
