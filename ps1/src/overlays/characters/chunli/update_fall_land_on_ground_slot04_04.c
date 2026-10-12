/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../../game.h"
#include "../../../protos.h"
#include "../../../externs.h"

int apply_facing_velocity_with_x_stop_slot04_04(Object *object);

void update_fall_land_on_ground_slot04_04(Object *o) {
    if (apply_facing_velocity_with_x_stop_slot04_04(o) < 0 && ((s16)o->step_flags & 0x8000)) {
        o->accel_y = -0x6000;
        if (o->pos_y > o->ground_y) {
            o->pos_y_frac = 0;
            o->active_state = 0;
            o->active_flag_3 = 0;
            o->state_3++;
            o->pos_y = o->ground_y;
            play_object_sound_by_kind(o);
            set_sequence_by_side_and_mode(o, o->direction_index + 0x54);
            return;
        }
    }
    update_sequence(o);
}

void update_sequence_or_finish_set_other_code_slot04_04(Object *o) {
    if (((s16)o->step_flags & 0x8000) == 0) {
        update_sequence(o);
    } else {
        ref_other.p = o->other;
        ref_other.p->other_state_code = 5;
        reset_object_to_state_1_0_sequence_0(o);
    }
}
