/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../../game.h"
#include "../../../protos.h"
#include "../../../externs.h"

/* What it does (inferred, not an original name): a per-frame update of an
   object that follows another object (parent). While the two game_state
   bytes field_65 and field_74 are both 0 and the object's vel_x is 0, it calls
   advance_sequence_checked when the low byte of the object's step_flags is 0, or, when
   that byte is not 0, the followed object's state_2 differs from the low byte
   of the object's accel_x and equals 8; in that second case it first clears
   the low byte of step_flags. Unless vel_x was not 0, it then copies the
   followed object's state_2 to accel_x. It always ends with set_flag_and_push_if_mode_8 on
   the object. */
/* The call of advance_sequence_checked passes a second argument although the callee takes one: the original has a, the object's step_flags, in the second argument register at both calls. Written with one argument, this function differs from the original in 12 instruction slots. */
void update_object_following_parent_state_2_slot06_08(Object *obj) {
    u16 a;
    Object *target = obj->parent;

    if ((game_state.field_65 | game_state.field_74) == 0) {
        if (obj->vel_x != 0) {
            goto done;
        }
        a = obj->step_flags;
        /* The call stands in two arms, once for each path that leads to it. With one call that both paths reach this function differs from the original in 9 instruction slots. */
        if ((u8)a != 0) {
            if (target->state_2 != (u8)obj->accel_x && target->state_2 == 8) {
                obj->step_flags = a & 0xff00;
                ((void (*)(Object *, int))advance_sequence_checked)(obj, a);
            }
        } else {
            ((void (*)(Object *, int))advance_sequence_checked)(obj, a);
        }
    }
    obj->accel_x = target->state_2;
done:
    set_flag_and_push_if_mode_8(obj);
}
