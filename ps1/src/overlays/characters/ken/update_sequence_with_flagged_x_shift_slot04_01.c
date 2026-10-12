/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../../game.h"
#include "../../../protos.h"
#include "../../../externs.h"

void update_sequence_with_flagged_x_shift_slot04_01(Object *obj) {
    s16 t = obj->step_flags;
    int d = 0x20000;

    if ((t & 0x8000) != 0) {
        reset_object_to_state_1_0_sequence_0(obj);
    } else {
        if ((t & 0xff) != 0) {
            if (obj->facing_side == 0) {
                *(s32 *)&obj->pos_x_frac = *(s32 *)&obj->pos_x_frac - d;
            } else {
                *(s32 *)&obj->pos_x_frac = *(s32 *)&obj->pos_x_frac + d;
            }
        }
        update_sequence(obj);
    }
}
