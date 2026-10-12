/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../../game.h"
#include "../../../protos.h"
#include "../../../externs.h"

/*
 * What it does (inferred, not an original name): a per-frame update of a
 * character state that moves the object along pos_y_frac with a velocity
 * (vel_y) and an acceleration (accel_y). When the low byte of step_flags is
 * 0 it advances the position, and if vel_y is then negative it counts up
 * state_3, stores direction_index + 1 in step_count and starts a sequence. In every
 * other case it hands over to update_sequence.
 */
void update_rise_then_set_sequence_32_base_slot04_00(Object *obj) {
    if ((u8)obj->step_flags == 0) {
        u8 count;
        u16 seq;
        obj->active_state = 1;
        /* The word at pos_y_frac is a 32-bit value (inferred); the plain
           member access differs in 3 instruction lines. */
        *(s32 *)&obj->pos_y_frac -= obj->vel_y;
        obj->vel_y += obj->accel_y;
        if (obj->vel_y < 0) {
            count = obj->state_3 + 1;
            seq = (obj->direction_index >> 1) + 0x32;
            obj->state_3 = count;
            ((Slot04aObj *)obj)->step_count = obj->direction_index + 1;
            set_sequence_by_side_and_mode(obj, seq);
            return;
        }
    }
    update_sequence(obj);
}
