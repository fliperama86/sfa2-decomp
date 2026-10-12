/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

u8 move_ballistic_and_check_ground_slot04_0b(Object *object);

void shift_pos_x_with_sounds_on_flag_2_slot04_0b(Object *obj) {
    update_sequence(obj);
    if ((u8)obj->step_flags == 2) {
        obj->active_state = 1;
        obj->state_3++;
        play_side_sound_at(obj, obj->side, 0xf);
        play_side_sound_at(obj, obj->side, 8);
        if (obj->facing_side == 0) {
            obj->pos_x -= 0x28;
        } else {
            obj->pos_x += 0x28;
        }
    }
}

void update_rise_then_start_descent_sequence_slot04_0b(Object *obj) {
    move_ballistic_and_check_ground_slot04_0b(obj);
    if (obj->vel_y < 0) {
        obj->accel_y = -0x8000;
        obj->state_3++;
        obj->vel_x = 0;
        set_sequence_by_side_and_mode(obj, (obj->direction_index >> 1) + 0x42);
    } else {
        update_sequence(obj);
    }
}
