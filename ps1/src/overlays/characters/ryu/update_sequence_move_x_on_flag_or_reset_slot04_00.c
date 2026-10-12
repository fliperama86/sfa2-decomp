/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../../game.h"
#include "../../../protos.h"
#include "../../../externs.h"

void update_sequence_move_x_on_flag_or_reset_slot04_00(Object *obj) {
    s16 v = obj->step_flags;
    if (v & 0x8000) {
        reset_object_to_state_1_0_sequence_0(obj);
    } else {
        if (v & 0xff) {
            if (obj->facing_side == 0) {
                *(s32 *)&obj->pos_x_frac += -0x20000;
            } else {
                *(s32 *)&obj->pos_x_frac += 0x20000;
            }
        }
        update_sequence(obj);
    }
}
