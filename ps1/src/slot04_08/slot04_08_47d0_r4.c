/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void follow_other_position_and_advance_sequence_slot04_08(Object *obj, Object *other) {
    if (other->field_15a != 0 || other->state_2 != 7 || other->state_3 == 4) {
        obj->state_0++;
    }
    *(s32 *)&obj->pos_x_frac = *(s32 *)&other->pos_x_frac;
    advance_sequence_checked(obj);
    obj->field_01 ^= 1;
}

void advance_sequence_checked_end_state_0_slot04_08(Object *obj, Object *unused) {
    if ((s16)obj->step_flags & 0x8000) {
        obj->state_0++;
    }
    advance_sequence_checked(obj);
    obj->field_01 = 1;
}
