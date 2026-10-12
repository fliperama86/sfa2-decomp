/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../../game.h"
#include "../../../protos.h"
#include "../../../externs.h"

void shift_pos_x_on_step_flag_advance_state_3_slot04_06(Object *obj) {
    int d = 0x18;

    if ((u8)obj->step_flags != 0) {
        obj->state_3 = obj->state_3 + 1;
        if (obj->facing_side == 0) {
            d = -0x18;
        }
        obj->pos_x = d + obj->pos_x;
    }
    update_sequence(obj);
}
