/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../../game.h"
#include "../../../protos.h"
#include "../../../externs.h"

void update_sequence_or_reset_with_state_3_clear_slot04_08(Object *obj) {
    s16 t = obj->step_flags;

    if (t & 0x8000) {
        reset_object_to_state_1_0_sequence_0(obj);
    } else {
        if (t & 0xff) {
            if (consume_active_frame_flag(obj) & 0xff) {
                obj->state_3 = 0;
            }
        }
        update_sequence(obj);
    }
}
