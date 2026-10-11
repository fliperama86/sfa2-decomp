/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

/*
 * What it does (inferred, not an original name): a per-frame update of a
 * character state that moves the object along pos_y_frac with a velocity
 * (vel_y) and an acceleration (accel_y). When the low byte of step_flags is
 * 0 it advances the position, and if vel_y is then negative it counts up
 * state_3, stores field_12a + 1 in field_1a4 and starts a sequence. In every
 * other case it hands over to update_sequence.
 */
void func_801b2560_slot04_00(Object *obj) {
    if ((u8)obj->step_flags == 0) {
        u8 count;
        u16 seq;
        obj->field_45 = 1;
        /* The word at pos_y_frac is a 32-bit value (inferred); the plain
           member access differs in 3 instruction lines. */
        *(s32 *)&obj->pos_y_frac -= obj->vel_y;
        obj->vel_y += obj->accel_y;
        if (obj->vel_y < 0) {
            count = obj->state_3 + 1;
            seq = (obj->field_12a >> 1) + 0x32;
            obj->state_3 = count;
            ((Slot04aObj *)obj)->field_1a4 = obj->field_12a + 1;
            func_801307e0(obj, seq);
            return;
        }
    }
    update_sequence(obj);
}
