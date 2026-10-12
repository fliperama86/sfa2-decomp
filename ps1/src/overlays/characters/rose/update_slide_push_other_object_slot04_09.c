/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../../game.h"
#include "../../../protos.h"
#include "../../../externs.h"

void update_slide_push_other_object_slot04_09(Object *o) {
    Slot00Obj *obj = (Slot00Obj *)o;
    obj->pos_x_hi += o->vel_x;
    o->vel_x += o->accel_x;
    if (o->vel_x == 0) {
        o->state_3++;
        o->pos_x_frac = 0;
    }
    if ((game_state.frame_counter & 3) == 0) {
        if (spawn_random_offset_object_variant_zero(o) & 0xff) {
            if (o->facing_side == 0) {
                ref_other.p->pos_x -= 0x30;
            } else {
                ref_other.p->pos_x += 0x30;
            }
        }
    }
    update_sequence(o);
}

void update_sequence_or_call_finish_on_flag_slot04_09(Object *obj) {
    if ((s16)obj->step_flags & 0x8000) {
        reset_object_to_state_1_0_0_2_sequence_1(obj);
    } else {
        update_sequence(obj);
    }
}
