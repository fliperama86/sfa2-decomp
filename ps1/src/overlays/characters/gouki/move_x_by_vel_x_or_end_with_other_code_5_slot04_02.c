/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../../game.h"
#include "../../../protos.h"
#include "../../../externs.h"

void move_x_by_vel_x_or_end_with_other_code_5_slot04_02(Object *obj) {
    s16 t = obj->step_flags;

    if (t & 0x8000) {
        obj->other->other_state_code = 5;
        reset_object_to_state_1_0_0_2_sequence_1(obj);
    } else {
        if ((t & 0xff) == 0) {
            *(s32 *)&obj->pos_x_frac += obj->vel_x;
        }
        update_sequence(obj);
    }
}
