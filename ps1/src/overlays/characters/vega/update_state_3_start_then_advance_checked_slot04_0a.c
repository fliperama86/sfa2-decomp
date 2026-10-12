/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../../game.h"
#include "../../../protos.h"
#include "../../../externs.h"

void update_state_3_start_then_advance_checked_slot04_0a(Object *obj) {
    if (obj->state_3 == 0) {
        obj->state_3++;
        set_sequence_by_sound_side(obj, 3);
    } else {
        if ((s16)obj->step_flags & 0x8000) {
            obj->state_0++;
        }
        advance_sequence_checked(obj);
    }
    clear_or_set_flag_by_pos_x_limit(obj);
}

