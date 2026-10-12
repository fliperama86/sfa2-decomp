/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

u16 apply_velocity_and_check_ground_slot04_0a(Object *obj);

void land_at_saved_pos_y_set_state_3_four_slot04_0a(Object *obj) {
    obj->state_3 = 4;
    play_object_sound_by_kind(obj);
    obj->pos_y_frac = 0;
    obj->pos_y = ((Slot04aObj *)obj)->saved_pos_y;
    set_sequence_by_side_and_mode(obj, 0x48);
}

void move_airborne_with_input_x_nudge_slot04_0a(Object *obj) {
    u32 t;

    if (apply_velocity_and_check_ground_slot04_0a(obj)) {
        land_at_saved_pos_y_set_state_3_four_slot04_0a(obj);
    } else {
        if (((Slot04aObj *)obj)->mode == 0 && obj->mode_flag_2 == 0) {
            t = obj->input_bits_2 & 0xa000;
            if (t != 0) {
                u32 m = t & 0x8000;

                t = 0xfffc0000;
                if (m == 0) {
                    t = 0x40000;
                }
                if (obj->facing_side != 0) {
                    t = -t;
                }
                *(s32 *)&obj->pos_x_frac = t + *(s32 *)&obj->pos_x_frac;
            }
        }
        update_sequence(obj);
    }
}

void update_sequence_or_finish_slot04_0a(Object *obj) {
    if (((s16)obj->step_flags & 0x8000) == 0) {
        update_sequence(obj);
    } else {
        reset_object_to_state_1_0_sequence_0(obj);
    }
}
