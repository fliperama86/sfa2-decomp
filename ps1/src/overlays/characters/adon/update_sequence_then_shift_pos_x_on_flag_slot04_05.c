/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../../game.h"
#include "../../../protos.h"
#include "../../../externs.h"

void update_sequence_then_shift_pos_x_on_flag_slot04_05(Object *obj) {
    int d = 0x2a;

    update_sequence(obj);
    if (*(u8 *)&obj->step_flags != 0) {
        obj->state_2 = obj->state_2 + 1;
        if (obj->facing_side == 0) {
            d = -0x2a;
        }
        obj->pos_x = d + obj->pos_x;
    }
}
