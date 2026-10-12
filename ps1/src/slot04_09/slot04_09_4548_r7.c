/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void spawn_fourth_frame_child_index_four_slot04_09(Object *obj);

void update_sequence_or_reset_at_sequence_end_slot04_09(Object *obj) {
    s16 t = obj->step_flags;
    if (t & 0x8000) {
        obj->pos_x_frac = 0;
        reset_object_to_state_1_0_sequence_0(obj);
    } else {
        if ((u8)t != 0) {
            obj->active_flag_4 = 0;
            start_action_if_buttons_pressed(obj);
        } else {
            spawn_fourth_frame_child_index_four_slot04_09(obj);
        }
        update_sequence(obj);
    }
}
