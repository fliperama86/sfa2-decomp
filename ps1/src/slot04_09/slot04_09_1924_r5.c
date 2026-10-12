/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void stop_x_motion_against_facing_then_move_slot04_09(Object *obj);
void func_801b5ba0_slot04_09(Object *obj);

void update_airborne_move_with_hit_check_slot04_09(Object *obj) {
    stop_x_motion_against_facing_then_move_slot04_09(obj);
    func_801b5ba0_slot04_09(obj);
    if (obj->vel_y >= 0) {
        if (obj->vel_y != 0 && (obj->step_flags & 1) == 0
            && (u8)check_hit_other_active_state_nonzero(obj, -0x28, 0x28, 0, 0x20)) {
            obj->step_count = 0xff;
            obj->countdown = 4;
            obj->state_3++;
            set_sequence_by_side_and_mode(obj, 0x21);
            if (obj->counter == 0) {
                add_to_meter_clamped(obj, 7);
            }
            play_side_sound_at(obj, obj->side, 5);
            play_sound_at(obj, obj->side ^ 1, 0x31a);
        } else {
            update_sequence(obj);
        }
    } else {
        obj->state_3 = 7;
        obj->vel_x = 0;
        obj->accel_x = 0;
        set_sequence_by_side_and_mode(obj, 0x23);
    }
}

void count_down_then_halve_velocities_slot04_09(Object *obj) {
    func_801b5ba0_slot04_09(obj);
    obj->countdown = (s16)obj->countdown - 1;
    if ((s16)obj->countdown < 0) {
        obj->vel_x >>= 1;
        obj->accel_x >>= 1;
        obj->vel_y >>= 1;
        obj->accel_y >>= 1;
        obj->state_3++;
        update_sequence(obj);
    }
}
