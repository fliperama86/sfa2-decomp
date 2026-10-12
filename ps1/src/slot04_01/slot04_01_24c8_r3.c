/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void init_forward_move_with_direction_sequence_slot04_01(Object *obj) {
    u8 t = obj->state_3;

    obj->active_flag_4 = 1;
    obj->field_29c = 2;
    obj->active_flag_3 = 1;
    obj->active_state = 1;
    t++;
    obj->state_3 = t;
    add_one_to_ec4_if_object_unflagged(&game_state, obj);
    if (obj->facing_side != 0) {
        obj->vel_x = 0x40000;
    } else {
        obj->vel_x = -0x40000;
    }
    set_sequence_by_side_and_mode(obj, (obj->direction_index >> 1) + 0x35);
}

void move_x_by_vel_x_or_end_with_other_code_5_slot04_01(Object *obj) {
    s16 t = obj->step_flags;

    if (t & 0x8000) {
        obj->other->other_state_code = 5;
        reset_object_to_state_1_0_0_2_sequence_1(obj);
    } else {
        if ((t & 0xff) == 0) {
            *(s32 *)&obj->pos_x_frac += obj->vel_x;
        }
        update_sequence(obj);
    }
}
