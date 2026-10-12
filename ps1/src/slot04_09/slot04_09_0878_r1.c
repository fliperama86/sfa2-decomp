/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void walk_by_step_flags_or_finish_slot04_09(Object *obj) {
    s16 t = obj->step_flags;

    if (t & 0x8000) {
        reset_object_to_state_1_0_sequence_0(obj);
    } else {
        if ((t & 0xff) != 0) {
            if (obj->facing_side == 0) {
                obj->pos_x -= 2;
            } else {
                obj->pos_x += 2;
            }
        }
        update_sequence(obj);
    }
}
