/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../../game.h"
#include "../../../protos.h"
#include "../../../externs.h"

void nudge_pos_x_from_input_bits_slot04_0a(Object *obj) {
    int t;

    if (obj->mode_flag_2 == 0) {
        t = obj->input_bits_2 & 0xa000;
        if (t != 0) {
            if ((t & 0x8000) == 0) {
                t = 3;
            } else {
                t = -3;
            }
            if (obj->facing_side != 0) {
                t = -t;
            }
            *(u16 *)&obj->pos_x += t;
        }
    }
}
