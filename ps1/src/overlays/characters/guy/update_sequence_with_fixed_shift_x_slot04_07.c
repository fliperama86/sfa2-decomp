/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../../game.h"
#include "../../../protos.h"
#include "../../../externs.h"

void update_sequence_with_fixed_shift_x_slot04_07(Object *obj) {
    u16 t;

    update_sequence(obj);
    t = obj->step_flags;
    if ((t & 0xff) != 0) {
        obj->step_flags = t & 0xff00;
        if (obj->facing_side == 0) {
            obj->pos_x = obj->pos_x - 0x38;
        } else {
            obj->pos_x = obj->pos_x + 0x38;
        }
    }
    if ((s16)obj->step_flags & 0x8000) {
        reset_object_to_state_1_0_sequence_0(obj);
    }
}
