/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void move_x_by_vel_x_facing_then_accelerate_slot04_06(Object *obj);

void start_move_with_x_velocity_slot04_06(Object *obj) {
    int one;

    obj->vel_x = 0xa0000;
    obj->state_3 = obj->state_3 + 1;
    obj->accel_x = -0xc000;
    one = 1;
    obj->active_flag_3 = one;
    set_sequence_and_tables_from_direction(obj);
    obj->field_278 = one;
}

void update_move_with_sound_and_cancel_slot04_06(Object *obj) {
    if ((s16)obj->step_flags < 0) {
        reset_object_to_state_1_0_0_2_sequence_1(obj);
    } else {
        if ((s16)obj->step_flags == 3) {
            play_sound_at(obj, obj->side, 0x324);
        }
        if ((s16)obj->step_flags == 2 && obj->vel_x >= 0) {
            move_x_by_vel_x_facing_then_accelerate_slot04_06(obj);
        } else {
            if (((*(u8 *)&obj->step_flags & 0x80) != 0 && (u8)match_pressed_direction_input(obj) != 0) || (u8)try_set_direction_from_pressed_bits(obj) != 0) {
                obj->state_3 = 0;
            }
        }
        update_sequence(obj);
    }
}
