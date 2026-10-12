/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void land_on_ground_set_state_3_seven_slot04_0a(Object *obj);
u16 apply_velocity_and_check_ground_slot04_0a(Object *obj);
void nudge_pos_x_from_input_bits_slot04_0a(Object *obj);

void start_air_move_on_input_while_rising_slot04_0a(Object *obj) {
    u16 t;
    int k;

    if (obj->vel_y < 0) {
        t = obj->input_bits;
        if (t & 0x80) {
            k = 0;
        } else if (t & 0x10) {
            k = 1;
        } else if (t & 4) {
            k = 2;
        } else if (t & 1) {
            k = 0;
        } else {
            goto back;
        }
        obj->state_3++;
        set_sequence_by_side_and_mode(obj, k | 0x28);
        obj->vel_y = 0xfffd8000;
        obj->accel_y = -0x2000;
        add_to_meter_clamped(obj, 2);
        add_one_to_ec4_if_object_unflagged(&game_state, obj);
        play_side_sound_at(obj, obj->side, 9);
        nudge_pos_x_from_input_bits_slot04_0a(obj);
        if (apply_velocity_and_check_ground_slot04_0a(obj)) {
            land_on_ground_set_state_3_seven_slot04_0a(obj);
        }
    } else {
back:
        nudge_pos_x_from_input_bits_slot04_0a(obj);
        if (apply_velocity_and_check_ground_slot04_0a(obj)) {
            land_on_ground_set_state_3_seven_slot04_0a(obj);
        }
        update_sequence(obj);
    }
}

u16 apply_velocity_and_check_ground_slot04_0a(Object *obj) {
    u16 r = 0;

    *(s32 *)&obj->pos_x_frac = *(s32 *)&obj->pos_x_frac + obj->vel_x;
    obj->vel_x = obj->vel_x + obj->accel_x;
    *(s32 *)&obj->pos_y_frac = *(s32 *)&obj->pos_y_frac - obj->vel_y;
    obj->vel_y = obj->vel_y + obj->accel_y;
    if (obj->vel_y < 0) {
        r = obj->pos_y > obj->ground_y;
    }
    return r;
}
