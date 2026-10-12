/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../../game.h"
#include "../../../protos.h"
#include "../../../externs.h"

void update_sequence_then_shift_pos_x_on_end_slot04_01(Object *obj) {
    s16 t = obj->step_flags;

    if (t & 0x8000) {
        obj->state_3++;
        if (obj->facing_side == 0) {
            obj->pos_x -= 0x18;
        } else {
            obj->pos_x += 0x18;
        }
        set_sequence_by_side_and_mode(obj, 0x20);
    } else {
        if (t & 0xff) {
            obj->step_flags = t & 0xff00;
            spawn_offset_effect_and_play_sounds(obj);
        }
        update_sequence(obj);
    }
}
