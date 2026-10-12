/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void update_sequence_with_step_x_shift_or_finish_slot04_02(Object *obj) {
    s16 t = obj->step_flags;

    if (t < 0) {
        reset_object_to_state_1_0_sequence_0(obj);
    } else {
        if ((t & 0xff) != 0) {
            if (obj->facing_side != 0) {
                *(s32 *)&obj->pos_x_frac = *(s32 *)&obj->pos_x_frac + 0x20000;
            } else {
                *(s32 *)&obj->pos_x_frac = *(s32 *)&obj->pos_x_frac - 0x20000;
            }
        }
        update_sequence(obj);
    }
}
