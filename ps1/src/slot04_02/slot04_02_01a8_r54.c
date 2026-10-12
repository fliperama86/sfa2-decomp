/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void update_sequence_with_fixed_x_step_slot04_02(Object *obj) {
    s16 t = obj->step_flags;
    u8 a;

    if (t & 0x8000) {
        reset_object_to_state_1_0_sequence_0(obj);
        return;
    }
    a = t;
    if (a != 0) {
        if (a == 2) {
            obj->step_flags = (t & -0x100) | 1;
        }
        if (obj->facing_side != 0) {
            *(s32 *)&obj->pos_x_frac += 0x20000;
        } else {
            *(s32 *)&obj->pos_x_frac -= 0x20000;
        }
    }
    update_sequence(obj);
}
